#include "app.h"
#include <ranges>
#include <iostream>
#include <algorithm>
#include "exprtk.hpp"

void MyWindow::insert_text(const Glib::ustring &text)
{
  int pos = input_entry.get_text_length();
  input_entry.insert_text(text, -1, pos);
  text_size_before_change = input_entry.get_text_length();
}

void MyWindow::insert_sign(const gunichar &sign)
{
  insert_text(Glib::ustring(1, sign));
  text_size_before_change = input_entry.get_text_length();
}

void MyWindow::set_text(const Glib::ustring &text)
{
  input_entry.set_text(text);
  text_size_before_change = input_entry.get_text_length();
}

void MyWindow::remove_sign(int pos)
{
  input_entry.delete_text(pos - 1, pos);
  text_size_before_change = input_entry.get_text_length();
}

void MyWindow::manage_button_input_handler(Gtk::Button &button)
{
  input_entry_connection.block();
  Glib::ustring sign_str = button.get_label();
  if (sign_str == "⌫")
  {
    int text_length = input_entry.get_text_length();
    input_entry.delete_text(std::max(0, text_length - 1), text_length);
    text_size_before_change = input_entry.get_text_length();
  }
  else if (sign_str == "=")
  {
    std::string expr_result = calculate_expr_result();
    append_history(expr_result);
    print_result(expr_result);
  }
  else if (sign_str == "AC")
  {
    set_text("");
  }
  else if (sign_str == "mod")
  {
    insert_text("mod(,)");
    //    input_entry.set_position(pos + mod_length - 1);
  }
  else if (sign_str == "√")
  {
    insert_text("root(,)");
  }
  else if (sign_str == "π")
  {
    insert_text("3.14");
  }
  else if (sign_str == "x²")
  {
    insert_text("pow(,)");
  }
  else if (sign_str == "logn")
  {
    insert_text("logn(,)");
  }
  else if (sign_str == "x")
  {
    insert_sign('*');
  }
  else if (sign_str.length() == 1)
  {
    // use of sign_str as single char
    if (std::ranges::find(valid_signs, sign_str[0]) != valid_signs.end())
      insert_text(button.get_label());
  }
  input_entry_connection.unblock();
}

void MyWindow::expr_result_handler()
{
  std::string expr_result = calculate_expr_result();
  append_history(expr_result);
  print_result(expr_result);
}

void MyWindow::print_result(std::string expr_result)
{
  if (expr_result == "nan")
    return;
  set_text(expr_result);
  input_entry.set_position(expr_result.size());
}

std::string MyWindow::calculate_expr_result()
{
  Glib::ustring expr_text = input_entry.get_text();
  int pos = 0;
  while ((pos = expr_text.find('%', pos)) != Glib::ustring::npos)
  {
    expr_text.erase(pos);
    insert_text("/100");
  }

  exprtk::expression<double> expr;
  expr_parser.compile(expr_text, expr);
  std::string result = std::to_string(expr.value());

  while (result.ends_with('0')) // remove zeros after '.'
    result.erase(result.end() - 1);
  if (result.ends_with('.'))
    result.erase(result.end() - 1);
  return result;
}

void MyWindow::append_history(std::string expr_result)
{
  if (history_label.get_text().length() == 0)
    history_label.set_text(history_label.get_text() + input_entry.get_text() + " = " + expr_result);
  else
    history_label.set_text(history_label.get_text() + "\n" + input_entry.get_text() + " = " + expr_result);
}

void MyWindow::filter_input_handler()
{
  if (button_unsafe.get_active())
    return;

  int cursor_pos = input_entry.get_position();
  gunichar sign = input_entry.get_text()[cursor_pos - 1];
  std::cout << "zmiana" << std::endl;
  if (text_size_before_change > input_entry.get_text_length())
  {
    // std::cout << "blocked " << char(sign) << std::endl;
    text_size_before_change = input_entry.get_text_length();
    return;
  }

  if (std::ranges::find(valid_signs, sign) == valid_signs.end())
  {
    std::cout << "before remove " << char(sign) << cursor_pos << std::endl;
    invalid_insertions.push_back({sign, cursor_pos});
    Glib::signal_idle().connect_once(sigc::mem_fun(*this, &MyWindow::remove_or_change_sign));
  }

  text_size_before_change = input_entry.get_text_length();
}

