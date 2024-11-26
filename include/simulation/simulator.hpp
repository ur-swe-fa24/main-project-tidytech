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
        bool sim_in_progress() {return ticking_;};
        void simulate(); // Start the ticking thread
        void simulate_robots(); // Adjust the robots for simulate
        void simulate_floors(); // Adjust the flooors for simulate

        Robot get_robot(int robot_id);
        Floor get_floor(int floor_id);
        std::vector<Floor> get_neighbors(int floor_id);
        int get_num_robots() const {return robots_.size();};
        int get_num_floors() const {return floorplan_.get_all_floor().size();};

        void add_floor(int id, std::string name, FloorRoomType room, FloorType floortype, FloorSize size, FloorInteraction interaction_level, bool restriction, int clean_level, std::vector<int> neighbors);
        void add_robot(int id, std::string name, RobotSize size, RobotType type, int base, int curr, RobotStatus status, int remaining_capacity, std::vector<int> task_queue, std::vector<int> path, int current_battery, int total_battery_used, int error_count, int rooms_cleaned);
        std::string status_report(int robot_id);
        void add_task_to_front(int robot_id, std::vector<int> floor_ids);
        void add_task_to_back(int robot_id, std::vector<int> floor_ids);
        std::vector<std::string> get_all_floor_names();
        std::vector<std::string> get_all_robot_names();

        void subscribe(Subscriber* subscriber, const Event& event) override;
        void unsubscribe(Subscriber* subscriber, const Event& event) override;
        void notify(const Event& event, const std::string& data) override;
        void notify(const types::Event& event, const int id) override;
        void notify(const types::Event& event, const int id, const int val) override;
        void notify(const types::Event& event, const int id, const ErrorType error_type, const bool resolved) override;
        void notify(const Event& event, const int id, const std::vector<int>& data) override;
        void notify(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) override;

    private:
        FloorPlan floorplan_;
        mutable std::mutex floors_mutex; // Mutex to protect floors
        std::vector<Robot> robots_;
        mutable std::mutex robots_mutex_; // Mutex to protect robots_
        int clock_;
        std::thread sim_thread_;  // Thread to start the clock when the simulator object is created
        std::atomic<bool> ticking_;  // Atomic flag to control the clock since it prevents other threads from interfering 

        bool can_move(Robot& robot); // Check whether if the robot can move or not
        bool check_compatibility(RobotType robot_type, std::vector<int> floor_ids); // Check if the robot can be task with the room
        bool check_robot_to_floor(RobotType robot_type, FloorType floor_type);
        void check_out_of_battery(int id, int battery);
        void update_robot_db(Robot& robot, int powerUsed);
        std::unordered_map<Event, std::vector<Subscriber*>> subscribers_;


};

#endif