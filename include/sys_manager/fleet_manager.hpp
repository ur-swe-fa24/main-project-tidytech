#ifndef SYS_MANAGER_FLEETMANAGER_HPP
#define SYS_MANAGER_FLEETMANAGER_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <wx/wx.h>

#include "../simulation/simulator.hpp"
#include "../database/database.hpp"
#include "pubsub/publisher.hpp"
#include "pubsub/subscriber.hpp"
#include "types/types.hpp"

using namespace types;

class FleetManager : public Subscriber, public wxApp {
    public:
        // Constructor
        FleetManager();

        // I/O methods
        void read_ui_input(std::string filepath);
        void write_output(std::string filepath, std::string message); // outputs to a file

        // Observer pattern methods
        void subscribe(const std::string& event);
        void unsubscribe(const std::string& event);
        void update(const std::string& event, const std::string& data) override;

        // Run simulation methods
        void start_sim() {simulator_.start_simulation();};

        bool OnInit() override;
    private:
        // Methods to handle the different events
        void handle_five_sec_ping(const std::string& data);
        void handle_finished_ping(const std::string& data);
        
        // Attributes for simulator, and database
        Simulator simulator_;
        Database database_;
};

#endif
