#ifndef DATABASE_ROBOT_ADAPTER_HPP
#define DATABASE_ROBOT_ADAPTER_HPP

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/document/value.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <string>

class RobotAdapter {
public:
    // Constructor to initialize the collection
    RobotAdapter(mongocxx::collection collection) : collection_(std::move(collection)) {}

    // Insert a new robot document
    void insertRobot(const std::string& id, const std::string& name, const std::string& size, const std::string& type,
                    const std::string& baseLocation, const std::string& currentLocation, const std::string& status,
                    const std::string& capacity, const std::vector<int>& taskQueue, const std::vector<int>& path, const int& curBattery,
                    const int& totalBatteryUsed, const int& errorCount, const int& roomsCleaned);

    // Find a document by robot ID
    std::optional<bsoncxx::document::value> findDocumentById(const std::string& robotId);

    bool updateRobot(const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& curBattery, const int& totalBatteryUsed);
    bool updateRobotErrorCount(const std::string& id);
    bool updateRobotRoomsCleaned(const std::string& id);
    
    bool deleteRobot(const std::string& robotId);
    
    std::vector<bsoncxx::document::value> getAllRobots();
    
private:
    mongocxx::collection collection_;
};

#endif