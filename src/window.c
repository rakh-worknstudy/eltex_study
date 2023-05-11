#include "window.h"
#include <ncurses.h>
#include <malloc.h>
#include <stdio.h>
#include <time.h>

//	================================================================
//	                            DIRENT
//	================================================================
/* DIRENT INIT */
void dirent_init(void) {
	/* LEFT */
	path_left[0] = '/';
	path_left[1] = 0;
	dirlist_left = dirlist_make(path_left);
	/* RIGHT */
	path_right[0] = '/';
	path_right[1] = 0;
	dirlist_right = dirlist_make(path_right);
}
/* DIRENT MOVE UP */
void dirent_out(void) {
	char delim_last;
	short iter;
	switch (window_side) {
	case 0:
		/* LEFT */
		for (iter = 0; path_left[iter]; ++iter);
		if (iter <= 2)
			return;
		for (iter -= 2; path_left[iter] != '/'; --iter);
		path_left[iter + 1] = 0;
		dirlist_free(dirlist_left);
		dirlist_left = dirlist_make(path_left);
		page_fill(pageinfo_left, page_size,  dirlist_left);
		page_left_size = page_fill_last;
		pos_left = 0;
		pos_left_long = 0;
		need_redraw = 1;
		return;
	case 1:
		/* RIGHT */
		for (iter = 0; path_right[iter]; ++iter);
		if (iter <= 2)
			return;
		for (iter -= 2; path_right[iter] != '/'; --iter);
		path_right[iter + 1] = 0;
		dirlist_free(dirlist_right);
		dirlist_right = dirlist_make(path_right);
		page_fill(pageinfo_right, page_size, dirlist_right);
		page_right_size = page_fill_last;
		pos_right = 0;
		pos_right_long = 0;
		need_redraw = 1;
		return;
	default:
		return;
	}
}
/* DIRENT MOVE IN */
void dirent_in(void) {
	short iter, pos_iter;
	switch (window_side) {
	case 0:
		if (pageinfo_left[pos_left].type == -1)
			return;
		if (pageinfo_left[pos_left].name[0] == '.' && pageinfo_left[pos_left].name[1] == '.') {
			dirent_out();
			return;
		}
		if (pageinfo_left[pos_left].type == DT_DIR) {
			/* LEFT */
			for (iter = 0; path_left[iter]; ++iter)
				continue;
			//	path_left[iter++] = '/';
			for (pos_iter = 0; pageinfo_left[pos_left].name[pos_iter]; ++pos_iter)
				path_left[iter + pos_iter] = pageinfo_left[pos_left].name[pos_iter];
			path_left[iter + pos_iter++] = '/';
			path_left[iter + pos_iter] = 0;
			dirlist_free(dirlist_left);
			dirlist_left = dirlist_make(path_left);
			page_fill(pageinfo_left, page_size,  dirlist_left);
			page_left_size = page_fill_last;
			need_redraw = 1;
			pos_left = 0;
			pos_left_long = 0;
		}
		return;
	case 1:
		/* RIGHT */
		if (pageinfo_right[pos_right].type == -1)
			return;
		if (pageinfo_right[pos_right].name[0] == '.' && pageinfo_right[pos_right].name[1] == '.') {
			dirent_out();
			return;
		}
		if (pageinfo_right[pos_right].type == DT_DIR) {
			/* LEFT */
			for (iter = 0; path_right[iter]; ++iter)
				continue;
			//	path_left[iter++] = '/';
			for (pos_iter = 0; pageinfo_right[pos_right].name[pos_iter]; ++pos_iter)
				path_right[iter + pos_iter] = pageinfo_right[pos_right].name[pos_iter];
			path_right[iter + pos_iter++] = '/';
			path_right[iter + pos_iter] = 0;
			dirlist_free(dirlist_right);
			dirlist_right = dirlist_make(path_right);
			page_fill(pageinfo_right, page_size, dirlist_right);
			page_right_size = page_fill_last;
			need_redraw = 1;
			pos_right = 0;
			pos_right_long = 0;
		}
		return;
	default:
		return;
	}	
}


