#include "ui/user_interface.hpp"
#include "ui/add_robot_window.hpp"
#include "ui/add_floor_window.hpp"
#include "ui/add_task_window.hpp"
#include <wx/wx.h>

enum IDs {
    BUTTON_ID = 2
};

wxBEGIN_EVENT_TABLE(UserInterface, wxFrame)
    EVT_BUTTON(BUTTON_ID, UserInterface::OnStartSimulation)
wxEND_EVENT_TABLE()

UserInterface::UserInterface(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    // To be able to change the main text on the panel
    subscribe(Event::DisplayText);

    // Binding buttons to events (Potention TODO: change this to match the style of the others)
    Bind(wxEVT_COMMAND_TEXT_UPDATED, &UserInterface::OnTextUpdated, this, GetId());
    Bind(wxEVT_CLOSE_WINDOW, &UserInterface::OnClose, this);

    // Crate a panel for the whole window
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Create a text section and add it to the sizer
    display_text_ = new wxStaticText(panel, wxID_ANY, "Final report will go here.",
                                     wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    display_text_->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    mainSizer->Add(display_text_, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    // Create a sizer for the three buttons and add the buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* btn = new wxButton(panel, BUTTON_ID, "Start Simulation", wxDefaultPosition, wxSize(150, 45));
    buttonSizer->Add(btn, 0, wxALL, 5);

    wxButton* openAddRobot = new wxButton(panel, wxID_ANY, "Add Robot", wxDefaultPosition, wxSize(150, 45));
    openAddRobot->Bind(wxEVT_BUTTON, &UserInterface::OnAddRobot, this);
    buttonSizer->Add(openAddRobot, 0, wxALL, 5);

    wxButton* openAddFloor = new wxButton(panel, wxID_ANY, "Add Floor", wxDefaultPosition, wxSize(150, 45));
    openAddFloor->Bind(wxEVT_BUTTON, &UserInterface::OnAddFloor, this);
    buttonSizer->Add(openAddFloor, 0, wxALL, 5);

    wxButton* openAddTask = new wxButton(panel, wxID_ANY, "Add Task", wxDefaultPosition, wxSize(150, 45));
    openAddTask->Bind(wxEVT_BUTTON, &UserInterface::OnAddTask, this);
    buttonSizer->Add(openAddTask, 0, wxALL, 5);

    // Add the buttons to the main sizer
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    panel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);
    CreateStatusBar();
}


// Wrapper to just call the start sim from the fleet manager
void UserInterface::OnStartSimulation(wxCommandEvent& evt) {
    fm_.start_sim();
}

// Have a dialogue (form) for when you click the add robot
void UserInterface::OnAddRobot(wxCommandEvent& event) {
    std::vector<std::string> names = fm_.get_all_floor_names();
    AddRobotWindow robotForm(this, names);
    if (robotForm.ShowModal() == wxID_OK) {
        if (fm_.add_robot(robotForm.get_name(), robotForm.get_size(), robotForm.get_type(), robotForm.get_charging_position(), robotForm.get_charging_position(), 100, {}, {})) {
            wxMessageBox(wxT(""), wxT("Robot Added Successfully"), wxICON_INFORMATION);
        } else {
            wxMessageBox(wxT(""), wxT("Could Not Add Robot"), wxICON_INFORMATION);
        }
    }
}



// Have a dialogue (form) for when you click the add floor
void UserInterface::OnAddFloor(wxCommandEvent& event) {
    std::vector<std::string> names = fm_.get_all_floor_names();
    AddFloorWindow floorForm(this, names, num_added_+names.size());
    if (floorForm.ShowModal() == wxID_OK) {
        std::vector<int> tmp = floorForm.get_floor_neighbors();
        std::cout << "In UI, OnAddFloor method. Size of neighbors: ";
        std::cout << tmp.size() <<endl;
        if (fm_.add_floor(floorForm.get_floor_name(), floorForm.get_floor_room_type(), floorForm.get_floor_type(), floorForm.get_floor_size(), floorForm.get_floor_interaction(), floorForm.get_floor_neighbors())) {
            wxMessageBox(wxT(""), wxT("Floor Added Successfully"), wxICON_INFORMATION);
            num_added_++;
        } else {
            wxMessageBox(wxT(""), wxT("Could Not Add Floor."), wxICON_INFORMATION);
        }
    }
}

void UserInterface::OnAddTask(wxCommandEvent& event) {
    std::vector<std::string> floor_names = fm_.get_all_floor_names();
    std::vector<std::string> robot_names = fm_.get_all_robot_names();
    AddTaskWindow taskForm(this, floor_names, robot_names, num_added_);
    if (taskForm.ShowModal() == wxID_OK) {
        std::vector<int> tmp;
        tmp.push_back(std::stoi(taskForm.get_floor()));
        if (fm_.add_task_to_back(std::stoi(taskForm.get_robot()), tmp)) {

            wxMessageBox(wxT(""), wxT("Task Added Successfully"), wxICON_INFORMATION);
        } else {
            wxMessageBox(wxT(""), wxT("Could Not Add Floor."), wxICON_INFORMATION);
        }
    }
}

// Method to set the main text at the top of the window
void UserInterface::setText(const std::string& new_text) {
    if (display_text_) { 
        wxCommandEvent* event = new wxCommandEvent(wxEVT_COMMAND_TEXT_UPDATED, GetId());
        event->SetString(new_text);
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

// Observer pattern methods
void UserInterface::subscribe(Subscriber* subscriber, const Event& event) {
    subscribers_[event].push_back(subscriber);
}

// Let the subscriber unsubscribe from an event
void UserInterface::unsubscribe(Subscriber* subscriber, const Event& event) {
    auto& subs = subscribers_[event];
    subs.erase(std::remove(subs.begin(), subs.end(), subscriber), subs.end());
}

// Notify all the subscribers
void UserInterface::notify(const Event& event, const std::string& data) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, data);
    }
}

void UserInterface::notify(const Event& event, const int id, const std::vector<int>& data) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, id, data);
    }
}

void UserInterface::subscribe(const Event& event) {
    // subscribe to an event
    fm_.subscribe(this, event);
}

void UserInterface::unsubscribe(const Event& event) {
    // unsubscribe from an event
    fm_.unsubscribe(this, event);
}

void UserInterface::update(const Event& event, const std::string& data) {
    // Do a particular method depending on what type of event is being updated
    if (event == Event::DisplayText) {
        handle_display_text(data);
    }
}

void UserInterface::update(const Event& event, const int id, const std::vector<int>& data) {
    // changes needed here
}

// Method called after event "display_text" occurs
void UserInterface::handle_display_text(const std::string& data) {
    setText(data);
}

// Method runs when the window is closed (TODO: fix the crash)
void UserInterface::OnClose(wxCloseEvent& event) {
    std::cout << "closed" << std::endl;
    event.Skip();
}
