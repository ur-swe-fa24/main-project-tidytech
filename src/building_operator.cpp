#include "ui/building_operator.hpp"
#include "ui/add_robot_window.hpp"
#include "ui/add_floor_window.hpp"
#include "ui/add_task_window.hpp"
#include "ui/login_page.hpp"
#include <wx/wx.h>
#include <regex>
#include <sstream>

BuildingOperator::BuildingOperator(const wxString& title, FleetManager* fm) : wxFrame(nullptr, wxID_ANY, title), fm_(*fm) {
    // Subscribe to the events pushed by the simulator to affect the UI
    subscribe(Event::FiveSecReport);
    subscribe(Event::FiveSecReportFloors);

    // Bind these events to update the grid each simulation tick
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &BuildingOperator::OnUpdateGrid, this, 1);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &BuildingOperator::OnUpdateGridFloors, this, 2);

    wxScrolledWindow* scrolledWindow = new wxScrolledWindow(this, wxID_ANY);
    scrolledWindow->SetScrollRate(5, 5);

    // Crate a panel for the whole window
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Logout button navigates back to the home page
    wxButton* btn1 = new wxButton(scrolledWindow, wxID_ANY, "Logout", wxDefaultPosition, wxSize(150, 45));
    btn1->Bind(wxEVT_BUTTON, &BuildingOperator::Logout, this);
    mainSizer->Add(btn1, 0, wxALIGN_RIGHT | wxALL, 5);

    // Create a sizer for the the add task button
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    // Add task button to assign robots to rooms
    wxButton* openAddTask = new wxButton(scrolledWindow, wxID_ANY, "Add Task", wxDefaultPosition, wxSize(150, 45));
    openAddTask->Bind(wxEVT_BUTTON, &BuildingOperator::OnAddTask, this);
    buttonSizer->Add(openAddTask, 0, wxALL, 5);

    // Add a horizontal sizer for the radio buttons
    wxBoxSizer* radioButtonSizer = new wxBoxSizer(wxHORIZONTAL);

    // Retrieve the robot names from the fleet manager
    std::vector<std::string> robotNames = fm_.get_all_robot_names();
    wxRadioButton* firstButton = nullptr; // To track the first button for default selection

    // Add a radio button for each robot currently in the simulation
    for (size_t i = 0; i < robotNames.size(); ++i) {
        wxRadioButton* radioButton = new wxRadioButton(
            scrolledWindow,                 // Parent
            wxID_ANY,                       // ID
            robotNames[i],                  // Label
            wxDefaultPosition,              // Position
            wxDefaultSize,                  // Size
            i == 0 ? wxRB_GROUP : 0         // Group the first button
        );

        // Set the first button as checked by default
        if (i == 0) {
            selected_robot_ = robotNames[i]; // Default selection
            radioButton->SetValue(true);
        }

        // Bind event to handle selection
        radioButton->Bind(wxEVT_RADIOBUTTON, [this, radioButton](wxCommandEvent& event) {
            selected_robot_ = std::string(radioButton->GetLabel().mb_str());
        });

        // Add the radio button to the sizer
        radioButtonSizer->Add(radioButton, 0, wxALL, 5);
    }

    // Add the buttons to the main sizer
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    // Add the radio buttons to the main sizer
    mainSizer->Add(radioButtonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    // Get the information currently in the database to initialize the table with on the window opening
    unordered_map<std::string, std::vector<std::string>> robots = fm_.get_table_data();
    grid = new wxGrid(scrolledWindow, wxID_ANY, wxDefaultPosition, wxSize(630, 80));
    
    // Fill in the table with the informatoin
    int rows = 1;
    int cols = 8;
    grid->CreateGrid(rows, cols);

    grid->SetDefaultColSize(90, true);
    grid->SetDefaultRowSize(25, true);
    grid->SetLabelBackgroundColour(*wxWHITE);

    grid->SetColLabelValue(0, "ID");
    grid->SetColLabelValue(1, "Name");
    grid->SetColLabelValue(2, "Type");
    grid->SetColLabelValue(3, "Battery");
    grid->SetColLabelValue(4, "Capacity");
    grid->SetColLabelValue(5, "Status");
    grid->SetColLabelValue(6, "Cur Room");
    grid->SetColLabelValue(7, "Tasks");

    for (int row = 0; row < rows; ++row) {
        grid->SetRowLabelValue(row, "Robot");
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

    // Create a section to show the robot table
    display_text_ = new wxStaticText(scrolledWindow, wxID_ANY, "My Robots:",
                                     wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    display_text_->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    mainSizer->Add(display_text_, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
    mainSizer->Add(grid, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 20);

    // Get the information currently in the database to initialize the table with on the window opening
    unordered_map<std::string, std::vector<std::string>> floors = fm_.get_table_data_floors();
    grid2 = new wxGrid(scrolledWindow, wxID_ANY, wxDefaultPosition, wxSize(630, 200));
    
    // Fill in table with information
    int rows2 = floors["name"].size();
    int cols2 = 7;
    grid2->CreateGrid(rows2, cols2);

    grid2->SetDefaultColSize(90, true);
    grid2->SetDefaultRowSize(25, true);
    grid2->SetLabelBackgroundColour(*wxWHITE);

    grid2->SetColLabelValue(0, "ID");
    grid2->SetColLabelValue(1, "Name");
    grid2->SetColLabelValue(2, "Room Type");
    grid2->SetColLabelValue(3, "Floor Type");
    grid2->SetColLabelValue(4, "Interaction");
    grid2->SetColLabelValue(5, "Clean Level");
    grid2->SetColLabelValue(6, "Neighbors");

    for (int row = 0; row < rows2; ++row) {
        grid2->SetRowLabelValue(row, "Floor " + std::to_string(row + 1));
    }

    for (int row = 0; row < rows2; ++row) {
        for (int col = 0; col < cols2; ++col) {
            if (col == 0) {
                grid2->SetCellValue(row, col, floors["id"][row]);  
            } else if (col == 1) {
                grid2->SetCellValue(row, col, floors["name"][row]);  
            } else if (col == 2) {
                grid2->SetCellValue(row, col, floors["room_type"][row]);  
            } else if (col == 3) {
                grid2->SetCellValue(row, col, floors["floor_type"][row]); 
            } else if (col == 4) {
                grid2->SetCellValue(row, col, floors["interaction"][row]); 
            } else if (col == 5) {
                grid2->SetCellValue(row, col, floors["clean_level"][row]); 
            } else if (col == 6) {
               grid2->SetCellValue(row, col, floors["neighbors"][row]); 
            } else {
                grid2->SetCellValue(row, col, "-");  
            }
        }
    }

    // Create a section to show information about the floors
    display_text_ = new wxStaticText(scrolledWindow, wxID_ANY, "My Floors:",
                                     wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    display_text_->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    mainSizer->Add(display_text_, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
    mainSizer->Add(grid2, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 20);

    // Set sizer
    scrolledWindow->SetSizer(mainSizer);
    mainSizer->SetSizeHints(scrolledWindow);

    // Fit the scrolled window into the frame
    wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(scrolledWindow, 1, wxEXPAND);
    SetSizer(frameSizer);
    frameSizer->SetSizeHints(this);

    CreateStatusBar();
    Fit();
}


// Method to handle clicking the "add task" button
void BuildingOperator::OnAddTask(wxCommandEvent& event) {
    std::vector<std::string> floor_names = fm_.get_all_floor_names();
    std::vector<std::string> robot_names = fm_.get_all_robot_names();
    AddTaskWindow taskForm(this, floor_names, {selected_robot_}, floor_names.size());
    if (taskForm.ShowModal() == wxID_OK) {
        if (taskForm.get_robot() == "-1" || taskForm.get_floor(floor_names.size()).size() == 0) {
            // do nothing 
        } else if (fm_.add_task_to_back(std::stoi(taskForm.get_robot()), taskForm.get_floor(floor_names.size()))) {
            wxMessageBox(wxT(""), wxT("Task Added Successfully"), wxICON_INFORMATION);
        } else {
            wxMessageBox(wxT(""), wxT("Could Not Add Task."), wxICON_INFORMATION);
        }
    }
}

// Observer pattern methods
void BuildingOperator::subscribe(Subscriber* subscriber, const Event& event) {
    subscribers_[event].push_back(subscriber);
}

// Let the subscriber unsubscribe from an event
void BuildingOperator::unsubscribe(Subscriber* subscriber, const Event& event) {
    auto& subs = subscribers_[event];
    subs.erase(std::remove(subs.begin(), subs.end(), subscriber), subs.end());
}

// Notify all the subscribers
void BuildingOperator::notify(const Event& event, const std::string& data) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, data);
    }
}

void BuildingOperator::notify(const Event& event, const int id) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, id);
    }
}

