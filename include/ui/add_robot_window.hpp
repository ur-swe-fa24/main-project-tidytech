#ifndef UI_ADD_ROBOT_WINDOW
#define UI_ADD_ROBOT_WINDOW

#include <wx/wx.h>

class AddRobotWindow : public wxDialog {
    public:
        AddRobotWindow(wxWindow* parent);
        std::string get_type() {
            if (shampoo_button_->GetValue()) {
                return "Shampoo";
            } else if (vacuum_button_->GetValue()) {
                return "Vacuum";
            } else if (scrubber_button_->GetValue()) {
                return "Scrubber";
            } else {
                return "Error";
            }
        };
        std::string get_size() {
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
        std::string get_current_position() { return std::string(current_position_->GetValue().mb_str()); };
        std::string get_charging_position() { return std::string(charging_position_->GetValue().mb_str()); };
    private:
        wxRadioButton* shampoo_button_;
        wxRadioButton* scrubber_button_;
        wxRadioButton* vacuum_button_;
        wxRadioButton* small_button_;
        wxRadioButton* medium_button_;
        wxRadioButton* large_button_;
        wxTextCtrl* current_position_;
        wxTextCtrl* charging_position_;
};

#endif