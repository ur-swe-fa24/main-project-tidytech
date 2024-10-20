#ifndef SYS_MANAGER_FLEET_MANAGER_HPP
#define SYS_MANAGER_FLEET_MANAGER_HPP

#include <unordered_map>
#include <vector>
#include <string>

#include "../simulation/simulator.hpp"
#include "../database/database.hpp"

class Fleet_manager {
    public:
        Fleet_manager(Database db);
        enum class UserType { SM, BM, BO, FE };
        vector<vector<string>> read_ui_input(std::string filepath);
        void retrive_status(std::string filepath, std::string robot_name, std::string room_name); // outputs to a file
    private:
        Simulator simulator_{};
        UserType user_type_;
        Database database_;
};

#endif