//	================================================================
//	                            LENGTH
//	================================================================
void length_calc(void) {
	length_size = (short)((double)COLS * 0.1);
	if (length_size > 7)
		length_size = 7;
	length_edit = (short)((double)COLS * 0.15);
	if (length_edit > 12)
		length_edit = 12;
	length_name = COLS/2 - length_size - length_edit - 4;
}

//	================================================================
//	                            COLORS
//	================================================================
void colors_pairs_init(void) {
	start_color();
	init_pair(1, COLOR_WHITE,  COLOR_BLUE);
	init_pair(2, COLOR_BLACK,  COLOR_CYAN);
	init_pair(3, COLOR_BLACK,  COLOR_WHITE);
	init_pair(4, COLOR_YELLOW, COLOR_BLUE);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
}

//	================================================================
//	                             DRAW
//	================================================================
/* TOP BAR */
void draw_line_top(void) {
	attron(COLOR_PAIR(2));
	const char bar[5][10] = {"Left", "File", "Command", "Options", "Right"};
	char here = 0, next = 0, iter = 0;
	x = 0;
	mvaddch(0, x, ' ');
	++x;
	mvaddch(0, x, ' ');
	++x;
	while (here < 5) {
		if (bar[here][iter] != '\0') {
			mvaddch(0, x, bar[here][iter]);
			iter++;
			++x;
		} else {
			for (iter = 4; iter; --iter) {
				mvaddch(0, x, ' ');
				++x;
			}
			++here;
		}
	}
	for (; x < COLS; ++x)
		mvaddch(0, x, ' ');
}
//	================================================================
/* CURRENT DIR PATH */
void draw_line_path(void) {
	attron(COLOR_PAIR(1));
	for (x = 0; x < COLS; ++x) {
		if (x == 0 || x == COLS/2) {
			mvaddch(1, x, ACS_ULCORNER);
			continue;
		}
		if (x == 1 || x == COLS/2 + 1) {
			mvaddch(1, x, '<');
			continue;
		}
		if (x == COLS/2 - 3 || x == COLS - 3) {
			mvaddch(1, x, 'v');
			continue;
		}
		if (x == COLS/2 - 2 || x == COLS - 2) {
			mvaddch(1, x, '>');
			continue;
		}
		if (x == COLS/2 - 1 || x == COLS - 1) {
			mvaddch(1, x, ACS_URCORNER);
			continue;
		}
		if (window_side) {
			if (x == 3) {
				short i = 0;
				while (path_left[i] && x < COLS/2 - 3) {
					mvaddch(1, x, path_left[i]);
					++x;
					++i;
				}
				--x;
				continue;
			}
			if (x == COLS/2 + 3) {
				attron(COLOR_PAIR(3));
				short i = 0;
				while (path_right[i] && x < COLS - 3) {
					mvaddch(1, x, path_right[i]);
					++x;
					++i;
				}
				attron(COLOR_PAIR(1));
				--x;
				continue;
			}
		} else {
			if (x == 3) {
				attron(COLOR_PAIR(3));
				short i = 0;
				while (path_left[i] && x < COLS/2 - 3) {
					mvaddch(1, x, path_left[i]);
					++x;
					++i;
				}
				attron(COLOR_PAIR(1));
				mvaddch(1, x, ACS_HLINE);
				continue;
			}
			if (x == COLS/2 + 3) {
				short i = 0;
				while (path_right[i] && x < COLS - 3) {
					mvaddch(1, x, path_right[i]);
					++x;
					++i;
				}
				mvaddch(1, x, ACS_HLINE);
				continue;
			}
		}
		mvaddch(1, x, ACS_HLINE);
	}
}
//	================================================================
/* COLUMNS NAMES */
void draw_line_cols(void) {
	const char str_name[] = "Name ";
	const char str_size[] = "Size ";
	const char str_edit[] = "Modify ";
	for (x = 1; x <= length_name/2 - 2; ++x) {
		mvaddch(2, x, ' ');
		mvaddch(2, x + COLS/2, ' ');
	}
	attron(COLOR_PAIR(4));
	attron(A_BOLD);
	for (; x <= length_name/2 + 2; ++x) {
		mvaddch(2, x, str_name[x - length_name/2 + 1]);
		mvaddch(2, x + COLS/2, str_name[x - length_name/2 + 1]);
	}
	attron(COLOR_PAIR(1));
	attroff(A_BOLD);
	for (; x <= length_name; ++x) {
		mvaddch(2, x, ' ');
		mvaddch(2, x + COLS/2, ' ');
	}
	mvaddch(2, x, ACS_VLINE);
	mvaddch(2, x + COLS/2, ACS_VLINE);
	for (++x; x <= length_name + length_size/2 - 1; ++x) {
		mvaddch(2, x, ' ');
		mvaddch(2, x + COLS/2, ' ');
	}
	attron(COLOR_PAIR(4));
	attron(A_BOLD);
	for (; x <= length_name + length_size/2 + 3; ++x) {
		mvaddch(2, x, str_size[x - length_name - length_size/2]);
		mvaddch(2, x + COLS/2, str_size[x - length_name - length_size/2]);
	}
	attron(COLOR_PAIR(1));
	attroff(A_BOLD);
	for (; x <= length_name + length_size + 1; ++x) {
		mvaddch(2, x, ' ');
		mvaddch(2, x + COLS/2, ' ');
	}
	mvaddch(2, x, ACS_VLINE);
	mvaddch(2, x + COLS/2, ACS_VLINE);
	for (++x; x <= length_name + length_size + length_edit/2 - 1; ++x) {
		mvaddch(2, x, ' ');
		mvaddch(2, x + COLS/2, ' ');
	}
	attron(COLOR_PAIR(4));
	attron(A_BOLD);
	for (; x <= length_name + length_size + length_edit/2 + 5; ++x) {
		mvaddch(2, x, str_size[x - length_name - length_size - length_edit/2 + 6]);
		mvaddch(2, x + COLS/2, str_size[x - length_name - length_size - length_edit/2 + 6]);
	}
	attron(COLOR_PAIR(1));
	attroff(A_BOLD);
	for (; x <= COLS/2 - 1; ++x) {
		mvaddch(2, x, ' ');
		mvaddch(2, x + COLS/2, ' ');
	}
}
//	================================================================
void draw_border_left(void);
void draw_border_right(void);
// void draw_delim(void);
void draw_dirent(void);
void draw_main(void) {
	attron(COLOR_PAIR(1));
	for (y = 2; y < LINES - 3; ++y) {
		for (x = 0; x < COLS; ++x) {
			/* LEFT BORDER */
			if (x == 0 ||x == COLS/2) {
				draw_border_left();
				continue;
			}
			/* RIGHT BORDER */
			if (x == COLS/2 - 1 || x == COLS - 1) {
				draw_border_right();
				continue;
			}
			/* CENTRAL & BOTTOM BORDERS */
			if (y == LINES - 6 || y == LINES - 4) {
				mvaddch(y, x, ACS_HLINE);
				continue;
			}
			if (x == length_name + 1 || x == length_name + length_size + 2) {
				draw_border_left();
				continue;
			}
			if (x == COLS/2 + length_name + 1 || x == COLS/2 + length_name + length_size + 2) {
				draw_border_right();
				continue;
			}
		}
	}
}
void draw_border_left(void) {
	if (y == 1) {
		mvaddch(y, x, ACS_ULCORNER);
		return;
	}
	if (y == LINES - 6) {
		mvaddch(y, x, ACS_LTEE);
		return;
	}
	if (y == LINES - 4) {
		mvaddch(y, x, ACS_LLCORNER);
		return;
	}
	mvaddch(y, x, ACS_VLINE);
}
void draw_border_right(void) {
	if (y == 1) {
		mvaddch(y, x, ACS_URCORNER);
		return;
	}
	if (y == LINES - 6) {
		mvaddch(y, x, ACS_RTEE);
		return;
	}
	if (y == LINES - 4) {
		mvaddch(y, x, ACS_LRCORNER);
		return;
	}
	mvaddch(y, x, ACS_VLINE);	
}
void draw_dirent_name_left(char iter) {
	for (x = 2; x < length_name + 1; ++x) {
		if (pageinfo_left[iter].name[x - 2])
			mvaddch(3 + iter, x, pageinfo_left[iter].name[x - 2]);
		else
			break;
	}
	for (; x < length_name + 1; ++x)
		mvaddch(3 + iter, x, ' ');
	if (iter == pos_left) {
		attroff(COLOR_PAIR(2));
		attron(COLOR_PAIR(1));
		if (pageinfo_left[iter].type == DT_DIR)
			mvaddch(4 + page_size, 1, '/');
		else
			mvaddch(4 + page_size, 1, '.');
		for (x = 2; x < COLS/2 - 1; ++x) {
			if (pageinfo_left[iter].name[x - 2])
				mvaddch(4 + page_size, x, pageinfo_left[iter].name[x - 2]);
			else
				break;
		}
		for (; x < COLS/2 - 1; ++x)
			mvaddch(4 + page_size, x, ' ');
		attroff(COLOR_PAIR(1));
		attron(COLOR_PAIR(2));
	}
}
void draw_dirent_size_left(char iter) {
	char reclen[16];
	char len = sprintf(reclen, "%-lu", pageinfo_left[iter].size);
	char moretempgarbage;
	for (moretempgarbage = 0; len < length_size; ++moretempgarbage, ++len)
		mvaddch(3 + iter, moretempgarbage + length_name + 2, ' ');
	for (len = 0; moretempgarbage < length_size; ++moretempgarbage, ++len)
		mvaddch(3 + iter, moretempgarbage + length_name + 2, reclen[len]);
}
void draw_dirent_time_left(char iter) {
	char reclen[32];
	char len = sprintf(reclen, "%-32s", ctime(&pageinfo_left[iter].time)) - 4;
	char moretempgarbage;
	for (moretempgarbage = 0; len < length_edit; ++moretempgarbage, ++len)
		mvaddch(3 + iter, moretempgarbage + length_name + length_size + 3, ' ');
	for (len = 0; moretempgarbage < length_edit; ++moretempgarbage, ++len)
		mvaddch(3 + iter, moretempgarbage + length_name + length_size + 3, reclen[len + 4]);
}

