#ifndef DATABASE_DATABASE_HPP
#define DATABASE_DATABASE_HPP

#include <unordered_map>
#include <vector>
#include <string>

class Database {
    public:
        Database() {};
        void add_robot(std::string id, std::string type, std::string available, std::string location);
        void add_floor(std::string id, std::string name, std::string type);
        void add_task(std::string id, std::string robot_assigned, std::string room_assigned, std::string status);
    private:
        std::unordered_map<std::string, std::vector<std::string>> robots_;
        std::unordered_map<std::string, std::vector<std::string>> floors_;
        std::unordered_map<std::string, std::vector<std::string>> tasks_;
};

#endif