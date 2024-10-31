#ifndef UI_USER_INTERFACE
#define UI_USER_INTERFACE

#include <wx/wx.h>
#include "../simulation/simulator.hpp"
#include "../sys_manager/fleet_manager.hpp"
#include "../database/database.hpp"
 
class UserInterface : public wxFrame {
    public:
        UserInterface(const wxString& title);

    private:
        void OnButtonClicked(wxCommandEvent& evt);
        wxDECLARE_EVENT_TABLE();
};

#endif