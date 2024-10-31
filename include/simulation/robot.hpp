#ifndef SIMULATION_ROBOT_HPP
#define SIMULATION_ROBOT_HPP

#include <string>
#include <vector>

class Robot {
    public:
        enum class Size {Small, Medium, Large};
        enum class Type {Scrubber, Vaccum, Shampoo};
        enum class Status {Available, Cleaning, Charging, Unavailable}; // You can task a robot if it is Available or Cleaning; but you cannot task it if it is unavailable

        Robot(std::string id, std::string size, std::string type, std::string base, std::string curr); // Constructor
        ~Robot() {}; // Destructor

        void set_size(const std::string size);
        void set_type(const std::string type);
        void set_status(const std::string status);

        std::string get_id() const {return id_;};
        std::string get_curr() const {return curr_;};
        Robot::Status get_status() const {return status_;};
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
        std::string id_;
        Robot::Size size_; 
        Robot::Type type_;
        std::string base_;
        std::string curr_;
        int battery_;
        std::vector<std::string> task_queue_;
        Robot::Status status_;

        
        
};



#endif