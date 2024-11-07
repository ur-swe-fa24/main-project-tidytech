#ifndef UI_ADD_FLOOR_WINDOW
#define UI_ADD_FLOOR_WINDOW

#include <wx/wx.h>

class AddFloorWindow : public wxDialog {
    public:
        AddFloorWindow(wxWindow* parent, std::vector<std::string> names);

        // Everything below this is just a getter for each of the form rows
        std::string get_floor_name() { return std::string(floor_name_->GetValue().mb_str()); };
        std::string get_floor_room_type() {
            if (elevator_button_->GetValue()) {
                return "Elevator";
            } else if (room_button_->GetValue()) {
                return "Room";
            } else if (hallway_button_->GetValue()) {
                return "Hallway";
            } else {
                return "Error";
            }
        };
        std::string get_floor_type() {
            if (wood_button_->GetValue()) {
                return "Wood";
            } else if (tile_button_->GetValue()) {
                return "Tile";
            } else if (carpet_button_->GetValue()) {
                return "Carpet";
            } else {
                return "Error";
            }
        };
        std::string get_floor_size() {
            if (small_button_->GetValue()) {
                return "Small";
            } else if (medium_button_->GetValue()) {
                return "Medium";
            } else if (large_button_->GetValue()) {
                return "Large";
            } else {
                return "Error";
            }
        };
        std::string get_floor_interaction() {
            if (low_button_->GetValue()) {
                return "Low";
            } else if (moderate_button_->GetValue()) {
                return "Moderate";
            } else if (high_button_->GetValue()) {
                return "High";
            } else {
                return "Error";
            }
        };

    private:
        // members for all the options in the floor form
        wxTextCtrl* floor_name_;
        wxRadioButton* hallway_button_;
        wxRadioButton* elevator_button_;
        wxRadioButton* room_button_;
        wxRadioButton* wood_button_;
        wxRadioButton* tile_button_;
        wxRadioButton* carpet_button_;
        wxRadioButton* small_button_;
        wxRadioButton* medium_button_;
        wxRadioButton* large_button_;
        wxRadioButton* low_button_;
        wxRadioButton* moderate_button_;
        wxRadioButton* high_button_;


        
};

#endif