void draw_dirent_left(char iter) {
	if (pageinfo_left[iter].type == -1) {
		for (x = 1; x < COLS/2 - 1; ++x) {
			if (x == length_name + 1 || x == length_name + length_size + 2)
				continue;
			mvaddch(3 + iter, x, ' ');
		}
		mvaddch(3 + iter, x, ACS_VLINE);
	} else {
		if (iter == pos_left) {
			attroff(COLOR_PAIR(1));
			attron(COLOR_PAIR(2));
		}
		if (pageinfo_left[iter].type == DT_DIR) {
			attron(A_BOLD);
			mvaddch(3 + iter, 1, '/');
		} else {
			mvaddch(3 + iter, 1, '.');
		}
		draw_dirent_name_left(iter);
		draw_dirent_size_left(iter);
		draw_dirent_time_left(iter);
		if (iter == pos_left) {
			attroff(COLOR_PAIR(2));
			attron(COLOR_PAIR(1));
		}
		if (pageinfo_left[iter].type == DT_DIR)
			attroff(A_BOLD);
	}
}

void draw_dirent_name_right(char iter) {
	for (x = COLS/2 + 2; x < COLS/2 + length_name + 1; ++x) {
		if (pageinfo_right[iter].name[x - 2 - COLS/2])
			mvaddch(3 + iter, x, pageinfo_right[iter].name[x - 2 - COLS/2]);
		else
			break;
	}
	for (; x < COLS/2 + length_name + 1; ++x)
		mvaddch(3 + iter, x, ' ');
	if (iter == pos_right) {
		attroff(COLOR_PAIR(2));
		attron(COLOR_PAIR(1));
		if (pageinfo_right[iter].type == DT_DIR)
			mvaddch(4 + page_size, COLS/2 + 1, '/');
		else
			mvaddch(4 + page_size, COLS/2 + 1, '.');
		for (x = COLS/2 + 2; x < COLS - 1; ++x) {
			if (pageinfo_right[iter].name[x - 2 - COLS/2])
				mvaddch(4 + page_size, x, pageinfo_right[iter].name[x - 2 - COLS/2]);
			else
				break;
		}
		for (; x < COLS - 1; ++x)
			mvaddch(4 + page_size, x, ' ');
		attroff(COLOR_PAIR(1));
		attron(COLOR_PAIR(2));
	}
}
void draw_dirent_size_right(char iter) {
	char reclen[16];
	char len = sprintf(reclen, "%-lu", pageinfo_right[iter].size);
	char moretempgarbage;
	for (moretempgarbage = 0; len < length_size; ++moretempgarbage, ++len)
		mvaddch(3 + iter, COLS/2 + moretempgarbage + length_name + 2, ' ');
	for (len = 0; moretempgarbage < length_size; ++moretempgarbage, ++len)
		mvaddch(3 + iter, COLS/2 + moretempgarbage + length_name + 2, reclen[len]);
}
void draw_dirent_time_right(char iter) {
	char reclen[32];
	char len = sprintf(reclen, "%-32s", ctime(&pageinfo_right[iter].time)) - 4;
	char moretempgarbage;
	for (moretempgarbage = 0; len < length_edit; ++moretempgarbage, ++len)
		mvaddch(3 + iter, COLS/2 + moretempgarbage + length_name + length_size + 3, ' ');
	for (len = 0; moretempgarbage < length_edit; ++moretempgarbage, ++len)
		mvaddch(3 + iter, COLS/2 + moretempgarbage + length_name + length_size + 3, reclen[len + 4]);
}

