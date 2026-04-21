#include <wx/wx.h>
#include <fmt/core.h>

class MyApp : public wxApp {
public:
    bool OnInit() override {
        fmt::print("App starting...\n");

        wxFrame* frame = new wxFrame(nullptr, wxID_ANY, "Hybrid wxWidgets App");
        frame->Show(true);

        return true;
    }
};

wxIMPLEMENT_APP(MyApp);