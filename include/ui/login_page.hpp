#ifndef UI_LOGIN_PAGE
#define UI_LOGIN_PAGE

#include "ui/user_interface.hpp"
#include "ui/senior_manager_page.hpp"
#include "ui/building_operator.hpp"

#include <wx/wx.h>

// class UserInterface;
 
class LoginPage : public wxFrame {
    public:
        LoginPage(const wxString& title, FleetManager* fm);
    private:
        void OnButtonClick(wxCommandEvent& evt, FleetManager* fm);
        void OnSeniorManagerClick(wxCommandEvent& evt, FleetManager* fm);
        void OnBuildingOperatorClick(wxCommandEvent& evt, FleetManager* fm);
        FleetManager* fm_;
        // UserInterface* userInterface;
};

#endif