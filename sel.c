#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

char *read_line(FILE *stream);

int main(int argc, char *argv[])
{
	char *line;
	char *lines[20];
	int is_selected[20] = {0};
	int num_lines = 0;
	int ch;
	int pos = 0, last_pos = 0;
	
	// read menu entries
	while ((line = read_line(stdin)) != NULL)
		lines[num_lines++] = line;

	freopen("/dev/tty", "r", stdin);
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	for (int i = 0; i < num_lines; i++)
		printw("   %s", lines[i]);
	refresh();

	do {
		mvchgat(last_pos, 0, -1, A_NORMAL, 0, NULL);
		mvchgat(pos, 0, -1, A_REVERSE, 0, NULL);
		move(pos, 1);
		refresh();

		last_pos = pos;

		ch = getch();
		switch (ch) {
		case KEY_UP:
			pos--;
			if (pos < 0) pos = num_lines - 1;
			break;
		case KEY_DOWN:
			pos++;
			if (pos >= num_lines) pos = 0;
			break;
		case ' ':
			is_selected[pos] = 1 - is_selected[pos];
			mvprintw(pos, 1, is_selected[pos] ? "*" : " ");
			break;
		}
	} while (ch != 'q' && ch != '\n');

	endwin();

	for (int i = 0; i < num_lines; i++)
		if (is_selected[i])
			fputs(lines[i], stderr);

	for (int i = 0; i < num_lines; i++)
		free(lines[i]);

	return 0;
}

char *read_line(FILE *stream)
{
	char *line = NULL;
	size_t buffer_len = 0;
	
	ssize_t len = getline(&line, &buffer_len, stream);

	if (len == -1) {
		free(line);
		return NULL;
	} else {
		return line;
	}
}
