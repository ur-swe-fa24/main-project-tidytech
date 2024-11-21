#ifndef UI_SENIOR_MANAGER_PAGE
#define UI_SENIOR_MANAGER_PAGE

#include <wx/wx.h>
 
class SeniorManagerPage : public wxFrame {
    public:
        SeniorManagerPage(const wxString& title);
    private:
        void OnButtonClick(wxCommandEvent& evt);
        void OnSeniorManagerClick(wxCommandEvent& evt);
};

#endif