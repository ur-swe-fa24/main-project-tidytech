#ifndef UI_SENIOR_MANAGER_PAGE
#define UI_SENIOR_MANAGER_PAGE

#include <wx/wx.h>
#include <wx/grid.h>
#include "sys_manager/fleet_manager.hpp"

 
class SeniorManagerPage : public wxFrame {
    public:
        SeniorManagerPage(const wxString& title, FleetManager* fm);
    private:
        FleetManager& fm_;
        void Logout(wxCommandEvent& evt);
};

#endif