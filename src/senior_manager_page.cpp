#include "ui/senior_manager_page.hpp"
#include "ui/user_interface.hpp"
#include <wx/grid.h>
#include "ui/login_page.hpp"

SeniorManagerPage::SeniorManagerPage(const wxString& title) 
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)), dbmanager_{DBManager::getInstance("mongodb://localhost:27017", "database")}, 
                                robot_adapter_{dbmanager_.getDatabase()["robots"]}, floor_adapter_{dbmanager_.getDatabase()["floors"]} {
    wxPanel* panel = new wxPanel(this);
    wxGrid* grid = new wxGrid(panel, wxID_ANY, wxDefaultPosition, wxSize(800, 600));

    unordered_map<std::string, std::vector<std::string>> robots = fm_.get_table_data();
    std::cout << robots["name"].size() << std::endl;
    
    int rows = robots["name"].size();
    int cols = 6;
    grid->CreateGrid(rows, cols);

    grid->SetDefaultColSize(90, true);
    grid->SetDefaultRowSize(35, true);
    grid->SetLabelBackgroundColour(*wxWHITE);

    grid->SetColLabelValue(0, "ID");
    grid->SetColLabelValue(1, "Name");
    grid->SetColLabelValue(2, "Type");
    grid->SetColLabelValue(3, "Total Battery");
    grid->SetColLabelValue(4, "Rooms Cleaned");
    grid->SetColLabelValue(5, "Total Errors");

    for (int row = 0; row < rows; ++row) {
        grid->SetRowLabelValue(row, "Robot " + std::to_string(row + 1));
    }

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (col == 0) {
                grid->SetCellValue(row, col, robots["id"][row]);  
            } else if (col == 1) {
                grid->SetCellValue(row, col, robots["name"][row]);  
            } else if (col == 2) {
                grid->SetCellValue(row, col, robots["type"][row]);  
            } else {
                grid->SetCellValue(row, col, "-");  
            }
        }
    }

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxButton* btn1 = new wxButton(panel, wxID_ANY, "Logout", wxDefaultPosition, wxSize(150, 45));
    btn1->Bind(wxEVT_BUTTON, &SeniorManagerPage::Logout, this);
    sizer->Add(btn1, 0, wxALIGN_RIGHT | wxALL, 5);

    sizer->Add(grid, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 20);

    panel->SetBackgroundColour(*wxWHITE);
    panel->SetSizer(sizer);
    grid->SetDefaultCellBackgroundColour(*wxWHITE);
    sizer->SetSizeHints(this);
}

void SeniorManagerPage::Logout(wxCommandEvent& evt) {
    this->Hide();

    // Show or create the UserInterface window
    LoginPage* login = new LoginPage("Login");
    login->SetClientSize(800, 600);
    login->Center();
    login->Show();
}