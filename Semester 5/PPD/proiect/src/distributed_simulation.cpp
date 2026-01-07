#include "distributed_simulation.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <numeric>

DistributedNBodySimulation::DistributedNBodySimulation(std::vector<Body> bodies) {
    comm_ = MPI_COMM_WORLD;
    MPI_Comm_rank(comm_, &rank_);
    MPI_Comm_size(comm_, &size_);
    
    // Distribute bodies across processes
    if (rank_ == 0) {
        num_bodies_ = bodies.size();
    } else {
        num_bodies_ = 0;
    }
    
    // Broadcast number of bodies
    MPI_Bcast(&num_bodies_, 1, MPI_INT, 0, comm_);
    
    // Calculate which bodies this process is responsible for
    int bodies_per_process = num_bodies_ / size_;
    int remainder = num_bodies_ % size_;
    
    start_idx_ = rank_ * bodies_per_process + std::min(rank_, remainder);
    end_idx_ = start_idx_ + bodies_per_process + (rank_ < remainder ? 1 : 0);
    local_num_ = end_idx_ - start_idx_;
    
    // Initialize local bodies
    if (rank_ == 0) {
        local_bodies_.assign(bodies.begin() + start_idx_, bodies.begin() + end_idx_);
        
        // Send bodies to other processes
        for (int proc = 1; proc < size_; ++proc) {
            int proc_start = proc * bodies_per_process + std::min(proc, remainder);
            int proc_end = proc_start + bodies_per_process + (proc < remainder ? 1 : 0);
            
            // Send number of bodies
            int proc_num = proc_end - proc_start;
            MPI_Send(&proc_num, 1, MPI_INT, proc, 0, comm_);
            
            // Send each body
            for (int i = proc_start; i < proc_end; ++i) {
                double body_data[8]; // id, mass, pos[3], vel[3]
                body_data[0] = static_cast<double>(bodies[i].id);
                body_data[1] = bodies[i].mass;
                body_data[2] = bodies[i].position[0];
                body_data[3] = bodies[i].position[1];
                body_data[4] = bodies[i].position[2];
                body_data[5] = bodies[i].velocity[0];
                body_data[6] = bodies[i].velocity[1];
                body_data[7] = bodies[i].velocity[2];
                MPI_Send(body_data, 8, MPI_DOUBLE, proc, 0, comm_);
            }
        }
    } else {
        // Receive bodies
        int proc_num;
        MPI_Recv(&proc_num, 1, MPI_INT, 0, 0, comm_, MPI_STATUS_IGNORE);
        local_bodies_.resize(proc_num);
        
        for (int i = 0; i < proc_num; ++i) {
            double body_data[8];
            MPI_Recv(body_data, 8, MPI_DOUBLE, 0, 0, comm_, MPI_STATUS_IGNORE);
            
            local_bodies_[i].id = static_cast<int>(body_data[0]);
            local_bodies_[i].mass = body_data[1];
            local_bodies_[i].position[0] = body_data[2];
            local_bodies_[i].position[1] = body_data[3];
            local_bodies_[i].position[2] = body_data[4];
            local_bodies_[i].velocity[0] = body_data[5];
            local_bodies_[i].velocity[1] = body_data[6];
            local_bodies_[i].velocity[2] = body_data[7];
        }
    }
    
    time_elapsed_ = 0.0;
    step_count_ = 0;
}

DistributedNBodySimulation::~DistributedNBodySimulation() {
    // MPI is typically finalized elsewhere, so we don't finalize here
}

