#ifndef SYS_MANAGER_FLEETMANAGER_HPP
#define SYS_MANAGER_FLEETMANAGER_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <wx/wx.h>

#include "../simulation/simulator.hpp"
#include "../database/database.hpp"
#include "../database/robot_adapter.hpp"
#include "pubsub/publisher.hpp"
#include "pubsub/subscriber.hpp"
#include "types/types.hpp"

using namespace types;

class FleetManager : public Subscriber, public wxApp, public Publisher {
    public:
        // Constructor
        FleetManager();
        static int robot_count; // For robot_id
        static int floor_count; // For floor_id

        void add_robot(std::string name, std::string size, std::string type, std::string charging_position, std::string current_position);
        void add_floor(std::string name);
        void write_output(std::string filepath, std::string message); // outputs to a file

        // Observer pattern methods
        void subscribe(const std::string& event);
        void unsubscribe(const std::string& event);
        void update(const std::string& event, const std::string& data) override;

        void subscribe(Subscriber* subscriber, const std::string& event) override;
        void unsubscribe(Subscriber* subscriber, const std::string& event) override;
        void notify(const std::string& event, const std::string& data) override;

        // Run simulation methods
        void start_sim() {simulator_.start_simulation();};

        bool OnInit() override;
    private:
        // Methods to handle the different events
        void handle_five_sec_ping(const std::string& data);
        void handle_finished_ping(const std::string& data);

        std::unordered_map<std::string, std::vector<Subscriber*>> subscribers_;
        
        // Attributes for simulator, and database
        Simulator simulator_;
        Database database_;
        RobotAdapter robot_adapter_;
};

#endif
