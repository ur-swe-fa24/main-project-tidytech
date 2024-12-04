#include "ui/add_floor_window.hpp"
#include "simulation/floorplan.hpp"
#include <algorithm>

AddFloorWindow::AddFloorWindow(wxWindow* parent, std::vector<std::string> names, int num_added)
        : wxDialog(parent, wxID_ANY, "Form Dialog", wxDefaultPosition, wxSize(300, 600)) {

    
    num_added_ = num_added;
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    Bind(wxEVT_CLOSE_WINDOW, &AddFloorWindow::OnClose, this);

    // Floor name
    floor_name_ = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(250, 25));
    sizer->Add(new wxStaticText(this, wxID_ANY, "Floor Name:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    sizer->Add(floor_name_, 0, wxALL, 10);

    // Room floor type
    wxBoxSizer* floorRoomTypeSizer = new wxBoxSizer(wxHORIZONTAL);
    hallway_button_ = new wxRadioButton(this, wxID_ANY, "Hallway", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    elevator_button_ = new wxRadioButton(this, wxID_ANY, "Elevator");
    room_button_ = new wxRadioButton(this, wxID_ANY, "Room");
    floorRoomTypeSizer->Add(hallway_button_, 0, wxALL, 5);
    floorRoomTypeSizer->Add(elevator_button_, 0, wxALL, 5);
    floorRoomTypeSizer->Add(room_button_, 0, wxALL, 5);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Room Type:"), 0, wxLEFT | wxRIGHT, 10);
    sizer->Add(floorRoomTypeSizer, 0, wxLEFT | wxRIGHT, 10);

    // Floor type
    wxBoxSizer* floorTypeSizer = new wxBoxSizer(wxHORIZONTAL);
    wood_button_ = new wxRadioButton(this, wxID_ANY, "Wood", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    tile_button_ = new wxRadioButton(this, wxID_ANY, "Tile");
    carpet_button_ = new wxRadioButton(this, wxID_ANY, "Carpet");
    floorTypeSizer->Add(wood_button_, 0, wxALL, 5);
    floorTypeSizer->Add(tile_button_, 0, wxALL, 5);
    floorTypeSizer->Add(carpet_button_, 0, wxALL, 5);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Floor Type:"), 0, wxLEFT | wxRIGHT, 10);
    sizer->Add(floorTypeSizer, 0, wxLEFT | wxRIGHT, 10);

    // Room floor type
    wxBoxSizer* floorSizeSizer = new wxBoxSizer(wxHORIZONTAL);
    small_button_ = new wxRadioButton(this, wxID_ANY, "Small", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    medium_button_ = new wxRadioButton(this, wxID_ANY, "Medium");
    large_button_ = new wxRadioButton(this, wxID_ANY, "Large");
    floorSizeSizer->Add(small_button_, 0, wxALL, 5);
    floorSizeSizer->Add(medium_button_, 0, wxALL, 5);
    floorSizeSizer->Add(large_button_, 0, wxALL, 5);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Floor Size:"), 0, wxLEFT | wxRIGHT, 10);
    sizer->Add(floorSizeSizer, 0, wxLEFT | wxRIGHT, 10);

    // Floor usage
    wxBoxSizer* floorInteractionSizer = new wxBoxSizer(wxHORIZONTAL);
    low_button_ = new wxRadioButton(this, wxID_ANY, "Low", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    moderate_button_ = new wxRadioButton(this, wxID_ANY, "Moderate");
    high_button_ = new wxRadioButton(this, wxID_ANY, "High");
    floorInteractionSizer->Add(low_button_, 0, wxALL, 5);
    floorInteractionSizer->Add(moderate_button_, 0, wxALL, 5);
    floorInteractionSizer->Add(high_button_, 0, wxALL, 5);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Interaction Level:"), 0, wxLEFT | wxRIGHT, 10);
    sizer->Add(floorInteractionSizer, 0, wxLEFT | wxRIGHT, 10);

    std::reverse(names.begin(), names.end());
    for (int i = 0; i < names.size(); i++) {
        boxes_[i] = new wxCheckBox(this, wxID_ANY, names[i]);
        sizer->Add(boxes_[i], 0, wxALL, 10);
    }

    // Submit button
    wxButton* submitButton = new wxButton(this, wxID_OK, "Submit");
    sizer->Add(submitButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(sizer);
    Centre();
}

void AddFloorWindow::OnClose(wxCloseEvent& event) {
    EndModal(wxID_CANCEL);
}