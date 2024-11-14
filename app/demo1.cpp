#include <iostream>
// #include "database/database.hpp"
// #include "simulation/simulator.hpp"
// #include "sys_manager/fleet_manager.hpp"
#include "ui/user_interface.hpp"
#include "ui/login_page.hpp"
#include <wx/wx.h>
 

bool FleetManager::OnInit() {
    // UserInterface* ui = new UserInterface("C++ GUI");
    LoginPage* ui = new LoginPage("Login");
    
    ui->SetClientSize(800, 600);
    ui->Center();
    ui->Show();

    return true;
}

wxIMPLEMENT_APP(FleetManager);