void MyWindow::remove_or_change_sign()
{
  input_entry_connection.block();

  std::cout << "after remove " << std::endl;
  for (auto ins = invalid_insertions.rbegin(); ins != invalid_insertions.rend(); ++ins)
  {
    if (input_entry.get_text()[ins->cursor_pos - 1] == ins->sign)
    {
      if (ins->sign == 'x')
      {
        int pos = ins->cursor_pos - 1;
        remove_sign(ins->cursor_pos);
        insert_text("*");
        input_entry.set_position(ins->cursor_pos);
      }
      else
        remove_sign(ins->cursor_pos);
    }
    invalid_insertions.clear();
  }

  input_entry_connection.unblock();
}

MyWindow::MyWindow() : layout_box(Gtk::Orientation::VERTICAL, 10)
{
  set_title("Calculator");
  set_default_size(300, 500);
  add_css_class("app");

  Glib::ustring signs[5][5] = {{"⌫", "7", "4", "1", "+\\-"}, {"AC", "8", "5", "2", "0"}, {"%", "9", "6", "3", "."}, {"/", "x", "-", "+", "logn"}, {"π", "√", "x²", "mod", "="}};

  buttons_grid.set_halign(Gtk::Align::CENTER);
  buttons_grid.set_row_spacing(grid_spacing);
  buttons_grid.set_column_spacing(grid_spacing);
  buttons_grid.add_css_class("buttons_grid");

  for (int x = 0; x < std::size(buttons); x++)
  {
    for (int y = 0; y < std::size(buttons[x]); y++)
    {
      buttons[x][y].set_label(signs[x][y]);
      buttons[x][y].signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &MyWindow::manage_button_input_handler), std::ref(buttons[x][y])));
      buttons_grid.attach(buttons[x][y], x, y, 1, 1);
      buttons[x][y].add_css_class("grid_buttons");
    }
  }

  button_unsafe.set_label("unsafe");
  button_unsafe.set_tooltip_text("turns off key filter");
  button_unsafe.add_css_class("button_unsafe");

  //  input_entry_connection = input_entry.signal_changed().connect(sigc::mem_fun(*this, &MyWindow::filter_input_once_handler));
  input_entry_connection = input_entry.signal_changed().connect(sigc::mem_fun(*this, &MyWindow::filter_input_handler));
  input_entry.signal_activate().connect(sigc::mem_fun(*this, &MyWindow::expr_result_handler));
  input_entry.set_margin_bottom(grid_spacing);
  input_entry.add_css_class("input_entry");

  history_label = Gtk::Label("", Gtk::Align::START, Gtk::Align::END, false);
  history_label.set_selectable(true);
  history_label.add_css_class("history_label");

  bottom_box = Gtk::Box(Gtk::Orientation::VERTICAL);
  bottom_box.set_valign(Gtk::Align::END);
  bottom_box.set_vexpand();

  upper_box = Gtk::Box(Gtk::Orientation::VERTICAL);
  upper_box.set_valign(Gtk::Align::START);
  upper_box.set_halign(Gtk::Align::START);
  upper_box.set_vexpand();
  // layout_box.set_valign(Gtk::Align::END);
  upper_box.append(button_unsafe);
  bottom_box.append(history_label);
  bottom_box.append(input_entry);
  bottom_box.append(buttons_grid);

  layout_box = Gtk::Box(Gtk::Orientation::VERTICAL);
  layout_box.append(upper_box);
  layout_box.append(bottom_box);

  set_child(layout_box);

  css_provider = Gtk::CssProvider::create();
  css_provider->load_from_path("src/style.css");
  Gtk::StyleProvider::add_provider_for_display(get_display(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
}