void draw_dirent_right(char iter) {
	if (pageinfo_right[iter].type == -1) {
		for (x = COLS/2 + 1; x < COLS - 1; ++x) {
			if (x == COLS/2 + length_name + 1 || x == COLS/2 + length_name + length_size + 2)
				continue;
			mvaddch(3 + iter, x, ' ');
		}
		mvaddch(3 + iter, x, ACS_VLINE);
	} else {
		if (iter == pos_right) {
			attroff(COLOR_PAIR(1));
			attron(COLOR_PAIR(2));
		}
		if (pageinfo_right[iter].type == DT_DIR) {
			attron(A_BOLD);
			mvaddch(3 + iter, COLS/2 + 1, '/');
		} else {
			mvaddch(3 + iter, COLS/2 + 1, '.');
		}
		draw_dirent_name_right(iter);
		draw_dirent_size_right(iter);
		draw_dirent_time_right(iter);
		if (iter == pos_right) {
			attroff(COLOR_PAIR(2));
			attron(COLOR_PAIR(1));
		}
		if (pageinfo_right[iter].type == DT_DIR)
			attroff(A_BOLD);
	}	
}

void draw_dirent(void) {
	for (char iter = 0; iter < page_size; ++iter) {
		draw_dirent_left(iter);
		draw_dirent_right(iter);
	}
	
}
//	================================================================
void draw_console(void) {
	attron(COLOR_PAIR(5));
	for (x = 0; x < COLS; ++x) {
		mvaddch(LINES - 3, x, ' ');
		mvaddch(LINES - 2, x, ' ');
	}
	attron(COLOR_PAIR(1));
}
void draw_line_bottom(void) {
	const char str_help[] = "  W/S - MOVE UP/DOWN | D - JUMP IN | TAB - SWITCH SIDE | CONSOLE - OFF | X - SAFE EXIT (NO CTRL+C CATCH YET)  ";
	attron(COLOR_PAIR(2));
	for (x = 0; x < COLS; ++x) {
		if (str_help[x])
			mvaddch(LINES - 1, x, str_help[x]);
		else
			break;
	}
	for (; x < COLS; ++x)
		mvaddch(LINES - 1, x, ' ');
	attron(COLOR_PAIR(1));
	
}

