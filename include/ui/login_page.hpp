#ifndef UI_LOGIN_PAGE
#define UI_LOGIN_PAGE

#include "ui/user_interface.hpp"
#include <wx/wx.h>
 
class LoginPage : public wxFrame {
    public:
        LoginPage(const wxString& title);
    private:
        void OnButtonClick(wxCommandEvent& evt);
        UserInterface* userInterface;
};

#endif