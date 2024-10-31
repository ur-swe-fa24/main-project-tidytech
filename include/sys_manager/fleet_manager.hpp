#ifndef SYS_MANAGER_FLEETMANAGER_HPP
#define SYS_MANAGER_FLEETMANAGER_HPP

#include <unordered_map>
#include <vector>
#include <string>

#include "../simulation/simulator.hpp"
#include "../database/database.hpp"
#include "pubsub/publisher.hpp"
#include "pubsub/subscriber.hpp"

class FleetManager : public Subscriber {
    public:
        // Constructor
        FleetManager(Database &db);

        // I/O methods
        void read_ui_input(std::string filepath);
        void write_output(std::string filepath, std::string message); // outputs to a file

        // Observer pattern methods
        void subscribe(const std::string& event);
        void unsubscribe(const std::string& event);
        void update(const std::string& event, const std::string& data) override;

        // Run simulation methods
        void start_sim() {simulator_.start_simulation();};
    private:
        // Methods to handle the different events
        void handle_five_sec_ping(const std::string& data);
        void handle_finished_ping(const std::string& data);
        
        // Attributes for simulator, and database
        Simulator simulator_;
        Database database_;
};

#endif
