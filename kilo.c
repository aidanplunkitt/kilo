#include <ctype.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

struct termios orig_termios;

void enableRawMode();
void disableRawMode();

int main() {
    enableRawMode();

    char c;

    //read() and STDIN_FILENO come from unistd.h (POSIX API)
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if (iscntrl(c)) {
            printf("%d %x %c\n", c, c, c);
        } else {
            printf("%d %x ('%c')\n", c, c, c);
        }
    }

    return 0;
}

void enableRawMode() {
    //store struct retrieved from STDIN's file handler into orig_termios
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode); //stdlib

    //make a copy of current termios
    struct termios raw = orig_termios;

    //alter copy
    raw.c_lflag &= ~(ECHO | ICANON); //turn off echo and canonical mode

    //change current shell settings using altered copy
    //this is undone on exit using atexit(disableRawMode)
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

