#include "window.h"
#include <ranges>
#include <iostream>
#include <algorithm>

void MyWindow::insert_sign(Gtk::Button &button)
{
  int pos = input_entry.get_text_length();
  input_entry.insert_text(button.get_label(), -1, pos);
}

void MyWindow::insert_valid_sign(Gtk::Button &button)
{
  Glib::ustring sign = button.get_label();
  if (std::ranges::find(valid_signs, sign) != valid_signs.end())
    insert_sign(button);
  else if (sign == "⌫")
  {
    int text_length = input_entry.get_text_length();
    input_entry.delete_text(std::max(0, text_length - 1), text_length);
  }
  else if (sign == "=")
  {
  }
  else if (sign == "AC")
  {
    input_entry.set_text("");
  }
}

void MyWindow::filter_input()
{
  Glib::ustring last_sign = Glib::ustring(1, input_entry.get_text()[input_entry.get_text_length() - 1]);
  if (last_sign == "⌫") return;
  if (std::ranges::find(valid_signs, last_sign) == valid_signs.end())
  {
    input_entry.delete_text(input_entry.get_text_length() - 1, input_entry.get_text_length());
    std::cout << "usunieto text" << std::endl;
  }
}

MyWindow::MyWindow() : layout_box(Gtk::Orientation::VERTICAL, 10)
{
  set_title("Calculator");
  set_default_size(300, 500);

  Glib::ustring signs[4][5] = {{"⌫", "7", "4", "1", "+\\-"}, {"AC", "8", "5", "2", "0"}, {"%", "9", "6", "3", "."}, {"÷", "x", "-", "+", "="}};

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
      buttons[x][y].signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &MyWindow::insert_valid_sign), std::ref(buttons[x][y])));
    }
  }

  input_entry.signal_changed().connect(sigc::mem_fun(*this, &MyWindow::filter_input));

  layout_box.set_valign(Gtk::Align::END);
  layout_box.append(input_entry);
  layout_box.append(buttons_grid);

  set_child(layout_box);
}