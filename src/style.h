#include <string>
inline static std::string style = R"(
.button_unsafe {
  background: #D52941;
  color: #3F2A2B;
  font-size: larger;
  border-radius: 0px 2px 10px 2px;
  border-width: 2px;
  border-color: #3F2A2B;
  box-shadow: #0E402D 0px 4px 0px;
}

.button_unsafe:active {
  box-shadow: #0E402D 0px 2px 0px;
  transform: translateY(2px);
}

.button_unsafe:checked {
  background: #137547;
}

.button_unsafe:hover {
  filter: brightness(110%);
}

.app {
  background: #3F5E5A;
}

.grid_buttons {
  background: #BCAB79;
  color: #3F2A2B;
  font-size: larger;
  border-radius: 10px;
  border-width: 2px;
  border-color: #3F2A2B;
  box-shadow: #0E402D 0px 4px 0px;
}

.grid_buttons:hover {
  filter: brightness(110%);
}

.grid_buttons:active {
  transform:translateY(2px);
  box-shadow: #0E402D 0px 2px 0px;
}

.input_entry {
  background: #BCAB79;
  color: #3F2A2B;
  font-size: larger;
  border-width: 2px;
  border-color: #3F2A2B;
  box-shadow: #0E402D 0px 2px 0px;
}

.input_entry:hover {
  filter: brightness(110%);
}

.history_label {
 background: #0E402D; 
 border-radius: 3px;
}
)";