std::pair<std::vector<std::array<double, 3>>, std::vector<double>> 
DistributedNBodySimulation::gatherAllPositionsMasses() {
    // Prepare local data
    std::vector<double> local_positions_flat(local_num_ * 3);
    std::vector<double> local_masses(local_num_);
    
    for (size_t i = 0; i < local_num_; ++i) {
        local_masses[i] = local_bodies_[i].mass;
        for (int j = 0; j < 3; ++j) {
            local_positions_flat[i * 3 + j] = local_bodies_[i].position[j];
        }
    }
    
    // Calculate sizes for each process
    int local_pos_size = static_cast<int>(local_num_ * 3);
    int local_mass_size = static_cast<int>(local_num_);
    
    std::vector<int> pos_sizes(size_);
    std::vector<int> mass_sizes(size_);
    
    MPI_Allgather(&local_pos_size, 1, MPI_INT, pos_sizes.data(), 1, MPI_INT, comm_);
    MPI_Allgather(&local_mass_size, 1, MPI_INT, mass_sizes.data(), 1, MPI_INT, comm_);
    
    // Calculate displacements
    std::vector<int> pos_displacements(size_);
    std::vector<int> mass_displacements(size_);
    pos_displacements[0] = 0;
    mass_displacements[0] = 0;
    for (int i = 1; i < size_; ++i) {
        pos_displacements[i] = pos_displacements[i-1] + pos_sizes[i-1];
        mass_displacements[i] = mass_displacements[i-1] + mass_sizes[i-1];
    }
    
    // Prepare receive buffers
    std::vector<double> all_positions_flat(num_bodies_ * 3);
    std::vector<double> all_masses(num_bodies_);
    
    // Gather all positions
    MPI_Allgatherv(
        local_positions_flat.data(), local_pos_size, MPI_DOUBLE,
        all_positions_flat.data(), pos_sizes.data(), pos_displacements.data(), MPI_DOUBLE,
        comm_
    );
    
    // Gather all masses
    MPI_Allgatherv(
        local_masses.data(), local_mass_size, MPI_DOUBLE,
        all_masses.data(), mass_sizes.data(), mass_displacements.data(), MPI_DOUBLE,
        comm_
    );
    
    // Reshape positions
    std::vector<std::array<double, 3>> all_positions(num_bodies_);
    for (int i = 0; i < num_bodies_; ++i) {
        for (int j = 0; j < 3; ++j) {
            all_positions[i][j] = all_positions_flat[i * 3 + j];
        }
    }
    
    return {all_positions, all_masses};
}

std::vector<std::array<double, 3>> 
DistributedNBodySimulation::calculateLocalForces(
    const std::vector<std::array<double, 3>>& all_positions,
    const std::vector<double>& all_masses) {
    
    std::vector<std::array<double, 3>> local_forces;
    local_forces.reserve(local_num_);
    
    for (const auto& local_body : local_bodies_) {
        std::array<double, 3> force = {0.0, 0.0, 0.0};
        
        for (int j = 0; j < num_bodies_; ++j) {
            if (local_body.id != j) {
                // Create temporary body for force calculation
                Body temp_body(all_positions[j], {0, 0, 0}, all_masses[j], j);
                std::array<double, 3> f = calculateForce(local_body, temp_body);
                for (int k = 0; k < 3; ++k) {
                    force[k] += f[k];
                }
            }
        }
        
        local_forces.push_back(force);
    }
    
    return local_forces;
}

void DistributedNBodySimulation::step(double dt) {
    // Velocity Verlet integration (energy-conserving)
    
    // Gather all positions and masses
    auto [all_positions, all_masses] = gatherAllPositionsMasses();
    
    // Calculate forces for local bodies at current positions
    std::vector<std::array<double, 3>> local_forces = calculateLocalForces(all_positions, all_masses);
    
    // Update positions: x(t+dt) = x(t) + dt * v(t) + 0.5 * dt^2 * a(t)
    for (size_t i = 0; i < local_bodies_.size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            double acceleration = local_forces[i][j] / local_bodies_[i].mass;
            local_bodies_[i].position[j] += local_bodies_[i].velocity[j] * dt + 0.5 * acceleration * dt * dt;
        }
    }
    
    // Gather updated positions and calculate new forces
    auto [new_positions, new_masses] = gatherAllPositionsMasses();
    std::vector<std::array<double, 3>> new_forces = calculateLocalForces(new_positions, new_masses);
    
    // Update velocities: v(t+dt) = v(t) + 0.5 * dt * (a(t) + a(t+dt))
    for (size_t i = 0; i < local_bodies_.size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            double accel_old = local_forces[i][j] / local_bodies_[i].mass;
            double accel_new = new_forces[i][j] / local_bodies_[i].mass;
            local_bodies_[i].velocity[j] += 0.5 * dt * (accel_old + accel_new);
        }
    }
    
    time_elapsed_ += dt;
    step_count_++;
}

