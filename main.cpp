#include <curses.h>
#include <stdlib.h>
#include <iostream>

#include "tetris/tetris.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <tetris|quit>\n";
        return 0;
    }

    std::string arg = argv[1];

    if (arg == "tetris") {
        run_tetris();
    } else if (arg == "quit") {
        // std::print("Quitting...\n");*
        std::cout << "Quitting...\n";
        return 0;
    } else {
        // std::print("Unknown option: {} \n", arg);
        // std::print("Usage: {} <tetris|quit>\n", arg);
        std::cout << "Unknown option: " << arg << "\n";
        std::cout << "Usage: " << argv[0] << " <tetris|quit>\n";
        return 1;
    }

    return 0;
}