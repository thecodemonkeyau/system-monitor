#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  NCursesDisplay::Display(system, 15);  // default processes = 10
}