//	================================================================
//	                            WINDOW
//	================================================================
void window_init(void) {
	cols_old = 0;
	lines_old = 0;
	dirent_init();
	page_size = LINES - 9;
	pageinfo_left  = page_make(page_size);
	pageinfo_right = page_make(page_size);
	page_fill(pageinfo_left,  page_size, dirlist_left);
	page_left_size = page_fill_last;
	page_fill(pageinfo_right, page_size, dirlist_right);
	page_right_size = page_fill_last;
	need_redraw = 0;
}


void pos_left_up(void) {
	if (pos_left) {
		--pos_left;
		--pos_left_long;
		need_redraw = 1;
		return;
	}
	if (pos_left_long) {
		if (pos_left_long > page_size)
			pos_left_long -= page_size;
		else
			pos_left_long = page_size - 1;
		pos_left = page_size - 1;
		dirlist_left = dirlist_stepback(dirlist_left, page_size + page_left_size);
		pageinfo_left = page_fill(pageinfo_left, page_size, dirlist_left);
		page_left_size = page_fill_last;
		need_redraw = 1;
	}
}
void pos_right_up(void) {
	if (pos_right) {
		--pos_right;
		--pos_right_long;
		need_redraw = 1;
		return;
	}	
	if (pos_right_long) {
		if (pos_right_long > page_size)
			pos_right_long -= page_size;
		else
			pos_right_long = page_size - 1;
		pos_right = page_size - 1;
		dirlist_right = dirlist_stepback(dirlist_right, page_size + page_right_size);
		pageinfo_right = page_fill(pageinfo_right, page_size, dirlist_right);
		page_right_size = page_fill_last;
		need_redraw = 1;
	}
}
void pos_left_down(void) {
	if (pos_left < page_left_size - 1) {
		++pos_left;
		++pos_left_long;
		need_redraw = 1;
		return;
	}
	if (page_left_size == page_size && (dirlist_left->curr->count > page_size || dirlist_left->curr->next)) {
		pos_left = 0;
		++pos_left_long;
		pageinfo_left = page_fill(pageinfo_left, page_size, dirlist_left);
		page_left_size = page_fill_last;
		need_redraw = 1;
	}
}
void pos_right_down(void) {
	if (pos_right < page_right_size - 1) {
		++pos_right;
		++pos_right_long;
		need_redraw = 1;
		return;
	}
	if (page_right_size == page_size && (dirlist_right->curr->count > page_size || dirlist_right->curr->next)) {
		pos_right = 0;
		++pos_right_long;
		pageinfo_right = page_fill(pageinfo_right, page_size, dirlist_right);
		page_right_size = page_fill_last;
		need_redraw = 1;
	}	
}

