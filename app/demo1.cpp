#include <iostream>
#include "ui/user_interface.hpp"
#include "ui/login_page.hpp"
#include <wx/wx.h>
 

bool FleetManager::OnInit() {
    LoginPage* ui = new LoginPage("Login", this);
    
    ui->SetClientSize(800, 600);
    ui->Center();
    ui->Show();

    return true;
}

wxIMPLEMENT_APP(FleetManager);