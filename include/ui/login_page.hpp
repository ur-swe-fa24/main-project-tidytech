#ifndef UI_LOGIN_PAGE
#define UI_LOGIN_PAGE

#include "ui/user_interface.hpp"
#include "ui/senior_manager_page.hpp"

#include <wx/wx.h>

class UserInterface;
 
class LoginPage : public wxFrame {
    public:
        LoginPage(const wxString& title);
    private:
        void OnButtonClick(wxCommandEvent& evt);
        void OnSeniorManagerClick(wxCommandEvent& evt);
        UserInterface* userInterface;
};

#endif