#include "ui/senior_manager_page.hpp"
#include "ui/user_interface.hpp"
#include <wx/grid.h>
#include "ui/login_page.hpp"

SeniorManagerPage::SeniorManagerPage(const wxString& title, FleetManager* fm)
    : fm_(*fm), wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {

    // Create a panel for the page
    wxPanel* panel = new wxPanel(this);

    // Create a grid to show business analytics
    wxGrid* grid = new wxGrid(panel, wxID_ANY, wxDefaultPosition, wxSize(800, 600));

    // Get data from the database (from the fleet manager) to initalize the table to on the window loading
    unordered_map<std::string, std::vector<std::string>> robots = fm_.get_table_data_metrics();
    
    // Fill in the table
    int rows = robots["name"].size();
    int cols = 7;
    grid->CreateGrid(rows, cols);

    grid->SetDefaultColSize(90, true);
    grid->SetDefaultRowSize(35, true);
    grid->SetLabelBackgroundColour(*wxWHITE);

    grid->SetColLabelValue(0, "ID");
    grid->SetColLabelValue(1, "Name");
    grid->SetColLabelValue(2, "Size");
    grid->SetColLabelValue(3, "Type");
    grid->SetColLabelValue(4, "Total Battery");
    grid->SetColLabelValue(5, "Rooms Cleaned");
    grid->SetColLabelValue(6, "Total Errors");

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
                grid->SetCellValue(row, col, robots["size"][row]);  
            } else if (col == 3) {
                grid->SetCellValue(row, col, robots["type"][row]);  
            } else if (col == 4) {
                grid->SetCellValue(row, col, robots["total_battery_used"][row]);  
            } else if (col == 5) {
                grid->SetCellValue(row, col, robots["rooms_cleaned"][row]);  
            } else if (col == 6) {
                grid->SetCellValue(row, col, robots["error_count"][row]);  
            } else {
                grid->SetCellValue(row, col, "-");  
            }
        }
    }

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Create a logout button to take the user back to the login page
    wxButton* btn1 = new wxButton(panel, wxID_ANY, "Logout", wxDefaultPosition, wxSize(150, 45));
    btn1->Bind(wxEVT_BUTTON, &SeniorManagerPage::Logout, this);
    sizer->Add(btn1, 0, wxALIGN_RIGHT | wxALL, 5);

    // Add and set all sizers
    sizer->Add(grid, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 20);

    panel->SetBackgroundColour(*wxWHITE);
    panel->SetSizer(sizer);
    grid->SetDefaultCellBackgroundColour(*wxWHITE);
    sizer->SetSizeHints(this);
}

// Logout method takes the user back to the login page
void SeniorManagerPage::Logout(wxCommandEvent& evt) {
    this->Hide();

    // Show or create the UserInterface window
    LoginPage* login = new LoginPage("Login", &fm_);
    login->SetClientSize(800, 600);
    login->Center();
    login->Show();
}
