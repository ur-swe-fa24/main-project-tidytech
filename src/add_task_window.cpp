#include "ui/add_task_window.hpp"

AddTaskWindow::AddTaskWindow(wxWindow* parent, std::vector<std::string> floor_names, std::vector<std::string> robot_names, int num_added) : wxDialog(parent, wxID_ANY, "Form Dialog", wxDefaultPosition, wxSize(300, 450)) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    num_added_ = num_added;

    // Robot radio buttons
    wxBoxSizer* robotSizer = new wxBoxSizer(wxVERTICAL);
    robotSizer->Add(new wxStaticText(this, wxID_ANY, "Robot to assign:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    for (int i = 0; i < robot_names.size(); i++) {
        robots_[i] = new wxRadioButton(this, wxID_ANY, robot_names[i], wxDefaultPosition, wxDefaultSize, i == 0 ? wxRB_GROUP : 0);
        robotSizer->Add(robots_[i], 0, wxALL, 5);
    }
    sizer->Add(robotSizer, 0, wxLEFT | wxRIGHT, 10);

    // Floor radio buttons
    wxBoxSizer* floorSizer = new wxBoxSizer(wxVERTICAL);
    floorSizer->Add(new wxStaticText(this, wxID_ANY, "Floor:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    for (int i = 0; i < floor_names.size(); i++) {
        floors_[i] = new wxRadioButton(this, wxID_ANY, floor_names[i], wxDefaultPosition, wxDefaultSize, i == 0 ? wxRB_GROUP : 0);
        floorSizer->Add(floors_[i], 0, wxALL, 5);
    }
    sizer->Add(floorSizer, 0, wxLEFT | wxRIGHT, 10);

    // Submit button
    wxButton* submitButton = new wxButton(this, wxID_OK, "Submit");
    sizer->Add(submitButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(sizer);
    Centre();
}