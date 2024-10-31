#include <iostream>
#include "database/database.hpp"
#include "simulation/simulator.hpp"
#include "sys_manager/fleet_manager.hpp"
#include "ui/user_interface.hpp"
#include "ui/main_frame.hpp"
#include <wx/wx.h>
 

bool UserInterface::OnInit() {
    MainFrame* mf = new MainFrame("C++ GUI");
    mf->SetClientSize(800, 600);
    mf->Center();
    mf->Show();

    return true;
}

wxIMPLEMENT_APP(UserInterface);