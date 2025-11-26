#ifndef WINDOW_H
#define WINDOW_H

// #include <gtkmm/window.h>
// #include <gtkmm/grid.h>
// #include <gtkmm/button.h>
// #include <gtkmm/entry.h>
// #include <gtkmm/box.h>
#include <vector>
#include <gtkmm.h>
#include "exprtk.hpp"

struct Insertion
{
  gunichar sign;
  int cursor_pos;
};

class MyWindow : public Gtk::Window
{
  const std::array<gunichar, 18> valid_signs = {'%', '-', '+', '.', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '/', '*', '^'};
  std::vector<Insertion> invalid_insertions;  

  exprtk::parser<double> expr_parser;

  Gtk::Box layout_box; 

  Gtk::Grid buttons_grid;
  Gtk::Button buttons[4][5];
  Gtk::Entry input_entry;
  Gtk::Label result_label;
  sigc::connection input_entry_connection;

  void insert_sign(Gtk::Button& button);
  void manage_valid_signs_handler(Gtk::Button &button);

  void filter_input_handler();

  void print_result();

  void remove_or_change_sign();

  Glib::ustring make_expression_valid(Glib::ustring expr);
public:
  MyWindow();
};

#endif