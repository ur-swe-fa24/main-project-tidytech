#include <iostream>
// #include "database/database.hpp"
// #include "simulation/simulator.hpp"
// #include "sys_manager/fleet_manager.hpp"
#include "ui/user_interface.hpp"
#include <wx/wx.h>
 

bool FleetManager::OnInit() {
    UserInterface* ui = new UserInterface("C++ GUI");
    
    ui->SetClientSize(800, 600);
    ui->Center();
    ui->Show();

    return true;
}

wxIMPLEMENT_APP(FleetManager);