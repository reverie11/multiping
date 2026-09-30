#ifndef COLORS_H
#define COLORS_H

// ANSI color/attribute macros for terminals
#define COLOR_RESET        "\033[0m"

// Standard 8 foreground colors
#define COLOR_FG_BLACK     "\033[30m"
#define COLOR_FG_RED       "\033[31m"
#define COLOR_FG_GREEN     "\033[32m"
#define COLOR_FG_YELLOW    "\033[33m"
#define COLOR_FG_BLUE      "\033[34m"
#define COLOR_FG_MAGENTA   "\033[35m"
#define COLOR_FG_CYAN      "\033[36m"
#define COLOR_FG_WHITE     "\033[37m"

// background colors (ligh on dark or dark on light)
#define COLOR_BG_BLACK     "\033[37m\033[100m"
#define COLOR_BG_RED       "\033[37m\033[101m"
#define COLOR_BG_GREEN     "\033[30m\033[102m"
#define COLOR_BG_YELLOW    "\033[30m\033[103m"
#define COLOR_BG_BLUE      "\033[37m\033[104m"
#define COLOR_BG_MAGENTA   "\033[37m\033[105m"
#define COLOR_BG_CYAN      "\033[30m\033[106m"
#define COLOR_BG_WHITE     "\033[30m\033[107m"

#endif // COLORS_H
