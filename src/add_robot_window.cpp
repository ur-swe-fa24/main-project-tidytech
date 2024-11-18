#include "ui/add_robot_window.hpp"

AddRobotWindow::AddRobotWindow(wxWindow* parent, std::vector<std::string> names)
        : wxDialog(parent, wxID_ANY, "Form Dialog", wxDefaultPosition, wxSize(300, 450)) {

    std::reverse(names.begin(), names.end());
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Name text field
    robot_name_ = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(250, 25));
    sizer->Add(new wxStaticText(this, wxID_ANY, "Robot Name:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    sizer->Add(robot_name_, 0, wxALL, 10);

    // Robot type options
    wxBoxSizer* robotTypeSizer = new wxBoxSizer(wxHORIZONTAL);
    shampoo_button_ = new wxRadioButton(this, wxID_ANY, "Shampoo", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    vacuum_button_ = new wxRadioButton(this, wxID_ANY, "Vacuum");
    scrubber_button_ = new wxRadioButton(this, wxID_ANY, "Scrubber");
    
    robotTypeSizer->Add(shampoo_button_, 0, wxALL, 5);
    robotTypeSizer->Add(vacuum_button_, 0, wxALL, 5);
    robotTypeSizer->Add(scrubber_button_, 0, wxALL, 5);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Select Type:"), 0, wxLEFT | wxRIGHT, 10);
    sizer->Add(robotTypeSizer, 0, wxLEFT | wxRIGHT, 10);

    // Robot size options
    wxBoxSizer* robotSizeSizer = new wxBoxSizer(wxHORIZONTAL);
    small_button_ = new wxRadioButton(this, wxID_ANY, "Small", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    medium_button_ = new wxRadioButton(this, wxID_ANY, "Medium");
    large_button_ = new wxRadioButton(this, wxID_ANY, "Large");
    
    robotSizeSizer->Add(small_button_, 0, wxALL, 5);
    robotSizeSizer->Add(medium_button_, 0, wxALL, 5);
    robotSizeSizer->Add(large_button_, 0, wxALL, 5);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Select Size:"), 0, wxLEFT | wxRIGHT, 10);
    sizer->Add(robotSizeSizer, 0, wxLEFT | wxRIGHT, 10);

    // TODO: Store the information in the checkboxes
    wxBoxSizer* defaultPositionSizer = new wxBoxSizer(wxVERTICAL);
    defaultPositionSizer->Add(new wxStaticText(this, wxID_ANY, "Charging Room:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    defaultPositionSizer->Add(new wxRadioButton(this, wxID_ANY, "No Room", wxDefaultPosition, wxDefaultSize, wxRB_GROUP), 0, wxALL, 5);
    for (int i = names.size() - 1; i >= 0; i--) {
    // for (int i = 0; i < names.size(); i++) {
        boxes_[i] = new wxRadioButton(this, wxID_ANY, names[i]);
        defaultPositionSizer->Add(boxes_[i], 0, wxALL, 5);
    }
    sizer->Add(defaultPositionSizer, 0, wxLEFT | wxRIGHT, 10);

    // Charging position input
    // charging_position_ = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(250, 25));
    // sizer->Add(new wxStaticText(this, wxID_ANY, "Charging Room (Room ID):"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    // sizer->Add(charging_position_, 0, wxALL, 10);

    // Current position input
    // current_position_ = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(250, 25));
    // sizer->Add(new wxStaticText(this, wxID_ANY, "Current Room (Room ID):"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    // sizer->Add(current_position_, 0, wxALL, 10);

    // Submit button
    wxButton* submitButton = new wxButton(this, wxID_OK, "Submit");
    sizer->Add(submitButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(sizer);
    Centre();
}