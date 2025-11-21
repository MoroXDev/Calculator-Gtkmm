#include <gtkmm.h>

class MyWindow : public Gtk::Window
{
public:
  MyWindow();
  
};

MyWindow::MyWindow()
{
  set_title("Basic application");
  set_default_size(300, 500);
}

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("org.gtkmm.examples.base");
 
  Gtk::Button button;
    

  Gtk::Box box;
  box.append(button);
  Gtk::Frame frame;
  frame.set_child(box);
  return app->make_window_and_run<MyWindow>(argc, argv);
}

