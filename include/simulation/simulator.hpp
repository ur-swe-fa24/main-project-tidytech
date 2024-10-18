#ifndef SIMULATION_SIMULATOR_HPP
#define SIMULATION_SIMULATOR_HPP

#include <string>
#include <vector>

class Simulator {
    public:
        Simulator() {};
        std::string clean(std::string robot, std::string room);
        void add_room(std::string room);
        void add_robot(std::string robot);
    private:
        std::vector<std::string> rooms_;
        std::vector<std::string> robots_;
};

#endif