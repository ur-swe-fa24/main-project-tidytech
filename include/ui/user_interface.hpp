#ifndef UI_USER_INTERFACE
#define UI_USER_INTERFACE

#include <wx/wx.h>
#include "../simulation/simulator.hpp"
#include "../database/database.hpp"
#include "../sys_manager/fleet_manager.hpp"
#include "pubsub/publisher.hpp"
#include "pubsub/subscriber.hpp"
 
class UserInterface : public wxFrame, public Publisher, public Subscriber {
    public:
        UserInterface(const wxString& title);

        void subscribe(Subscriber* subscriber, const std::string& event) override;
        void unsubscribe(Subscriber* subscriber, const std::string& event) override;
        void notify(const std::string& event, const std::string& data) override;

        void subscribe(const std::string& event);
        void unsubscribe(const std::string& event);
        void update(const std::string& event, const std::string& data) override;
    private:
        void OnButtonClicked(wxCommandEvent& evt);
        void setText(const string& new_text);
        void handle_display_text(const std::string& data);
        void OnTextUpdated(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        wxDECLARE_EVENT_TABLE();
        FleetManager fm_;
        wxStaticText* display_text_;

        std::unordered_map<std::string, std::vector<Subscriber*>> subscribers_;
};

#endif