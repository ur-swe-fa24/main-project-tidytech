#ifndef UI_ADD_FLOOR_WINDOW
#define UI_ADD_FLOOR_WINDOW

#include <wx/wx.h>

class AddFloorWindow : public wxDialog {
    public:
        AddFloorWindow(wxWindow* parent);
        std::string get_floor_name() { return std::string(floor_name_->GetValue().mb_str()); };
    private:
        wxTextCtrl* floor_name_;
};

#endif