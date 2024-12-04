#ifndef UI_ADD_TASK_WINDOW
#define UI_ADD_TASK_WINDOW

#include <wx/wx.h>

class AddTaskWindow : public wxDialog {
    public:
        AddTaskWindow(wxWindow* parent, std::vector<std::string> floor_names, std::vector<std::string> robot_names, int num_added);
        std::string get_robot() {
            int sol;
            for (int i = 0; i < 11; i++) {
                if (robots_[i] == NULL) {
                    break;
                } else if (robots_[i]->GetValue()) {
                    sol = i + 1;
                    return std::to_string(sol);
                }
            }
            return "-1";
        };

        std::vector<int> get_floor(int num_floors) {
            std::vector<int> sol;
            for (int i = 0; i < num_floors; i++) {
                if (floors_[i] == NULL) {
                    break;
                } else if (floors_[i]->GetValue()) {
                    sol.push_back(i + 1);
                }
            }
            return sol;
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
        wxCheckBox* floor1;
        wxCheckBox* floor2;
        wxCheckBox* floor3;
        wxCheckBox* floor4;
        wxCheckBox* floor5;
        wxCheckBox* floor6;
        wxCheckBox* floor7;
        wxCheckBox* floor8;
        wxCheckBox* floor9;
        wxCheckBox* floor10;
        wxCheckBox* floor11;
        int num_added_;
        std::vector<wxCheckBox*> floors_ = {floor1, floor2, floor3, floor4, floor5, floor6, floor7, floor8, floor9, floor10, floor11};
        std::vector<wxRadioButton*> robots_ = {robot1, robot2, robot3, robot4, robot5, robot6, robot7, robot8, robot9, robot10, robot11};

        void OnClose(wxCloseEvent& event);
};

#endif