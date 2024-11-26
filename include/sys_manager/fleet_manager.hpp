#ifndef SYS_MANAGER_FLEETMANAGER_HPP
#define SYS_MANAGER_FLEETMANAGER_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <wx/wx.h>

#include "simulation/simulator.hpp"
#include "database/db_manager.hpp"
#include "database/robot_adapter.hpp"
#include "database/floor_adapter.hpp"
#include "database/error_adapter.hpp"
#include "pubsub/publisher.hpp"
#include "pubsub/subscriber.hpp"
#include "types/types.hpp"

using namespace types;

class FleetManager : public Subscriber, public wxApp, public Publisher {
    public:
        // Constructor
        FleetManager();

        int add_robot(std::string name, std::string size, std::string type, std::string charging_position, std::string current_position, int capacity, std::vector<int> task_queue, std::vector<int> path, int total_battery_used, int error_count, int rooms_cleaned);
        int add_floor(std::string name, std::string roomType, std::string type, std::string size, std::string interaction, std::vector<int> neighbors);
        bool add_task_to_front(int robot_id, std::vector<int> floor_ids);
        bool add_task_to_back(int robot_id, std::vector<int> floor_ids);
        void write_output(std::string filepath, std::string message); // outputs to a file

        unordered_map<std::string, std::vector<std::string>> get_table_data();
        unordered_map<std::string, std::vector<std::string>> get_table_data_floors();

        std::vector<std::string> get_all_floor_names();
        std::vector<std::string> get_all_robot_names();

        // Observer pattern methods
        void subscribe(const Event& event);
        void unsubscribe(const Event& event);
        void update(const Event& event, const std::string& data) override;
        void update(const types::Event& event, const int id) override;
        void update(const types::Event& event, const int id, const ErrorType error_type, const bool resolved) override;
        void update(const Event& event, const int id, const std::vector<int>& data) override;
        void update(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) override;

        void subscribe(Subscriber* subscriber, const Event& event) override;
        void unsubscribe(Subscriber* subscriber, const Event& event) override;
        void notify(const Event& event, const std::string& data) override;
        void notify(const types::Event& event, const int id) override;
        void notify(const types::Event& event, const int id, const ErrorType error_type, const bool resolved) override;
        void notify(const Event& event, const int id, const std::vector<int>& data) override;
        void notify(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) override;

        // Run simulation methods
        void start_sim() {simulator_.start_simulation();};

        bool OnInit() override;
    private:
        // Methods to handle the different events
        void handle_five_sec_ping(const std::string& data);
        void handle_finished_ping(const std::string& data);
        void handle_five_sec_floors(const std::string& data);
        void update_neighbors_db(const int id, const std::vector<int>& data);
        void update_robot_db(const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed);
        void update_db_num_floors_clean(const int id);
        void update_db_robot_error(const int robotid, const ErrorType error_type, const bool resolved);

        std::unordered_map<Event, std::vector<Subscriber*>> subscribers_;
        
        // Attributes for simulator, and database
        Simulator simulator_;
        DBManager& dbmanager_;
        RobotAdapter robot_adapter_;
        FloorAdapter floor_adapter_;
        ErrorAdapter error_adapter_;
        int robot_id_count;
        int floor_id_count;
};

#endif
