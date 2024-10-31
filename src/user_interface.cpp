#include "ui/user_interface.hpp"
#include <wx/wx.h>

enum IDs {
    BUTTON_ID = 2
};

wxBEGIN_EVENT_TABLE(UserInterface, wxFrame)
    EVT_BUTTON(BUTTON_ID, UserInterface::OnButtonClicked)
wxEND_EVENT_TABLE()

UserInterface::UserInterface(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    wxPanel* panel = new wxPanel(this);
    wxButton* btn = new wxButton(panel, BUTTON_ID, "Button", wxPoint(350, 100), wxSize(100, 35));
    CreateStatusBar();
}

void UserInterface::OnButtonClicked(wxCommandEvent& evt) {
    auto f1 = []() -> void {
        int clock = 0;
        while (clock < 5) {
            std::cout << clock << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            clock++;
        }
    };
    std::thread t1(f1);
    t1.detach();
    wxLogStatus("Button Clicked");
    
}