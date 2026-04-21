#include <wx/wx.h>
#include <fmt/core.h>




class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {

    fmt::print("App starting...\n");
    wxFrame* frame = new wxFrame(nullptr, wxID_ANY, "Hybrid wxWidgets App");
    frame->Show(true);

    return true;
}