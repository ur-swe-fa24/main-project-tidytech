#include "ui/login_page.hpp"

LoginPage::LoginPage(const wxString& title, FleetManager* fm) : wxFrame(nullptr, wxID_ANY, title) {
    // Create a panel and sizer for the page
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Create a text section and add it to the sizer
    wxStaticText* tmp = new wxStaticText(panel, wxID_ANY, "Login:", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    tmp->SetFont(wxFont(32, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    mainSizer->Add(tmp, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 50);

    // Create a button for each user role that lets them "login" to that role
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);

    wxButton* btn1 = new wxButton(panel, wxID_ANY, "Senior Manager", wxDefaultPosition, wxSize(150, 45));
    btn1->Bind(wxEVT_BUTTON, [this, fm](wxCommandEvent& evt){this->OnSeniorManagerClick(evt, fm);});
    buttonSizer->Add(btn1, 0, wxALL, 5);

    wxButton* btn2 = new wxButton(panel, wxID_ANY, "Building Manager", wxDefaultPosition, wxSize(150, 45));
    btn2->Bind(wxEVT_BUTTON, [this, fm](wxCommandEvent& evt){this->OnButtonClick(evt, fm);});
    buttonSizer->Add(btn2, 0, wxALL, 5);

    wxButton* btn3 = new wxButton(panel, wxID_ANY, "Building Operator", wxDefaultPosition, wxSize(150, 45));
    btn3->Bind(wxEVT_BUTTON, [this, fm](wxCommandEvent& evt){this->OnBuildingOperatorClick(evt, fm);});
    buttonSizer->Add(btn3, 0, wxALL, 5);

    wxButton* btn4 = new wxButton(panel, wxID_ANY, "Field Engineer", wxDefaultPosition, wxSize(150, 45));
    btn4->Bind(wxEVT_BUTTON, [this, fm](wxCommandEvent& evt){this->OnFieldEngineerClick(evt, fm);});
    buttonSizer->Add(btn4, 0, wxALL, 5);

    // Add the buttons to the main sizer
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    // Set the sizers
    panel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);
    CreateStatusBar();
}

// Navigate to the building manager page
void LoginPage::OnButtonClick(wxCommandEvent& evt, FleetManager* fm) {
    this->Hide();

    UserInterface* ui = new UserInterface("User Interface", fm);
    ui->SetClientSize(800, 600);
    ui->Center();
    ui->Show();
}

// Navigagte to the senior manager page
void LoginPage::OnSeniorManagerClick(wxCommandEvent& evt, FleetManager* fm) {
    this->Hide();

    SeniorManagerPage* smp = new SeniorManagerPage("Senior Manager", fm);
    smp->SetClientSize(800, 600);
    smp->Center();
    smp->Show();
}

// Navigate to the building operator page
void LoginPage::OnBuildingOperatorClick(wxCommandEvent& evt, FleetManager* fm) {
    this->Hide();

    BuildingOperator* bop = new BuildingOperator("Building Operator", fm);
    bop->SetClientSize(800, 600);
    bop->Center();
    bop->Show();
}

// Navigate to the field engineer page
void LoginPage::OnFieldEngineerClick(wxCommandEvent& evt, FleetManager* fm) {
    this->Hide();

    FieldEngineer* fep = new FieldEngineer("Field Engineer", fm);
    fep->SetClientSize(800, 600);
    fep->Center();
    fep->Show();
}