#ifndef SIMULATION_SIMULATOR_HPP
#define SIMULATION_SIMULATOR_HPP

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <unordered_map>

#include "simulation/robot.hpp"
#include "simulation/floor.hpp"
#include "simulation/floorplan.hpp"
#include "pubsub/publisher.hpp"
#include "pubsub/subscriber.hpp"
#include "types/types.hpp"
#include "spdlog/spdlog.h"

using namespace types;

class Simulator : public Publisher {
    public:
        static const int MAX_SIM_TIME = 10; //Max simulation time
        static const int MAX_NUM_FLOORS = 11; // Max num of floors in a floorplan

        Simulator(); // Default constructor
        Simulator& operator=(const Simulator& other);
        ~Simulator(); // Destructor

        int get_clock() const {return clock_;}; 

        void start_simulation(); // Start the sim_thread_
        void reset_simulation(); // Reset the simulation

        void add_floor(int id, std::string name, FloorRoomType room, FloorType floortype, FloorSize size, FloorInteraction interaction_level, bool restriction, int clean_level, std::vector<int> neighbors);
        void add_robot(int id, std::string name, RobotSize size, RobotType type, int base, int curr, RobotStatus status);
        std::string status_report(int robot_id);
        void add_task(int robot_id, int floor_id);
        std::vector<std::string> get_all_floor_names();
        std::vector<std::string> get_all_robot_names();

        void subscribe(Subscriber* subscriber, const Event& event) override;
        void unsubscribe(Subscriber* subscriber, const Event& event) override;
        void notify(const Event& event, const std::string& data) override;
    private:
        FloorPlan floorplan_;
        mutable std::mutex floors_mutex; // Mutex to protect floors
        std::vector<Robot> robots_;
        mutable std::mutex robots_mutex_; // Mutex to protect robots_
        int clock_;
        std::thread sim_thread_;  // Thread to start the clock when the simulator object is created
        std::atomic<bool> ticking_;  // Atomic flag to control the clock since it prevents other threads from interfering 

        void simulate(); // Start the ticking thread
        void simulate_robots(); // Adjust the robots for simulate
        void simulate_floors(); // Adjust the flooors for simulate
        bool can_move(Robot& robot); // Check whether if the robot can move or not
        std::unordered_map<Event, std::vector<Subscriber*>> subscribers_;


};

#endif