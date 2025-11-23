#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/box.h>
class MyWindow : public Gtk::Window
{
  const std::array<Glib::ustring, 16> valid_signs = {"%", "÷", "x", "-", "+", ".", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};

  Gtk::Box layout_box; 

  Gtk::Grid buttons_grid;
  Gtk::Button buttons[4][5];
  Gtk::Entry input_entry; 

  void insert_sign(Gtk::Button& button);
  void insert_valid_sign(Gtk::Button &button);

  void filter_input();

public:
  MyWindow();
};

#endif