void window_main(void) {
	window_side = 0;
	initscr();
	cbreak();
	noecho();
	colors_pairs_init();
	window_init();
	char noexit = 1;
	length_calc();
	window_redraw();
	while (noexit) {
		switch (getch()) {
		case 's':
			if (window_side)
				pos_right_down();
			else
				pos_left_down();
			break;
		case 'w':
			if (window_side)
				pos_right_up();
			else
				pos_left_up();
			break;
		case '\t':
			(window_side) ? (window_side = 0) : (window_side = 1);
			need_redraw = 1;
			break;
		case 'd':
			dirent_in();
			need_redraw = 1;
			break;
		case 'x':
			noexit = 0;
		default:
			break;
		}
		window_redraw();
	}
	endwin();
	page_free(pageinfo_left);
	page_free(pageinfo_right);
	dirlist_free(dirlist_left);
	dirlist_free(dirlist_right);
}
void window_resize_cols(void) {
	if (COLS != cols_old) {
		cols_old = COLS;
		length_calc();
		need_redraw = 1;
	}
}
void window_resize_lines(void) {
	if (LINES != lines_old) {
		lines_old = LINES;
		dirlist_stepback(dirlist_left,  page_size);
		dirlist_stepback(dirlist_right, page_size);
		page_free(pageinfo_left);
		page_free(pageinfo_right);
		page_size = LINES - 9;
		pageinfo_left  = page_make(page_size);
		pageinfo_right = page_make(page_size);
		page_fill(pageinfo_left, page_size,  dirlist_left);
		page_left_size = page_fill_last;
		page_fill(pageinfo_right, page_size, dirlist_right);
		page_right_size = page_fill_last;
		need_redraw = 1;
	}
}
void window_redraw(void) {
	window_resize_cols();
	window_resize_lines();
	if (need_redraw) {
		draw_line_top();
		draw_line_path();
		draw_line_cols();
		draw_main();
		draw_line_bottom();
		draw_dirent();
		draw_console();
		need_redraw = 0;
	}
	refresh();
}