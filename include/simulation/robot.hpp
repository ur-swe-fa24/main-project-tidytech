#ifndef SIMULATION_ROBOT_HPP
#define SIMULATION_ROBOT_HPP

#include <string>
#include <vector>
#include <queue>
#include "types/types.hpp"
#include "pubsub/publisher.hpp"
#include "pubsub/subscriber.hpp"

using namespace types;

class Robot{
    public:
        Robot(int id, std::string name, RobotSize size, RobotType type, int base, int curr, RobotStatus status); // Constructor
        ~Robot() {}; // Destructor

        bool operator==(const Robot& other) const {return id_ == other.id_;} // Overriding Robot comparison

        void set_size(const RobotSize size) {size_ = size;};
        void set_type(const RobotType type) {type_ = type;};
        void set_status(const RobotStatus status) {status_ = status;};
        void set_curr_path(const std::queue<int> path) {curr_path_ = path;};

        int get_id() const {return id_;};
        std::string get_name() const {return name_;};
        RobotSize get_size() const {return size_;};
        int get_curr() const {return curr_;};
        int get_base() const {return base_;};
        int get_remaining_capacity() const {return remaining_capacity_;};
        RobotStatus get_status() const {return status_;};
        int get_battery() const {return battery_;};
        std::vector<int> get_task_queue() const {return task_queue_;};
        int get_task_size() const {return task_queue_.size();};
        int get_curr_path_size() const {return curr_path_.size();};
        std::string to_string() const;

        void add_tasks_to_back(std::vector<int> floors);
        void add_tasks_to_front(std::vector<int> floors);
        int get_first_task() {return task_queue_.front();};
        void update_curr_path(const std::queue<int> path) {curr_path_ = path;};
        void remove_curr_path() {curr_path_ = std::queue<int>();}; // Reset the path
        void move_to_next_task();
        void move_to_next_floor();
        void start_task();
        bool at_base() {return base_ == curr_;}; // Check if robot is at base
        bool tasks_empty() {return task_queue_.empty();}; // Check if task is empty
        void go_charge();
        void go_empty();
        void charge();
        void clean() {remaining_capacity_ = max(0, remaining_capacity_-3);};
        bool is_capacity_empty();
        void consume_power(int amount = 1);
        void fix_error() {status_ = RobotStatus::Available; battery_ = 100;}; // Error fix
        void reset_capacity() {status_ = RobotStatus::Available; remaining_capacity_ = 100;};
        void break_robot();
        

    private:
        int id_;
        std::string name_;
        RobotSize size_; 
        RobotType type_;
        int base_;
        int curr_;
        int battery_;
        int remaining_capacity_;
        std::vector<int> task_queue_;
        RobotStatus status_;
        std::queue<int> curr_path_; // Shortest path from one floor to another
        
};



#endif