void DistributedNBodySimulation::run(int num_steps, double dt, bool verbose) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (int step = 0; step < num_steps; ++step) {
        step(dt);
        
        if (verbose && rank_ == 0 && (step + 1) % std::max(1, num_steps / 10) == 0) {
            auto [kinetic, potential, total] = getEnergy();
            std::cout << "Step " << (step + 1) << "/" << num_steps 
                      << " | Time: " << std::scientific << std::setprecision(2) << time_elapsed_
                      << "s | Energy: " << total << " J | Processes: " << size_ << std::endl;
        }
    }
    
    if (verbose && rank_ == 0) {
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start_time).count();
        std::cout << "\nSimulation completed in " << elapsed_time << " ms" << std::endl;
        std::cout << "Average time per step: " 
                  << static_cast<double>(elapsed_time) / num_steps << " ms" << std::endl;
        std::cout << "Using " << size_ << " MPI processes" << std::endl;
    }
}

std::tuple<double, double, double> DistributedNBodySimulation::getEnergy() {
    if (rank_ != 0) {
        gatherAllBodies();  // Participate in gather
        return {0.0, 0.0, 0.0};
    }
    
    std::vector<Body> all_bodies = gatherAllBodies();
    auto [kinetic, potential] = calculateEnergy(all_bodies);
    return {kinetic, potential, kinetic + potential};
}

std::vector<Body> DistributedNBodySimulation::gatherAllBodies() {
    if (rank_ == 0) {
        std::vector<Body> all_bodies = local_bodies_;
        
        // Receive bodies from other processes
        for (int proc = 1; proc < size_; ++proc) {
            int proc_num;
            MPI_Recv(&proc_num, 1, MPI_INT, proc, 0, comm_, MPI_STATUS_IGNORE);
            
            for (int i = 0; i < proc_num; ++i) {
                double body_data[8];
                MPI_Recv(body_data, 8, MPI_DOUBLE, proc, 0, comm_, MPI_STATUS_IGNORE);
                
                Body body(
                    {body_data[2], body_data[3], body_data[4]},
                    {body_data[5], body_data[6], body_data[7]},
                    body_data[1],
                    static_cast<int>(body_data[0])
                );
                all_bodies.push_back(body);
            }
        }
        
        // Sort by ID to maintain order
        std::sort(all_bodies.begin(), all_bodies.end(),
                  [](const Body& a, const Body& b) { return a.id < b.id; });
        
        return all_bodies;
    } else {
        // Send local bodies
        int proc_num = static_cast<int>(local_num_);
        MPI_Send(&proc_num, 1, MPI_INT, 0, 0, comm_);
        
        for (const auto& body : local_bodies_) {
            double body_data[8];
            body_data[0] = static_cast<double>(body.id);
            body_data[1] = body.mass;
            body_data[2] = body.position[0];
            body_data[3] = body.position[1];
            body_data[4] = body.position[2];
            body_data[5] = body.velocity[0];
            body_data[6] = body.velocity[1];
            body_data[7] = body.velocity[2];
            MPI_Send(body_data, 8, MPI_DOUBLE, 0, 0, comm_);
        }
        
        return {};
    }
}

DistributedNBodySimulation::Statistics DistributedNBodySimulation::getStatistics() {
    if (rank_ != 0) {
        return {};
    }
    
    std::vector<Body> all_bodies = gatherAllBodies();
    auto [kinetic, potential, total] = getEnergy();
    
    // Calculate center of mass
    double total_mass = 0.0;
    std::array<double, 3> com = {0.0, 0.0, 0.0};
    
    for (const auto& body : all_bodies) {
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
        size_,
        time_elapsed_,
        step_count_,
        kinetic,
        potential,
        total,
        com
    };
}

std::vector<Body> DistributedNBodySimulation::getAllBodies() {
    if (rank_ == 0) {
        return gatherAllBodies();
    } else {
        gatherAllBodies();  // Participate in gather
        return {};
    }
}

