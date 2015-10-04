#define _POSIX_C_SOURCE 200809L

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ncurses.h>

#include "dyn_arr.h"


char *read_line(FILE *stream);

void show_choices(int start, int count,
		  const struct p_dyn_arr *choices,
		  const struct dyn_arr *is_selected);
void select_choices(struct p_dyn_arr *choices, struct dyn_arr *is_selected);

int main(int argc, char *argv[])
{
	struct p_dyn_arr choices = {0};
	struct dyn_arr is_selected = { sizeof(bool) };

	/* read choices */
	char *line;
	bool initial_selected = false;
	while ((line = read_line(stdin)) != NULL) {
		p_dyn_arr_append(&choices, line);

		dyn_arr_append(&is_selected, &initial_selected);
	}

	setlocale(LC_CTYPE, "");
	FILE *term = fopen("/dev/tty", "r+");
	SCREEN *scr = newterm(NULL, term, term);
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	select_choices(&choices, &is_selected);

	endwin();

	/* write selected choices */
	for (int i = 0; i < choices.count; i++) {
		bool is_sel = *(bool *)dyn_arr_get(&is_selected, i);

		if (is_sel)
			fputs(p_dyn_arr_get(&choices, i), stdout);
	}

	for (int i = 0; i < choices.count; i++)
		free(p_dyn_arr_get(&choices, i));
	p_dyn_arr_clear(&choices);
	dyn_arr_clear(&is_selected);

	return 0;
}

void select_choices(struct p_dyn_arr *choices, struct dyn_arr *is_selected)
{
	int ch;
	int pos = 0, last_pos = 0;
	int win_pos = 0, win_last_pos = 0;
	int scroll_off = 0;
	do {
		int max_y = getmaxy(stdscr);
		int choices_visible =
			choices->count <= max_y ? choices->count : max_y;

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
			if (pos < 0) pos = choices->count - 1;
			break;
		case KEY_DOWN:
			pos++;
			if (pos >= choices->count) pos = 0;
			break;
		case ' ':
			*(bool *)dyn_arr_get(is_selected, pos) ^= 1;
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

void show_choices(int start, int count,
		  const struct p_dyn_arr *choices,
		  const struct dyn_arr *is_selected)
{
	int max_x = getmaxx(stdscr);

	for (int i = start; i < start + count; i++) {
		bool is_sel = *(bool *)dyn_arr_get(is_selected, i);
		mvaddstr(i - start, 1, is_sel ? "*" : " ");
		mvaddnstr(i - start, 3,
			  p_dyn_arr_get(choices, i),
			  max_x - 3);
	}
	/* screen refresh to be done elsewere */
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
