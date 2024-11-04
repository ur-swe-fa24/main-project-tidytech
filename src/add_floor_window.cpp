#include "ui/add_floor_window.hpp"

AddFloorWindow::AddFloorWindow(wxWindow* parent)
        : wxDialog(parent, wxID_ANY, "Form Dialog", wxDefaultPosition, wxSize(300, 200)) {

    // Create a vertical sizer for layout
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Text input
    floor_name_ = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(250, 25));
    sizer->Add(new wxStaticText(this, wxID_ANY, "Floor Name:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    sizer->Add(floor_name_, 0, wxALL, 10);

    // // Radio button options
    // wxBoxSizer* radioSizer = new wxBoxSizer(wxHORIZONTAL);
    // wxRadioButton* radio1 = new wxRadioButton(this, wxID_ANY, "Option 1", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    // wxRadioButton* radio2 = new wxRadioButton(this, wxID_ANY, "Option 2");
    // radioSizer->Add(radio1, 0, wxALL, 5);
    // radioSizer->Add(radio2, 0, wxALL, 5);
    // sizer->Add(new wxStaticText(this, wxID_ANY, "Choose an option:"), 0, wxLEFT | wxRIGHT, 10);
    // sizer->Add(radioSizer, 0, wxLEFT | wxRIGHT, 10);

    // // Checkbox
    // wxCheckBox* checkBox = new wxCheckBox(this, wxID_ANY, "Check me!");
    // sizer->Add(checkBox, 0, wxALL, 10);

    // Submit button
    wxButton* submitButton = new wxButton(this, wxID_OK, "Submit");
    sizer->Add(submitButton, 0, wxALIGN_CENTER | wxALL, 10);

    // Set the sizer and center the dialog
    SetSizer(sizer);
    Centre();
}