#ifndef SIMULATION_SIMULATOR_HPP
#define SIMULATION_SIMULATOR_HPP

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

#include "simulation/robot.hpp"
#include "sys_manager/fleet_manager.hpp"

class Simulator {
    public:
        static const int MAX_SIM_TIME = 100; //Max simulation time

        Simulator(Fleet_manager* fm); // Default constructor
        ~Simulator(); // Destructor

        void start_simulation(); // Start the sim_thread_
        void reset_simulation(); // Reset the simulation

        void add_floor(std::string floor);
        void add_robot(std::string id, std::string size, std::string type, std::string base, std::string curr);
        std::string status_report(std::string robot_id);
        std::string clean(std::string robot_id, std::string floor_id);
    private:
        Fleet_manager* fm_;
        std::vector<std::string> floors_;
        std::vector<Robot> robots_;
        std::mutex robots_mutex_; // Mutex to protect robots_
        int clock_;
        std::thread sim_thread_;  // Thread to start the clock when the simulator object is created
        std::atomic<bool> ticking_;  // Atomic flag to control the clock since it prevents other threads from interfering 

        void simulate(); // Start the ticking thread

};

#endif