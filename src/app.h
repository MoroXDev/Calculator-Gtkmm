#ifndef WINDOW_H
#define WINDOW_H

// #include <gtkmm/window.h>
// #include <gtkmm/grid.h>
// #include <gtkmm/button.h>
// #include <gtkmm/entry.h>
// #include <gtkmm/box.h>
#include <vector>
#include <gtkmm.h>

class parser;

struct Insertion
{
  gunichar sign;
  int cursor_pos;
};

class MyWindow : public Gtk::Window
{
  Glib::RefPtr<Gtk::CssProvider> css_provider;
  int text_size_before_change = 0;
  static constexpr int grid_spacing = 10;
  const std::array<gunichar, 21> valid_signs = {'%', '-', '+', '.', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '/', '*', '^', '(', ')', ','};
  std::vector<Insertion> invalid_insertions;

  exprtk::parser<double> expr_parser;

  Gtk::Box upper_box;
  Gtk::Box bottom_box;
  Gtk::Box layout_box;

  Gtk::Grid buttons_grid;
  Gtk::Button buttons[5][5];
  Gtk::ToggleButton button_unsafe;
  Gtk::Entry input_entry;
  Gtk::Label history_label;
  sigc::connection input_entry_connection;

  void insert_sign(const gunichar& sign);
  void insert_sign_at(const gunichar &sign, int pos);
  void replace_sign_at(const gunichar &sign, int pos);
  void insert_text(const Glib::ustring &text);
  void insert_text_at(const Glib::ustring &text, int pos);
  void set_text(const Glib::ustring &text);
  void remove_sign(int pos);
  void manage_button_input_handler(Gtk::Button &button);

  void filter_input_handler();

  void expr_result_handler();

  void print_result(std::string expr_result);
  std::string calculate_expr_result();
  void append_history(std::string expr_result);

  void remove_or_change_sign();
public:
  MyWindow();
};

#endif