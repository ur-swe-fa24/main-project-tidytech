#include "ui/user_interface.hpp"
#include "ui/add_robot_window.hpp"
#include "ui/add_floor_window.hpp"
#include <wx/wx.h>

enum IDs {
    BUTTON_ID = 2
};

wxBEGIN_EVENT_TABLE(UserInterface, wxFrame)
    EVT_BUTTON(BUTTON_ID, UserInterface::OnStartSimulation)
wxEND_EVENT_TABLE()

UserInterface::UserInterface(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    subscribe("display_text");
    Bind(wxEVT_COMMAND_TEXT_UPDATED, &UserInterface::OnTextUpdated, this, GetId());
    Bind(wxEVT_CLOSE_WINDOW, &UserInterface::OnClose, this);

    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    display_text_ = new wxStaticText(panel, wxID_ANY, "Final report will go here.",
                                     wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    display_text_->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    mainSizer->Add(display_text_, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btn = new wxButton(panel, BUTTON_ID, "Start Simulation", wxDefaultPosition, wxSize(150, 45));
    buttonSizer->Add(btn, 0, wxALL, 5);

    wxButton* openAddRobot = new wxButton(panel, wxID_ANY, "Add Robot", wxDefaultPosition, wxSize(150, 45));
    openAddRobot->Bind(wxEVT_BUTTON, &UserInterface::OnAddRobot, this);
    buttonSizer->Add(openAddRobot, 0, wxALL, 5);

    wxButton* openAddFloor = new wxButton(panel, wxID_ANY, "Add Floor", wxDefaultPosition, wxSize(150, 45));
    openAddFloor->Bind(wxEVT_BUTTON, &UserInterface::OnAddFloor, this);
    buttonSizer->Add(openAddFloor, 0, wxALL, 5);

    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    panel->SetSizer(mainSizer);

    mainSizer->SetSizeHints(this);

    CreateStatusBar();
}


void UserInterface::OnStartSimulation(wxCommandEvent& evt) {
    fm_.start_sim();
}

void UserInterface::OnAddRobot(wxCommandEvent& event) {
    // Show the form dialog when the button is clicked
    AddRobotWindow robotForm(this);
    if (robotForm.ShowModal() == wxID_OK) {
        fm_.add_robot(robotForm.get_size(), robotForm.get_type(), robotForm.get_charging_position(), robotForm.get_current_position());
        wxMessageBox(wxT(""), wxT("Robot Added Successfully"), wxICON_INFORMATION);
    }
}

void UserInterface::OnAddFloor(wxCommandEvent& event) {
    // Show the form dialog when the button is clicked
    AddFloorWindow floorForm(this);
    if (floorForm.ShowModal() == wxID_OK) {
        fm_.add_floor(floorForm.get_floor_name());
        wxMessageBox(wxT(""), wxT("Floor Added Successfully"), wxICON_INFORMATION);
    }
}

void UserInterface::setText(const std::string& new_text) {
    if (display_text_) { 
        // Create a custom event
        wxCommandEvent* event = new wxCommandEvent(wxEVT_COMMAND_TEXT_UPDATED, GetId());
        event->SetString(new_text);
        
        // Queue the event to be processed on the main thread
        wxPostEvent(display_text_, *event);
    } else {
        wxLogError("error");
    }
}

void UserInterface::OnTextUpdated(wxCommandEvent& event) {
    display_text_->SetLabel(event.GetString());
    display_text_->GetParent()->Fit();
    Layout();
}

void UserInterface::subscribe(Subscriber* subscriber, const std::string& event) {
    subscribers_[event].push_back(subscriber);
}

// Let the subscriber unsubscribe from an event
void UserInterface::unsubscribe(Subscriber* subscriber, const std::string& event) {
    auto& subs = subscribers_[event];
    subs.erase(std::remove(subs.begin(), subs.end(), subscriber), subs.end());
}

// Notify all the subscribers
void UserInterface::notify(const std::string& event, const std::string& data) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, data);
    }
}

void UserInterface::subscribe(const std::string& event) {
    // subscribe to an event
    fm_.subscribe(this, event);
}

void UserInterface::unsubscribe(const std::string& event) {
    // unsubscribe from an event
    fm_.unsubscribe(this, event);
}

void UserInterface::update(const std::string& event, const std::string& data) {
    // Do a particular method depending on what type of event is being updated
    if (event == "display_text") {
        handle_display_text(data);
    }
}

void UserInterface::handle_display_text(const std::string& data) {
    setText(data);
}

void UserInterface::OnClose(wxCloseEvent& event) {
    std::cout << "closed" << std::endl;
    event.Skip();
}
