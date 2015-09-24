#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define MAX_CHOICES 600

char *read_line(FILE *stream);

void show_choices(int start, int count, char* choices[], int is_selected[]);
void main_loop(char *choices[], int is_selected[], int num_choices);

int main(int argc, char *argv[])
{
	char *choices[MAX_CHOICES];
	int is_selected[MAX_CHOICES] = {0};
	int num_choices = 0;
	
	/* read choices */
	char *line;
	while ((line = read_line(stdin)) != NULL)
		choices[num_choices++] = line;

	freopen("/dev/tty", "r", stdin);
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	main_loop(choices, is_selected, num_choices);

	endwin();

	for (int i = 0; i < num_choices; i++)
		if (is_selected[i])
			fputs(choices[i], stderr);

	for (int i = 0; i < num_choices; i++)
		free(choices[i]);

	return 0;
}

void main_loop(char *choices[], int is_selected[], int num_choices)
{
	int ch;
	int pos = 0, last_pos = 0;
	int win_pos = 0, win_last_pos = 0;
	int scroll_off = 0;
	do {
		int max_y = getmaxy(stdscr);
		int choices_visible =
			num_choices <= max_y ? num_choices : max_y;

		show_choices(scroll_off, choices_visible, choices, is_selected);

		/* update cursor position highlighting */
		mvchgat(win_last_pos, 0, -1, A_NORMAL,  0, NULL);
		mvchgat(win_pos,      0, -1, A_REVERSE, 0, NULL);

		move(win_pos, 1);
		refresh();

		last_pos = pos;
		win_last_pos = last_pos - scroll_off;

		ch = getch();
		switch (ch) {
		case KEY_UP:
			pos--;
			if (pos < 0) pos = num_choices - 1;
			break;
		case KEY_DOWN:
			pos++;
			if (pos >= num_choices) pos = 0;
			break;
		case ' ':
			is_selected[pos] ^= 1;
			break;
		}

		win_pos = pos - scroll_off;

		if (pos < scroll_off)
			scroll_off = pos;
		else if (win_pos >= max_y)
			scroll_off += win_pos - (max_y - 1);
		/* recalculate after scrolling */
		win_pos = pos - scroll_off;
	} while (ch != 'q' && ch != '\n');
}

void show_choices(int start, int count, char* choices[], int is_selected[])
{
	int max_x = getmaxx(stdscr);

	for (int i = start; i < start + count; i++) {
		mvaddstr(i - start, 1, is_selected[i] ? "*" : " ");
		mvaddnstr(i - start, 3, choices[i], max_x - 3);
	}
	refresh();
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