void BuildingOperator::notify(const Event& event, const int id, const int val) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, id, val);
    }
}

void BuildingOperator::notify(const types::Event& event, const int id, const ErrorType error_type, const bool resolved) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, id, error_type, resolved);
    }
}

void BuildingOperator::notify(const Event& event, const int id, const std::vector<int>& data) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, id, data);
    }
}

void BuildingOperator::notify(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, id, currentLocation, status, capacity, taskQueue, path, currentBattery, totalBatteryUsed);
    }
}

void BuildingOperator::subscribe(const Event& event) {
    // subscribe to an event
    fm_.subscribe(this, event);
}

void BuildingOperator::unsubscribe(const Event& event) {
    // unsubscribe from an event
    fm_.unsubscribe(this, event);
}

void BuildingOperator::update(const Event& event, const std::string& data) {
    // Do a particular method depending on what type of event is being updated
    if (event == Event::FiveSecReport) {
        handle_five_sec(data);
    } else if (event == Event::FiveSecReportFloors) {
        handle_five_sec_floors(data);
    }
}

void BuildingOperator::update(const Event& event, const int id, const std::vector<int>& data) {
    // do nothing
}

void BuildingOperator::update(const types::Event& event, const int id, const ErrorType error_type, const bool resolved) {
    // do nothing
}

