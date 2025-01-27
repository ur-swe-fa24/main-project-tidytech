#ifndef UI_BUILDING_OPERATOR
#define UI_BUILDING_OPERATOR

#include <wx/wx.h>
#include <wx/grid.h>
#include "../simulation/simulator.hpp"
#include "../sys_manager/fleet_manager.hpp"
#include "../types/types.hpp"
#include "pubsub/publisher.hpp"
#include "pubsub/subscriber.hpp"

using namespace types;
 
class BuildingOperator : public wxFrame, public Publisher, public Subscriber {
    public:
        BuildingOperator(const wxString& title, FleetManager* fm_);

        void subscribe(Subscriber* subscriber, const Event& event) override;
        void unsubscribe(Subscriber* subscriber, const Event& event) override;
        void notify(const Event& event, const std::string& data) override;
        void notify(const types::Event& event, const int id) override;
        void notify(const types::Event& event, const int id, const int val) override;
        void notify(const types::Event& event, const int id, const ErrorType error_type, const bool resolved);
        void notify(const Event& event, const int id, const std::vector<int>& data) override;
        void notify(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) override;

        void subscribe(const Event& event);
        void unsubscribe(const Event& event);
        void update(const Event& event, const std::string& data) override;
        void update(const types::Event& event, const int id) override;
        void update(const types::Event& event, const int id, const int val) override;
        void update(const types::Event& event, const int id, const ErrorType error_type, const bool resolved) override;
        void update(const Event& event, const int id, const std::vector<int>& data) override;
        void update(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) override;
        
    private:
        void OnClose(wxCloseEvent& event);
        void OnAddTask(wxCommandEvent& event);
        void OnUpdateGrid(wxCommandEvent& evt);
        // void handle_display_text(const std::string& data);

        void handle_five_sec(const std::string& data);
        void handle_five_sec_floors(const std::string& data);
        void Logout(wxCommandEvent& evt);

        void AddRowToGrid(std::vector<std::string> row_info);
        void AddRowToGridFloor(std::vector<std::string> row_info);

        void update_grid_floors(std::string cleanLevel);
        void OnUpdateGridFloors(wxCommandEvent& evt);

        void update_grid_neighbors();

        void update_grid(const std::vector<std::vector<std::string>>& new_data);

        void RefreshRobotTable();

        std::vector<std::vector<std::string>> extract_five_ping(std::string str);
        // wxDECLARE_EVENT_TABLE();
        FleetManager& fm_;
        wxStaticText* display_text_;
        int num_added_ = 0;
        wxGrid* grid;
        wxGrid* grid2;
        std::string selected_robot_;

        std::unordered_map<Event, std::vector<Subscriber*>> subscribers_;
};

#endif