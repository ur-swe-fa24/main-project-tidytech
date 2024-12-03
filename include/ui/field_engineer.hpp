#ifndef UI_FIELD_ENGINEER
#define UI_FIELD_ENGINEER

#include <wx/wx.h>
#include <wx/grid.h>
#include "sys_manager/fleet_manager.hpp"
#include "pubsub/subscriber.hpp"

 
class FieldEngineer : public wxFrame, public Subscriber {
    public:
        FieldEngineer(const wxString& title, FleetManager* fm);

        void subscribe(const Event& event);
        void unsubscribe(const Event& event);
        void update(const Event& event, const std::string& data) override;
        void update(const types::Event& event, const int id) override;
        void update(const types::Event& event, const int id, const int val) override;
        void update(const types::Event& event, const int id, const ErrorType error_type, const bool resolved) override;
        void update(const Event& event, const int id, const std::vector<int>& data) override;
        void update(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) override;

        std::vector<std::vector<std::string>> extract_five_ping(std::string input);
        void OnUpdateGrid(wxCommandEvent& evt);
        void update_grid(const std::vector<std::vector<std::string>>& robotData);
        void handle_five_sec(const std::string& data);
    private:
        FleetManager& fm_;
        wxGrid* grid;
        void Logout(wxCommandEvent& evt);
};

#endif