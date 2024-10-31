#include "ui/main_frame.hpp"
#include <wx/wx.h>

enum IDs {
    BUTTON_ID = 2
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(BUTTON_ID, MainFrame::OnButtonClicked)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    wxPanel* panel = new wxPanel(this);
    wxButton* btn = new wxButton(panel, BUTTON_ID, "Button", wxPoint(350, 100), wxSize(100, 35));
    CreateStatusBar();
}

void MainFrame::OnButtonClicked(wxCommandEvent& evt) {
    int clock = 0;
    while (clock < 5) {
        std::cout << clock << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        clock++;
    }
    wxLogStatus("Button Clicked");
    
}