void BuildingOperator::update(const types::Event& event, const int id) {
    // do nothing
}

void BuildingOperator::update(const types::Event& event, const int id, const int val) {
    // do nothing
}

void BuildingOperator::update(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) {
    // do nothing
}

// Method handles the pings sent from the simulation to update robots
void BuildingOperator::handle_five_sec(const std::string& data) {
    std::vector<std::vector<std::string>> updated_table_info = extract_five_ping(data);
    if (updated_table_info[0][1] == selected_robot_) {
        update_grid(updated_table_info);
    }
}

// Method handles the pings sent from the simulation to update floors
void BuildingOperator::handle_five_sec_floors(const std::string& data) {
    update_grid_floors(data);
}

// Method to handle the user clicking "logout," takes user back to login page
void BuildingOperator::Logout(wxCommandEvent& evt) {
    this->Hide();
    LoginPage* login = new LoginPage("Login", &fm_);
    login->SetClientSize(800, 600);
    login->Center();
    login->Show();
}

// Adds a row to the grid when the user adds a robot
void BuildingOperator::AddRowToGrid(std::vector<std::string> row_info) {
    int row = grid->GetNumberRows();
    grid->AppendRows(1);

    grid->SetRowLabelValue(row, "Robot " + std::to_string(row + 1));
    grid->SetCellValue(row, 0, row_info[0]);
    grid->SetCellValue(row, 1, row_info[1]);
    grid->SetCellValue(row, 2, row_info[2]);
    grid->SetCellValue(row, 3, row_info[3]);
    grid->SetCellValue(row, 4, row_info[4]);
    grid->SetCellValue(row, 5, row_info[5]);

    grid->Refresh();
}

