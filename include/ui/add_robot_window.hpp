#ifndef UI_ADD_ROBOT_WINDOW
#define UI_ADD_ROBOT_WINDOW

#include <wx/wx.h>

class AddRobotWindow : public wxDialog {
    public:
        AddRobotWindow(wxWindow* parent, std::vector<std::string> names);

        // Everything below this is just a getter for the form rows
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
        std::string get_charging_position() {
            int sol;
            for (int i = 0; i < 11; i++) {
                if (boxes_[i] == nullptr) {
                    break;
                } else if (boxes_[i]->GetValue()) {
                    sol = i + 1;
                    return std::to_string(sol);
                }
            }
            return "-1";
        };
        
        std::string get_name() { return std::string(robot_name_->GetValue().mb_str()); };
    private:
        // members for all the options in the form
        wxRadioButton* shampoo_button_;
        wxRadioButton* scrubber_button_;
        wxRadioButton* vacuum_button_;
        wxRadioButton* small_button_;
        wxRadioButton* medium_button_;
        wxRadioButton* large_button_;
        wxTextCtrl* current_position_;
        wxTextCtrl* charging_position_;
        wxTextCtrl* robot_name_;
        wxRadioButton* floor1 = nullptr;
        wxRadioButton* floor2 = nullptr;
        wxRadioButton* floor3 = nullptr;
        wxRadioButton* floor4 = nullptr;
        wxRadioButton* floor5 = nullptr;
        wxRadioButton* floor6 = nullptr;
        wxRadioButton* floor7 = nullptr;
        wxRadioButton* floor8 = nullptr;
        wxRadioButton* floor9 = nullptr;
        wxRadioButton* floor10 = nullptr;
        wxRadioButton* floor11 = nullptr;
        int num_added_;
        std::vector<wxRadioButton*> boxes_ = {floor1, floor2, floor3, floor4, floor5, floor6, floor7, floor8, floor9, floor10, floor11};

        void OnClose(wxCloseEvent& event);
};

#endif