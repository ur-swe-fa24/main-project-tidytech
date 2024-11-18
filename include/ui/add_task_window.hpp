#ifndef UI_ADD_TASK_WINDOW
#define UI_ADD_TASK_WINDOW

#include <wx/wx.h>

class AddTaskWindow : public wxDialog {
    public:
        AddTaskWindow(wxWindow* parent, std::vector<std::string> floor_names, std::vector<std::string> robot_names, int num_added);
        std::string get_robot() {
            int sol;
            for (int i = 0; i < 11; i++) {
                try {
                    if (robots_[i]->GetValue()) {
                        sol = i + 1;
                        return std::to_string(sol);
                    }
                } catch (...) {
                    continue;
                }
            }
            return "-1";
        };

        std::string get_floor() {
            int sol;
            for (int i = 0; i < 11; i++) {
                try {
                    if (floors_[i]->GetValue()) {
                        sol = i + 1;
                        return std::to_string(sol);
                    }
                } catch (...) {
                    continue;
                }
            }
            return "-1";
        };



    private:
        // members for all the options in the form
        wxTextCtrl* task_name_;
        wxRadioButton* robot1;
        wxRadioButton* robot2;
        wxRadioButton* robot3;
        wxRadioButton* robot4;
        wxRadioButton* robot5;
        wxRadioButton* robot6;
        wxRadioButton* robot7;
        wxRadioButton* robot8;
        wxRadioButton* robot9;
        wxRadioButton* robot10;
        wxRadioButton* robot11;
        wxRadioButton* floor1;
        wxRadioButton* floor2;
        wxRadioButton* floor3;
        wxRadioButton* floor4;
        wxRadioButton* floor5;
        wxRadioButton* floor6;
        wxRadioButton* floor7;
        wxRadioButton* floor8;
        wxRadioButton* floor9;
        wxRadioButton* floor10;
        wxRadioButton* floor11;
        int num_added_;
        std::vector<wxRadioButton*> floors_ = {floor1, floor2, floor3, floor4, floor5, floor6, floor7, floor8, floor9, floor10, floor11};
        std::vector<wxRadioButton*> robots_ = {robot1, robot2, robot3, robot4, robot5, robot6, robot7, robot8, robot9, robot10, robot11};
};

#endif