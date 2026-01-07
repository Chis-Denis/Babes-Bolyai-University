#include "threaded_simulation.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <thread>
#include <future>

ThreadedNBodySimulation::ThreadedNBodySimulation(std::vector<Body> bodies, int num_threads)
    : bodies_(std::move(bodies)), num_bodies_(bodies_.size()), time_elapsed_(0.0), step_count_(0) {
    
    if (num_threads <= 0) {
        num_threads_ = std::thread::hardware_concurrency();
        if (num_threads_ == 0) {
            num_threads_ = 4;  // Default fallback
        }
    } else {
        num_threads_ = num_threads;
    }
    
    // Initialize previous forces for Velocity Verlet
    prev_forces_.resize(bodies_.size(), {0.0, 0.0, 0.0});
}

std::vector<std::pair<size_t, std::array<double, 3>>> 
ThreadedNBodySimulation::calculateForcesChunk(size_t start_idx, size_t end_idx) const {
    std::vector<std::pair<size_t, std::array<double, 3>>> results;
    results.reserve(end_idx - start_idx);
    
    size_t num_bodies_size = static_cast<size_t>(num_bodies_);
    for (size_t i = start_idx; i < end_idx; ++i) {
        std::array<double, 3> force = {0.0, 0.0, 0.0};
        for (size_t j = 0; j < num_bodies_size; ++j) {
            if (i != j) {
                std::array<double, 3> f = calculateForce(bodies_[i], bodies_[j]);
                for (int k = 0; k < 3; ++k) {
                    force[k] += f[k];
                }
            }
        }
        results.emplace_back(i, force);
    }
    
    return results;
}

std::vector<std::array<double, 3>> 
ThreadedNBodySimulation::calculateForcesParallel() const {
    // Divide work among threads
    size_t num_bodies_size = static_cast<size_t>(num_bodies_);
    size_t num_threads_size = static_cast<size_t>(num_threads_);
    size_t chunk_size = std::max(size_t(1), num_bodies_size / num_threads_size);
    std::vector<std::future<std::vector<std::pair<size_t, std::array<double, 3>>>>> futures;
    
    for (size_t i = 0; i < num_bodies_size; i += chunk_size) {
        size_t end_idx = std::min(i + chunk_size, num_bodies_size);
        futures.push_back(std::async(std::launch::async, 
            &ThreadedNBodySimulation::calculateForcesChunk, 
            this, i, end_idx));
    }
    
    // Collect results
    std::vector<std::array<double, 3>> forces(num_bodies_size, {0.0, 0.0, 0.0});
    
    for (auto& future : futures) {
        auto chunk_results = future.get();
        for (const auto& [idx, force] : chunk_results) {
            forces[idx] = force;
        }
    }
    
    return forces;
}

void ThreadedNBodySimulation::step(double dt) {
    // Leapfrog integration requires completing the velocity half-step from previous iteration
    static bool first_step = true;
    
    if (!first_step) {
        // Complete the velocity half-step from previous iteration
        // (forces were calculated at the end of previous step)
        std::vector<std::array<double, 3>> prev_forces = calculateForcesParallel();
        for (size_t i = 0; i < bodies_.size(); ++i) {
            for (int j = 0; j < 3; ++j) {
                bodies_[i].velocity[j] += (prev_forces[i][j] / bodies_[i].mass) * (dt * 0.5);
            }
        }
    }
    
    // Calculate forces at current positions
    std::vector<std::array<double, 3>> forces = calculateForcesParallel();
    
    // Update positions and velocities (half-step velocity, full-step position)
    for (size_t i = 0; i < bodies_.size(); ++i) {
        // Half-step velocity update
        for (int j = 0; j < 3; ++j) {
            bodies_[i].velocity[j] += (forces[i][j] / bodies_[i].mass) * (dt * 0.5);
        }
        // Full-step position update
        for (int j = 0; j < 3; ++j) {
            bodies_[i].position[j] += bodies_[i].velocity[j] * dt;
        }
    }
    
    first_step = false;
    time_elapsed_ += dt;
    step_count_++;
}

void ThreadedNBodySimulation::run(int num_steps, double dt, bool verbose) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (int step_num = 0; step_num < num_steps; ++step_num) {
        step(dt);
        
        if (verbose && (step_num + 1) % std::max(1, num_steps / 10) == 0) {
            auto [kinetic, potential, total] = getEnergy();
            std::cout << "Step " << (step_num + 1) << "/" << num_steps 
                      << " | Time: " << std::scientific << std::setprecision(2) << time_elapsed_
                      << "s | Energy: " << total << " J" << std::endl;
        }
    }
    
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start_time).count();
    
    if (verbose) {
        std::cout << "\nSimulation completed in " << elapsed_time << " ms" << std::endl;
        std::cout << "Average time per step: " 
                  << static_cast<double>(elapsed_time) / num_steps << " ms" << std::endl;
        std::cout << "Using " << num_threads_ << " threads" << std::endl;
    }
}

std::tuple<double, double, double> ThreadedNBodySimulation::getEnergy() const {
    auto [kinetic, potential] = calculateEnergy(bodies_);
    return {kinetic, potential, kinetic + potential};
}

ThreadedNBodySimulation::Statistics ThreadedNBodySimulation::getStatistics() const {
    auto [kinetic, potential, total] = getEnergy();
    
    // Calculate center of mass
    double total_mass = 0.0;
    std::array<double, 3> com = {0.0, 0.0, 0.0};
    
    for (const auto& body : bodies_) {
        total_mass += body.mass;
        for (int i = 0; i < 3; ++i) {
            com[i] += body.position[i] * body.mass;
        }
    }
    
    if (total_mass > 0) {
        for (int i = 0; i < 3; ++i) {
            com[i] /= total_mass;
        }
    }
    
    return {
        num_bodies_,
        num_threads_,
        time_elapsed_,
        step_count_,
        kinetic,
        potential,
        total,
        com
    };
}

