#include "app.h"
#include <ranges>
#include <iostream>
#include <algorithm>

void MyWindow::insert_sign(Gtk::Button &button)
{
  int pos = input_entry.get_text_length();
  input_entry.insert_text(button.get_label(), -1, pos);
}

void MyWindow::manage_valid_signs_handler(Gtk::Button &button)
{
  input_entry_connection.block();
  Glib::ustring sign_str = button.get_label();
  if (sign_str == "⌫")
  {
    int text_length = input_entry.get_text_length();
    input_entry.delete_text(std::max(0, text_length - 1), text_length);
  }
  else if (sign_str == "=")
  {
    print_result();
  }
  else if (sign_str == "AC")
  {
    input_entry.set_text("");
  }

  if (sign_str.length() == 1)
  {
    // use of sign_str as single char
    if (std::ranges::find(valid_signs, sign_str[0]) != valid_signs.end())
      insert_sign(button);
  }
  input_entry_connection.unblock();
}

void MyWindow::filter_input_handler()
{
  int cursor_pos = input_entry.get_position();

  if (input_entry.get_text_length() == 0 || cursor_pos == 0)
  {
    input_entry_connection.unblock();
    return;
  }

  gunichar sign = input_entry.get_text()[cursor_pos - 1];

  if (std::ranges::find(valid_signs, sign) == valid_signs.end())
  {
    invalid_insertions.push_back({sign, cursor_pos});
    Glib::signal_idle().connect_once(sigc::mem_fun(*this, &MyWindow::remove_or_change_sign));
  }
}

void MyWindow::print_result()
{
  exprtk::expression<double> expr;
  expr_parser.compile(input_entry.get_text(), expr);
  result_label.set_text(std::to_string(expr.value()));
}

void MyWindow::remove_or_change_sign()
{
  input_entry_connection.block();
  for (auto &ins : invalid_insertions)
  {
    if (input_entry.get_text()[ins.cursor_pos - 1] == ins.sign)
    {
      if (ins.sign == 'x')
      {
        int pos = ins.cursor_pos - 1;
        input_entry.delete_text(ins.cursor_pos - 1, ins.cursor_pos);
        input_entry.insert_text("*", 1, pos);
        input_entry.set_position(ins.cursor_pos);
      }
      else
        input_entry.delete_text(ins.cursor_pos - 1, ins.sign);
    }
  }
  input_entry_connection.unblock();
}

MyWindow::MyWindow() : layout_box(Gtk::Orientation::VERTICAL, 10)
{
  set_title("Calculator");
  set_default_size(300, 500);

  Glib::ustring signs[4][5] = {{"⌫", "7", "4", "1", "+\\-"}, {"AC", "8", "5", "2", "0"}, {"%", "9", "6", "3", "."}, {"/", "x", "-", "+", "="}};

  buttons_grid.set_row_spacing(10);
  buttons_grid.set_column_spacing(10);
  buttons_grid.set_valign(Gtk::Align::END);
  buttons_grid.set_halign(Gtk::Align::CENTER);

  for (int x = 0; x < std::size(buttons); x++)
  {
    for (int y = 0; y < std::size(buttons[x]); y++)
    {
      buttons[x][y].set_label(signs[x][y]);
      buttons_grid.attach(buttons[x][y], x, y, 1, 1);
      buttons[x][y].signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &MyWindow::manage_valid_signs_handler), std::ref(buttons[x][y])));
    }
  }

  //  input_entry_connection = input_entry.signal_changed().connect(sigc::mem_fun(*this, &MyWindow::filter_input_once_handler));
  input_entry.signal_changed().connect(sigc::mem_fun(*this, &MyWindow::filter_input_handler));
  input_entry.signal_activate().connect(sigc::mem_fun(*this, &MyWindow::print_result));

  result_label = Gtk::Label("", Gtk::Align::END, Gtk::Align::END, false);

  layout_box.set_valign(Gtk::Align::END);
  layout_box.append(result_label);
  layout_box.append(input_entry);
  layout_box.append(buttons_grid);

  set_child(layout_box);
}