#include <ctime>
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <array>

constexpr auto TEXT_RED = 1;     // Red text on black
constexpr auto TEXT_YELLOW = 2;  // Yellow text on black
constexpr auto TEXT_GREEN = 3;   // Green text on black
constexpr auto TEXT_BLUE = 4;    // Blue text on black
constexpr auto TEXT_CYAN = 5;    // Cyan text on black
constexpr auto TEXT_MAGENTA = 6; // Magenta text on black
constexpr auto TEXT_WHITE = 7;  // White text on black
constexpr auto TEXT_WHITE_BLACK = 10; // Black text on white
constexpr auto NO_OF_COLOR_PAIR = 8;

// Block layout: {w-1, h-1}{x0,y0}{x1,y1}{x2,y2}{x3,y3}
// packed integer encoding the coordinates
int x  = 431424;
int y  = 598356;
int r  = 427089;
int px = 247872;
int py = 799248;
int pr = 0;
int c  = 348480;
int p  = 615696;

std::array<std::array<int, 4>, 7> block = {{
    {x, y, x, y},
    {r, p, r, p},
    {c, c, c, c},
    {599636, 431376, 598336, 432192},
    {411985, 610832, 415808, 595540},
    {px, py, px, py},
    {614928, 399424, 615744, 428369}
}};

// Global game state
int tick = 0;
int score = 0;
std::array<std::array<int, 10>, 20> board{};

// extract a 2-bit number from a block entry
static int extract_number(int x, int y) { 
    return 3 & block[p][x] >> y; 
}

static void generate_new_piece() {
  y = py = 0;
  p = rand() % 7; // pick random tetromino
  r = pr = rand() % 4; // pick random rotation
  x = px = rand() % (10 - extract_number(r, 16)); // starting x
}

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

static void set_piece(int x, int y, int r, int v) {
    for (int i = 0; i < NO_OF_COLOR_PAIR; i += 2) {
        board[extract_number(r, i * 2) + y][extract_number(r, (i * 2) + 2) + x] = v;
    }
}

static void update_piece() {
    set_piece(px, py, pr, 0);
    set_piece(px = x, py = y, pr = r, p + 1);
}

static int check_hit(int x, int y, int r) {
  if (y + extract_number(r, 18) > 19) {
    return 1;
  }
  set_piece(px, py, pr, 0);
  c = 0;
  for (int i = 0; i < NO_OF_COLOR_PAIR; i += 2) {
    board[y + extract_number(r, i * 2)][x + extract_number(r, (i * 2) + 2)] && c++;
  }
  set_piece(px, py, pr, p + 1);
  return c;
}

static void remove_line() {
  for (int row = y; row <= y + extract_number(r, 18); row++) {
    c = 1;
    for (int i = 0; i < 10; i++) {
      c *= board[row][i];
    }
    if (!c) {
      continue;
    }
    for (int i = row - 1; i > 0; i--) {
      memcpy(&board[i + 1][0], &board[i][0], 40);
    }
    memset(&board[0][0], 0, 10);
    score++;
  }
}

static int tick_new_piece() {
    if (++tick > 30)
    {
        tick = 0;
        if (check_hit(x, y + 1, r)) {
            if(!y) return 0;
            remove_line();
            generate_new_piece();
        }
        else
        {
            y++;
            update_piece();
        }
    }

    return 1;
}

static void print_board_and_score() {
  for (int i = 0; i < 20; i++) {
    move(1 + i, 1); // otherwise the box won't draw
    for (int j = 0; j < 10; j++) {
      board[i][j] && attron(262176 | board[i][j] << 8);
      printw("  ");
      attroff(262176 | board[i][j] << 8);
    }
  }
  move(21, 1);
  printw("Score: %d", score);
  refresh();
}

static void execute_loop() {
    while(tick_new_piece()) {
        usleep(10000); // 10,000 microseconds = 0.01 seconds
        if ((c = getch()) == 'a' && x > 0 && !check_hit(x - 1, y, r)) {
            x--;
        } // left
        if (c == 'd' && x + extract_number(r, 16) < 9 && !check_hit(x + 1, y, r)) {
            x++;
        } // right
        if (c == 's') {
            while (!check_hit(x, y + 1, r)) {
                y++;
                update_piece();
            }
            remove_line();
            generate_new_piece();
        } // drop
        if (c == 'w') {
            ++r %= 4;
            while (x + extract_number(r, 16) > 9) {
                x--;
            }
            if (check_hit(x, y, r)) {
                x = px;
                r = pr;
            }
        } // rotate
        if (c == 'q') {
            return;
        } // quit
        update_piece();
        print_board_and_score();
    }
}

void run_tetris() {
  srand(time(0));

  initscr(); // initialize curses
  start_color();

  set_up_init_color_pair();
  generate_new_piece();
  resizeterm(22, 22);
  noecho(); // disable automatic echo
  timeout(0);
  curs_set(0);
  box(stdscr, 0, 0);

  execute_loop();

  endwin();
}