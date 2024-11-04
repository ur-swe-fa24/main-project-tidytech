#include "ui/user_interface.hpp"
#include <wx/wx.h>

enum IDs {
    BUTTON_ID = 2
};

wxBEGIN_EVENT_TABLE(UserInterface, wxFrame)
    EVT_BUTTON(BUTTON_ID, UserInterface::OnButtonClicked)
wxEND_EVENT_TABLE()

UserInterface::UserInterface(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    subscribe("display_text");
    Bind(wxEVT_COMMAND_TEXT_UPDATED, &UserInterface::OnTextUpdated, this, GetId());
    Bind(wxEVT_CLOSE_WINDOW, &UserInterface::OnClose, this);
    wxPanel* panel = new wxPanel(this);
    wxButton* btn = new wxButton(panel, BUTTON_ID, "Button", wxPoint(350, 100), wxSize(100, 35));
    CreateStatusBar();

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    display_text_ = new wxStaticText(panel, wxID_ANY, "Final report will go here.",
                                            wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    display_text_->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    sizer->Add(display_text_, 1, wxALIGN_CENTER);
    panel->SetSizer(sizer);
}

void UserInterface::OnButtonClicked(wxCommandEvent& evt) {
    // auto f1 = []() -> void {
    //     int clock = 0;
    //     while (clock < 5) {
    //         std::cout << clock << std::endl;
    //         std::this_thread::sleep_for(std::chrono::seconds(1));
    //         clock++;
    //     }
    // };
    // std::thread t1(f1);
    // t1.detach();
    // wxLogStatus("Button Clicked");
    fm_.read_ui_input("../app/input.txt");
    fm_.start_sim();
}

void UserInterface::setText(const std::string& new_text) {
    if (display_text_) { 
        // Create a custom event
        wxCommandEvent* event = new wxCommandEvent(wxEVT_COMMAND_TEXT_UPDATED, GetId());
        event->SetString(new_text);
        
        // Queue the event to be processed on the main thread
        wxPostEvent(display_text_, *event);
    } else {
        wxLogError("error");
    }
}

void UserInterface::OnTextUpdated(wxCommandEvent& event) {
    display_text_->SetLabel(event.GetString());
    display_text_->GetParent()->Fit();
    Layout();
}

void UserInterface::subscribe(Subscriber* subscriber, const std::string& event) {
    subscribers_[event].push_back(subscriber);
}

// Let the subscriber unsubscribe from an event
void UserInterface::unsubscribe(Subscriber* subscriber, const std::string& event) {
    auto& subs = subscribers_[event];
    subs.erase(std::remove(subs.begin(), subs.end(), subscriber), subs.end());
}

// Notify all the subscribers
void UserInterface::notify(const std::string& event, const std::string& data) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, data);
    }
}

void UserInterface::subscribe(const std::string& event) {
    // subscribe to an event
    fm_.subscribe(this, event);
}

void UserInterface::unsubscribe(const std::string& event) {
    // unsubscribe from an event
    fm_.unsubscribe(this, event);
}

void UserInterface::update(const std::string& event, const std::string& data) {
    // Do a particular method depending on what type of event is being updated
    if (event == "display_text") {
        handle_display_text(data);
    }
}

void UserInterface::handle_display_text(const std::string& data) {
    setText(data);
}

void UserInterface::OnClose(wxCloseEvent& event) {
    std::cout << "closed" << std::endl;
    event.Skip();
}
