#ifndef UI_MAIN_FRAME_HPP
#define UI_MAIN_FRAME_HPP

#include <wx/wx.h>
#include "../simulation/simulator.hpp"
#include "../sys_manager/fleet_manager.hpp"
#include "../database/database.hpp"
 
class MainFrame : public wxFrame {
    public:
        MainFrame(const wxString& title);

    private:
        void OnButtonClicked(wxCommandEvent& evt);
        wxDECLARE_EVENT_TABLE();
};

#endif