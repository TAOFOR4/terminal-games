cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    copts = ["-std=c++23", "-O2", "-Wall"],
    deps = [
        "@ncurses//:ncurses_headers",
        "@ncurses",
        "//tetris:tetris_if"
    ]
)