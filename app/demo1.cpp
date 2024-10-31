#include <iostream>
#include "database/database.hpp"
#include "simulation/simulator.hpp"
#include "sys_manager/fleet_manager.hpp"
#include "ui/user_interface.hpp"
#include <wx/wx.h>
 

bool FleetManager::OnInit() {
    UserInterface* mf = new UserInterface("C++ GUI");
    mf->SetClientSize(800, 600);
    mf->Center();
    mf->Show();

    return true;
}

wxIMPLEMENT_APP(FleetManager);