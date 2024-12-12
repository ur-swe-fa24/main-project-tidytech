#include "ui/field_engineer.hpp"
#include <wx/grid.h>
#include "ui/login_page.hpp"
#include <sstream>

FieldEngineer::FieldEngineer(const wxString& title, FleetManager* fm) 
    : fm_(*fm), wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {

    // Subscribe to the report to update the robot grid live
    subscribe(Event::FiveSecReport);

    // Create  panel for the whole page
    wxPanel* panel = new wxPanel(this);

    // Create a grid to show informatoin about the robots that need maintenence
    grid = new wxGrid(panel, wxID_ANY, wxDefaultPosition, wxSize(800, 600));

    // Updates grid each simulation tick
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FieldEngineer::OnUpdateGrid, this, 1);

    // Get data to fill in the table based on the database info when the window loads
    unordered_map<std::string, std::vector<std::string>> robots = fm_.get_table_data_fe();

    // Fill in the table
    int rows = robots["name"].size();
    int cols = 4;
    grid->CreateGrid(rows, cols);

    grid->SetDefaultColSize(90, true);
    grid->SetDefaultRowSize(35, true);
    grid->SetLabelBackgroundColour(*wxWHITE);

    grid->SetColLabelValue(0, "ID");
    grid->SetColLabelValue(1, "Name");
    grid->SetColLabelValue(2, "Status");
    grid->SetColLabelValue(3, "Capacity");

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
                grid->SetCellValue(row, col, robots["status"][row]);  
            } else if (col == 3) {
                grid->SetCellValue(row, col, robots["capacity"][row]);
            }
        }
    }

    // Create the main sizer for the window
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Logout button to take the user back to the login page
    wxButton* btn1 = new wxButton(panel, wxID_ANY, "Logout", wxDefaultPosition, wxSize(150, 45));
    btn1->Bind(wxEVT_BUTTON, &FieldEngineer::Logout, this);
    mainSizer->Add(btn1, 0, wxALIGN_RIGHT | wxALL, 5);

    // Make the grid stretch and take up the available space
    mainSizer->Add(grid, 1, wxEXPAND | wxALL, 20);

    // Add horizontal sizer for the bottom buttons
    wxBoxSizer* resetSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btn2 = new wxButton(panel, wxID_ANY, "Reset Errors", wxDefaultPosition, wxSize(150, 45));
    btn2->Bind(wxEVT_BUTTON, [this, fm](wxCommandEvent& evt){fm_.resolve_all_robots();});
    wxButton* btn3 = new wxButton(panel, wxID_ANY, "Reset Capacity", wxDefaultPosition, wxSize(150, 45));
    btn3->Bind(wxEVT_BUTTON, [this, fm](wxCommandEvent& evt){fm_.reset_capacity_for_robots();});
    resetSizer->Add(btn2, 0, wxALL, 5);
    resetSizer->Add(btn3, 0, wxALL, 5);

    mainSizer->Add(resetSizer, 0, wxALIGN_CENTER | wxALL, 10);

    // Set sizers
    panel->SetBackgroundColour(*wxWHITE);
    panel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);
}


// Logout button takes the user back to the login page
void FieldEngineer::Logout(wxCommandEvent& evt) {
    this->Hide();

    LoginPage* login = new LoginPage("Login", &fm_);
    login->SetClientSize(800, 600);
    login->Center();
    login->Show();
}

// All the methods in the pubsub interface
void FieldEngineer::subscribe(const Event& event) {
    // subscribe to an event
    fm_.subscribe(this, event);
}

void FieldEngineer::unsubscribe(const Event& event) {
    // unsubscribe from an event
    fm_.unsubscribe(this, event);
}

void FieldEngineer::update(const Event& event, const std::string& data) {
    // Do a particular method depending on what type of event is being updated
    if (event == Event::FiveSecReport) {
        handle_five_sec(data);
    }
}

void FieldEngineer::update(const Event& event, const int id, const std::vector<int>& data) {
    // do nothing
}

void FieldEngineer::update(const types::Event& event, const int id, const ErrorType error_type, const bool resolved) {
    // do nothing
}

void FieldEngineer::update(const types::Event& event, const int id) {
    // do nothing
}

void FieldEngineer::update(const types::Event& event, const int id, const int val) {
    // do nothing
}

void FieldEngineer::update(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) {
    // do nothing
}

void FieldEngineer::handle_five_sec(const std::string& data) {
    std::vector<std::vector<std::string>> updated_table_info = extract_five_ping(data);
    update_grid(updated_table_info);
}

// Updates the robot grid each simulation tick
void FieldEngineer::update_grid(const std::vector<std::vector<std::string>>& robotData) {
    for (int col = 0; col < 4; col++) {
        std::string value = robotData[0][col];
        wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, 1);
        event.SetString(value.c_str());
        event.SetInt(std::stoi(robotData[0][0]) - 1);
        event.SetExtraLong(col);
        wxPostEvent(this, event);
    }
}

// wxWidgets method to update the grid each simulation tick
void FieldEngineer::OnUpdateGrid(wxCommandEvent& evt) {
    int row = evt.GetInt();
    int col = static_cast<int>(evt.GetExtraLong());
    std::string value = std::string(evt.GetString().mb_str());
    grid->SetCellValue(row, col, value);  
    grid->ForceRefresh(); 
}

// Clean the information output in the robot.to_string method
std::vector<std::vector<std::string>> FieldEngineer::extract_five_ping(std::string input) {
    // This function was heavily inspired by LLM output
    std::vector<std::vector<std::string>> robots;
    std::istringstream stream(input);
    std::string line;
    std::string id, name, status, capacity;

    while (std::getline(stream, line)) {
        if (line.find("Robot Id:") != std::string::npos) {
            id = line.substr(line.find(":") + 2, line.find(",") - (line.find(":") + 2));
            name = line.substr(line.find("Name:") + 6);
        } else if (line.find("Status:") != std::string::npos) {
            status = line.substr(line.find("Status:") + 8, line.find(",") - (line.find("Status:") + 8));
            capacity = line.substr(line.find("Remaining Capacity:") + 20);
            robots.push_back({id, name, status, capacity});
        }
    }

    return robots;
}

