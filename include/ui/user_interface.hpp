#ifndef UI_USER_INTERFACE
#define UI_USER_INTERFACE

#include <wx/wx.h>
#include "../simulation/simulator.hpp"
#include "../sys_manager/fleet_manager.hpp"
#include "../types/types.hpp"
#include "pubsub/publisher.hpp"
#include "pubsub/subscriber.hpp"

using namespace types;
 
class UserInterface : public wxFrame, public Publisher, public Subscriber {
    public:
        UserInterface(const wxString& title);

        void subscribe(Subscriber* subscriber, const Event& event) override;
        void unsubscribe(Subscriber* subscriber, const Event& event) override;
        void notify(const Event& event, const std::string& data) override;
        void notify(const types::Event& event, const int id) override;
        void notify(const types::Event& event, const int id, const ErrorType error_type, const bool resolved);
        void notify(const Event& event, const int id, const std::vector<int>& data) override;
        void notify(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) override;

        void subscribe(const Event& event);
        void unsubscribe(const Event& event);
        void update(const Event& event, const std::string& data) override;
        void update(const types::Event& event, const int id) override;
        void update(const types::Event& event, const int id, const ErrorType error_type, const bool resolved) override;
        void update(const Event& event, const int id, const std::vector<int>& data) override;
        void update(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) override;
        
    private:
        void OnStartSimulation(wxCommandEvent& evt);
        void OnAddRobot(wxCommandEvent& event);
        // void OnRobotAdded(wxCommandEvent& event); 
        void OnAddFloor(wxCommandEvent& event);
        void OnAddTask(wxCommandEvent& event);
        void setText(const string& new_text);
        void handle_display_text(const std::string& data);
        void OnTextUpdated(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        wxDECLARE_EVENT_TABLE();
        FleetManager fm_;
        wxStaticText* display_text_;
        int num_added_ = 0;

        std::unordered_map<Event, std::vector<Subscriber*>> subscribers_;
};

#endif