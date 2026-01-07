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
}

std::vector<std::pair<size_t, std::array<double, 3>>> 
ThreadedNBodySimulation::calculateForcesChunk(size_t start_idx, size_t end_idx) const {
    std::vector<std::pair<size_t, std::array<double, 3>>> results;
    results.reserve(end_idx - start_idx);
    
    for (size_t i = start_idx; i < end_idx; ++i) {
        std::array<double, 3> force = {0.0, 0.0, 0.0};
        for (size_t j = 0; j < num_bodies_; ++j) {
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
    size_t chunk_size = std::max(size_t(1), num_bodies_ / num_threads_);
    std::vector<std::future<std::vector<std::pair<size_t, std::array<double, 3>>>>> futures;
    
    for (size_t i = 0; i < num_bodies_; i += chunk_size) {
        size_t end_idx = std::min(i + chunk_size, size_t(num_bodies_));
        futures.push_back(std::async(std::launch::async, 
            &ThreadedNBodySimulation::calculateForcesChunk, 
            this, i, end_idx));
    }
    
    // Collect results
    std::vector<std::array<double, 3>> forces(num_bodies_, {0.0, 0.0, 0.0});
    
    for (auto& future : futures) {
        auto chunk_results = future.get();
        for (const auto& [idx, force] : chunk_results) {
            forces[idx] = force;
        }
    }
    
    return forces;
}

void ThreadedNBodySimulation::step(double dt) {
    // Calculate forces in parallel
    std::vector<std::array<double, 3>> forces = calculateForcesParallel();
    
    // Update positions and velocities
    for (size_t i = 0; i < bodies_.size(); ++i) {
        updatePosition(bodies_[i], forces[i], dt);
    }
    
    time_elapsed_ += dt;
    step_count_++;
}

void ThreadedNBodySimulation::run(int num_steps, double dt, bool verbose) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (int step = 0; step < num_steps; ++step) {
        step(dt);
        
        if (verbose && (step + 1) % std::max(1, num_steps / 10) == 0) {
            auto [kinetic, potential, total] = getEnergy();
            std::cout << "Step " << (step + 1) << "/" << num_steps 
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

