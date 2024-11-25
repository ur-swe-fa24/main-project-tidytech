#ifndef UI_USER_INTERFACE
#define UI_USER_INTERFACE

#include <wx/wx.h>
#include <wx/grid.h>
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

        void subscribe(const Event& event);
        void unsubscribe(const Event& event);
        void update(const Event& event, const std::string& data) override;
    private:
        void OnStartSimulation(wxCommandEvent& evt);
        void OnAddRobot(wxCommandEvent& event);
        // void OnRobotAdded(wxCommandEvent& event); 
        void OnAddFloor(wxCommandEvent& event);
        void OnAddTask(wxCommandEvent& event);
        void OnUpdateGrid(wxCommandEvent& evt);
        void handle_display_text(const std::string& data);
        void handle_five_sec(const std::string& data);
        void Logout(wxCommandEvent& evt);

        void AddRowToGrid(std::vector<std::string> row_info);
        void AddRowToGridFloor(std::vector<std::string> row_info);

        void update_grid(const std::vector<std::vector<std::string>>& new_data);


        std::vector<std::vector<std::string>> extract_five_ping(std::string str);
        // wxDECLARE_EVENT_TABLE();
        FleetManager fm_;
        wxStaticText* display_text_;
        int num_added_ = 0;
        wxGrid* grid;
        wxGrid* grid2;

        std::unordered_map<Event, std::vector<Subscriber*>> subscribers_;
};

#endif