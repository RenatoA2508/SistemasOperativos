// desabilita buffering y echo en terminal

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    struct termios oldt, newt;

    // Get current terminal settings
    if (tcgetattr(STDIN_FILENO, &oldt) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    newt = oldt;

    // Disable canonical mode and echo
    newt.c_lflag &= ~(ICANON | ECHO );

    // Apply new terminal settings immediately
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    char c;
    write(STDOUT_FILENO, "Type something (press 'q' to quit):\n", 36);

    while (1) {
        ssize_t n = read(STDIN_FILENO, &c, 1);  // read one character (unbuffered)
        if (n > 0) {
	    c+=1;
            write(STDOUT_FILENO, &c, 1);        // write it immediately
            if (c == 'q') break;               // quit on 'q'
        }
    }

    // Restore original terminal settings
    if (tcsetattr(STDIN_FILENO, TCSANOW, &oldt) == -1) {
        perror("restore tcsetattr");
        exit(EXIT_FAILURE);
    }

    write(STDOUT_FILENO, "\nDone.\n", 7);
    return 0;
}