// Adds a row to the grid when the user adds a floor
void BuildingOperator::AddRowToGridFloor(std::vector<std::string> row_info) {
    int row = grid2->GetNumberRows();
    grid2->AppendRows(1);

    grid2->SetRowLabelValue(row, "Floor " + std::to_string(row + 1));
    grid2->SetCellValue(row, 0, row_info[0]);
    grid2->SetCellValue(row, 1, row_info[1]);
    grid2->SetCellValue(row, 2, row_info[2]);
    grid2->SetCellValue(row, 3, row_info[3]);
    grid2->SetCellValue(row, 4, row_info[4]);
    grid2->SetCellValue(row, 5, row_info[5]);
    grid2->SetCellValue(row, 6, row_info[6]);

    grid2->Refresh();
}

// Cleans the to_string output from the simulation about each robot
std::vector<std::vector<std::string>> BuildingOperator::extract_five_ping(std::string input) {
    // This function was heavily inspired by LLM output
    std::vector<std::vector<std::string>> robots;
    std::istringstream stream(input);
    std::string line;
    std::string id, name, status, battery, capacity, type, current_location, tasks;

    while (std::getline(stream, line)) {
        if (line.find("Robot Id:") != std::string::npos) {
            id = line.substr(line.find(":") + 2, line.find(",") - (line.find(":") + 2));
            name = line.substr(line.find("Name:") + 6);
        } else if (line.find("Size:") != std::string::npos) {
            type = line.substr(line.find("Type:") + 6);
        } else if (line.find("Status:") != std::string::npos) {
            status = line.substr(line.find("Status:") + 8, line.find(",") - (line.find("Status:") + 8));
            battery = line.substr(line.find("Battery:") + 9, line.find(",", line.find("Battery:")) - (line.find("Battery:") + 9));
            capacity = line.substr(line.find("Remaining Capacity:") + 20);
            robots.push_back({id, name, type, battery, capacity, status});
        } else if (line.find("Current Location:") != std::string::npos) {
            current_location = line.substr(line.find("Current Location:") + 17);
        } else if (line.find("Tasks:") != std::string::npos) {
            tasks = line.substr(line.find("Tasks:") + 7);
            robots.back().push_back(current_location);
            robots.back().push_back(tasks);
        }
    }

    return robots;
}

// Updates the grid for each robot on each simulation tick
void BuildingOperator::update_grid(const std::vector<std::vector<std::string>>& robotData) {
    for (int col = 0; col < 8; col++) {
        std::string value = robotData[0][col];
        wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, 1);
        event.SetString(value.c_str());
        event.SetInt(0);
        event.SetExtraLong(col);
        wxPostEvent(this, event);
    }
}

// Necessary method for wxWidgets to update the grid live
void BuildingOperator::OnUpdateGrid(wxCommandEvent& evt) {
    int row = evt.GetInt();
    int col = static_cast<int>(evt.GetExtraLong());
    std::string value = std::string(evt.GetString().mb_str());
    grid->SetCellValue(row, col, value);  
    grid->ForceRefresh(); 
}

// Updates the grid for each floor on each simulation tick
void BuildingOperator::update_grid_floors(std::string cleanLevel) {
    size_t pos = cleanLevel.find(',');
    std::string id = cleanLevel.substr(0, pos);
    std::string level = cleanLevel.substr(pos + 1);

    wxCommandEvent event2(wxEVT_COMMAND_BUTTON_CLICKED, 2);
    event2.SetString(level.c_str());
    event2.SetInt(std::stoi(id) - 1);
    event2.SetExtraLong(5);
    wxPostEvent(this, event2);
}

// Necessary method for wxWidgets to update the floor grid live
void BuildingOperator::OnUpdateGridFloors(wxCommandEvent& evt) {
    int row = evt.GetInt();
    int col = 5;
    std::string value = std::string(evt.GetString().mb_str());
    grid2->SetCellValue(row, col, value);  
    grid2->ForceRefresh(); 
}

// Update the neigbors of the existing floors when a new neighbor to those floors are added
void BuildingOperator::update_grid_neighbors() {
    unordered_map<std::string, std::vector<std::string>> updatedFloors = fm_.get_table_data_floors();
    int rows = updatedFloors["name"].size();
    for (int row = 0; row < rows; ++row) {
        std::string newNeighbors = updatedFloors["neighbors"][row];
        grid2->SetCellValue(row, 6, newNeighbors);
    }
    grid2->ForceRefresh();
}
