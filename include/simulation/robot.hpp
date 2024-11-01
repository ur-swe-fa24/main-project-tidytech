#ifndef SIMULATION_ROBOT_HPP
#define SIMULATION_ROBOT_HPP

#include <string>
#include <vector>
#include "types/types.hpp"

using namespace types;

class Robot {
    public:
        static int robotCreationCount; // For id

        Robot(RobotSize size, RobotType type, std::string base, std::string curr); // Constructor
        ~Robot() {}; // Destructor

        void set_size(const RobotSize size) {size_ = size;};
        void set_type(const RobotType type) {type_ = type;};
        void set_status(const RobotStatus status) {status_ = status;};

        int get_id() const {return id_;};
        std::string get_curr() const {return curr_;};
        RobotStatus get_status() const {return status_;};
        int get_battery() const {return battery_;};
        std::string to_string() const;

        void add_tasks_to_back(std::vector<std::string> floors);
        void add_tasks_to_front(std::vector<std::string> floors);
        bool move_to_next();
        bool can_move();
        void start_task();
        bool at_base() {return base_ == curr_;}; // Check if robot is at base
        bool tasks_empty() {return task_queue_.empty();}; // Check if task is empty
        void go_charge();
        void charge();
        void consume_power(int amount = 1) {battery_ = std::max(0, battery_-amount);}; // One tick of battery consume for every second
        

    private:
        int id_;
        RobotSize size_; 
        RobotType type_;
        std::string base_;
        std::string curr_;
        int battery_;
        std::vector<std::string> task_queue_;
        RobotStatus status_;

        
        
};



#endif