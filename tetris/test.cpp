#include <curses.h>

#include <clocale>
#include <cstdlib>

constexpr auto TEXT_RED = 1;     // Red text on black
constexpr auto TEXT_YELLOW = 2;  // Yellow text on black
constexpr auto TEXT_GREEN = 3;   // Green text on black
constexpr auto TEXT_BLUE = 4;    // Blue text on black
constexpr auto TEXT_CYAN = 5;    // Cyan text on black
constexpr auto TEXT_MAGENTA = 6; // Magenta text on black
constexpr auto TEXT_WHITE = 7;  // White text on black
constexpr auto TEXT_WHITE_BLACK = 10; // Black text on white
constexpr auto NO_OF_COLOR_PAIR = 8;

static void set_up_init_color_pair() {
    init_pair(TEXT_RED, COLOR_RED, COLOR_BLACK);
    init_pair(TEXT_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(TEXT_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(TEXT_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(TEXT_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(TEXT_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(TEXT_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(TEXT_WHITE_BLACK, COLOR_BLACK, COLOR_WHITE);
}

auto main(int argc, char **argv) -> int {
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  curs_set(0);

  if (!has_colors()) {
    printw("ERROR: Colors are not supported. Press any key to exit.");
    getch();
    endwin();
    return EXIT_FAILURE;
  }
  start_color();
  set_up_init_color_pair();

  WINDOW *stdwin = stdscr;
  wrefresh(stdwin);
  wrefresh(stdscr);
  wborder(stdwin, 0, 0, 0, 0, 0, 0, 0, 0);
  wprintw(stdwin, "Hello World █");

  getch();
  endwin();

  return EXIT_SUCCESS;
}