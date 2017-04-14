#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disableRawMode(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode(void)
{
	/* Obtain the terminal settings and store it in a struct */
	tcgetattr(STDIN_FILENO, &orig_termios);

	/* Disable raw mode on exit */
	atexit(disableRawMode);

	struct termios raw = orig_termios;

	/* Disable echo and turn offcanonical mode */
	raw.c_lflag &= ~(ECHO | ICANON);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(void)
{
	enableRawMode();

	char c;

	/*
	 * 1. Read 1 byte into variable c and keep doing it. And press q
	 * to quit.
	 * 2. Print the character and it's ASCII value.
	 */
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
		if (iscntrl(c))
			printf("%d\n", c);
		else
			printf("%d ('%c')\n", c, c);
	}

	return 0;
}

