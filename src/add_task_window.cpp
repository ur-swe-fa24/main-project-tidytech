#include "ui/add_task_window.hpp"

AddTaskWindow::AddTaskWindow(wxWindow* parent, std::vector<std::string> floor_names, std::vector<std::string> robot_names, int num_added)
    : wxDialog(parent, wxID_ANY, "Form Dialog", wxDefaultPosition, wxSize(300, 450)) {
    
    // Main sizer for the page
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Num added is needed to keep track of total number of floors added, for get_floor_neighbors method
    num_added_ = num_added;

    // Method is bound to the window closing, needed to throw wxID_CANCEL instead of wx_ID_OK
    Bind(wxEVT_CLOSE_WINDOW, &AddTaskWindow::OnClose, this);

    // Robot radio buttons
    wxBoxSizer* robotSizer = new wxBoxSizer(wxVERTICAL);
    robotSizer->Add(new wxStaticText(this, wxID_ANY, "Robot to assign:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    for (int i = robot_names.size() - 1; i >= 0; i--) {
        robots_[i] = new wxRadioButton(this, wxID_ANY, robot_names[i], wxDefaultPosition, wxDefaultSize, i == robot_names.size() - 1 ? wxRB_GROUP : 0);
        robotSizer->Add(robots_[i], 0, wxALL, 5);
    }
    sizer->Add(robotSizer, 0, wxLEFT | wxRIGHT, 10);

    // Floor radio buttons
    wxBoxSizer* floorSizer = new wxBoxSizer(wxVERTICAL);
    floorSizer->Add(new wxStaticText(this, wxID_ANY, "Floor:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    for (int i = floor_names.size() - 1; i >= 0; i--) {
        floors_[i] = new wxCheckBox(this, wxID_ANY, floor_names[i]);
        floorSizer->Add(floors_[i], 0, wxALL, 5);
    }
    sizer->Add(floorSizer, 0, wxLEFT | wxRIGHT, 10);

    // Submit button
    wxButton* submitButton = new wxButton(this, wxID_OK, "Submit");
    sizer->Add(submitButton, 0, wxALIGN_CENTER | wxALL, 10);

    // Set sizer and center the dialogue
    SetSizer(sizer);
    Centre();
}

// Method is bound to the window closing, needed to throw wxID_CANCEL instead of wx_ID_OK
void AddTaskWindow::OnClose(wxCloseEvent& event) {
    EndModal(wxID_CANCEL);
}