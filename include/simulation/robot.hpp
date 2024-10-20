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
        ~Robot() {robotCount--;}; // Destructor

        void setSize(std::string size);
        void setType(std::string type);
        void setStatus(std::string status);

        std::string getId() {return id_;};
        std::string getCurr() {return curr_;};
        Robot::Status getStatus() {return status_;};
        int getBattery() {return battery_;};
        std::string toString();

        void addTasksToBack(std::vector<std::string> floors);
        void addTasksToFront(std::vector<std::string> floors);
        bool moveToNext();
        bool canMove();
        void startTask();
        bool atBase() {return base_ == curr_;};
        bool tasksEmpty() {return task_queue_.empty();};
        void goCharge();
        void charge();
        void consumePower(int amount = 1) {battery_ = std::max(0, battery_-amount);}; // One tick of battery consume for every second
        

    private:
        static int robotCount;

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