#include "window.h"
#include <ncurses.h>
#include <malloc.h>

//	================================================================
//	                            DIRENT
//	================================================================
#include <dirent.h>
/* DIRENT INIT */
void dirent_init(void) {
	/* LEFT */
	path_left[0] = '/';
	path_left[1] = 0;
	dir_left = opendir(path_left);
	list_left = (struct dirent_list *)malloc(sizeof(struct dirent_list));
	list_left->here = telldir(dir_left);
	list_left->prev = NULL;
	list_left->next = NULL;
	closedir(dir_left);
	/* RIGHT */
	path_right[0] = '/';
	path_right[1] = 0;
	dir_right = opendir(path_right);
	list_right = (struct dirent_list *)malloc(sizeof(struct dirent_list));
	list_right->here = telldir(dir_right);
	list_right->prev = NULL;
	list_right->next = NULL;
	closedir(dir_right);
}
/* DIRENT MOVE UP */
void dirent_out(void) {
	char delim_last;
	short iter;
	switch (window_side) {
	case 0:
		/* LEFT */
		for (iter = 0; path_left[iter]; ++iter) {
			if (path_left[iter] == '/')
				delim_last = iter;
		}
		if (delim_last)
			path_left[delim_last + 1] = 0;
		return;
	case 1:
		/* RIGHT */
		for (iter = 0; path_right[iter]; ++iter) {
			if (path_right[iter] == '/')
				delim_last = iter;
		}
		if (delim_last)
			path_right[delim_last + 1] = 0;
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
		/* LEFT */
		for (iter = 0; path_left[iter]; ++iter)
			continue;
		dir_left = opendir(path_left);
		seekdir(dir_left, list_left->here);
		for (pos_iter = 0; pos_iter < pos_left % page_size; ++pos_iter)
			dirent_left = readdir(dir_left);
		path_left[iter++] = 0;
		for (pos_iter = 0; dirent_left->d_name[pos_iter]; ++pos_iter)
			path_left[iter++] = dirent_left->d_name[pos_iter];
		path_left[iter] = 0;
		closedir(dir_left);
		return;
	case 1:
		/* RIGHT */
		for (iter = 0; path_right[iter]; ++iter)
			continue;
		dir_right = opendir(path_left);
		seekdir(dir_right, list_left->here);
		for (pos_iter = 0; pos_iter < pos_left % page_size; ++pos_iter)
			dirent_right = readdir(dir_right);
		path_right[iter++] = 0;
		for (pos_iter = 0; dirent_right->d_name[pos_iter]; ++pos_iter)
			path_right[iter++] = dirent_right->d_name[pos_iter];
		path_right[iter] = 0;
		closedir(dir_right);
		return;
	default:
		return;
	}	
}
/* DIRENT INCREMENT */
void list_next(void) {
	switch (window_side) {
	case 0:
		if (list_left->next == NULL)
			list_add();
		else
			list_left = list_left->next;
		return;
	case 1:
		if (list_right->next == NULL)
			list_add();
		else
			list_right = list_right->next;
		return;
	default:
		return;
	}
}
/* FREE LIST EXCEPT HEAD */
void list_free(void) {
	struct dirent_list *list_save;
	switch (window_side) {
	case 0:
		/* LEFT */
		for (; list_left->prev != NULL; list_left = list_left->prev)
			continue;
		for (; list_left->next != NULL; list_left = list_left->next) {
			list_save = list_left->next->next;
			free(list_left->next);
			list_left->next = list_save;
		}
		return;
	case 1:
		/* RIGHT */
		for (; list_right->prev != NULL; list_right = list_right->prev)
			continue;
		for (; list_right->next != NULL; list_right = list_right->next) {
			list_save = list_right->next->next;
			free(list_right->next);
			list_right->next = list_save;
		}
		return;
	default:
		return;
	}
}
/* OPEN NEW LIST */
void list_open(void) {
	list_free();
	switch (window_side) {
	case 0:
		/* LEFT */
		dir_left = opendir(path_left);
		list_left->here = telldir(dir_left);
		list_left->next = NULL;
		closedir(dir_left);
		return;
	case 1:
		/* RIGHT */
		dir_right = opendir(path_right);
		list_right->here = telldir(dir_right);
		list_right->next = NULL;
		closedir(dir_right);
		return;
	default:
		return;
	}
}
/* ADD TO LIST AT POSITION */
void list_add(void) {
	//struct dirent_list *list_new;
	switch (window_side) {
	case 0:
		/* LEFT */
		dir_left = opendir(path_left);
		seekdir(dir_left, list_left->here);
		for (short iter = 0; iter < LINES - 9; ++iter)
			readdir(dir_left);
		list_new = (struct dirent_list *)malloc(sizeof(struct dirent_list));
		list_new->here = telldir(dir_left);
		list_new->prev = list_left;
		list_new->next = NULL;
		list_left->next = list_new;
		list_left = list_left->next;
		closedir(dir_left);
		return;
	case 1:
		/* RIGHT */
		dir_right = opendir(path_right);
		seekdir(dir_right, list_right->here);
		for (short iter = 0; iter < LINES - 9; ++iter)
			readdir(dir_right);
		list_new = (struct dirent_list *)malloc(sizeof(struct dirent_list));
		list_new->prev = list_right;
		list_right->next = list_new;
		list_right = list_right->next;
		list_right->here = telldir(dir_right);
		list_right->next = NULL;
		closedir(dir_right);
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
			if (x == 0 || x == COLS/2) {
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



void draw_name_left(void) {
	if (dirent_left->d_type == DT_DIR) {
		attron(A_BOLD);
		mvaddch(y, 1, '/');
	} else {
		mvaddch(y, 1, '.');
	}
	for (x = 2; x < length_name + 1; ++x) {
		if (dirent_left->d_name[x - 2])
			mvaddch(y, x, dirent_left->d_name[x - 2]);
		else
			break;
	}
	if (dirent_left->d_type == DT_DIR) {
		attroff(A_BOLD);
	}
	for (; x < length_name + 1; ++x)
		mvaddch(y, x, ' ');
	mvaddch(y, x, ACS_VLINE);
}
void draw_size_left(void) {
//	unsigned short temp = dirent_left->d_reclen;
//	char reclen[7], iter;
//	for (iter = 6; iter >= 0; --iter) {
//		reclen[iter] = temp % 10 + '0';
//		temp /= 10;
//	}
//	++iter;
	for (x = length_name + 2; x < length_name + length_size + 2; ++x) {
//		mvaddch(y, x, reclen[iter++]);
//		if (iter == 7)
			break;
	}
	for (; x < length_name + length_size + 2; ++x)
		mvaddch(y, x, ' ');
	mvaddch(y, x, ACS_VLINE);	
}
void draw_time_left(void) {
	for (x = length_name + length_size + 3; x < length_name + length_size + length_edit + 3; ++x) {
		mvaddch(y, x, ' ');
	}
}
void draw_space_left(void) {
	for (x = 1; x < COLS/2 - 1; ++x) {
		if (x == length_name + 1 || x == length_name + length_size + 2) {
			mvaddch(y, x, ACS_VLINE);
			continue;
		}
		mvaddch(y, x, ' ');
	}
}

void draw_name_right(void) {
	if (dirent_left->d_type == DT_DIR) {
		attron(A_BOLD);
		mvaddch(y, COLS/2 + 1, '/');
	} else {
		mvaddch(y, COLS/2 + 1, '.');
	}
	for (x = COLS/2 + 2; x < length_name + COLS/2 + 1; ++x) {
		if (dirent_right->d_name[x - COLS/2 - 2])
			mvaddch(y, x, dirent_right->d_name[x - COLS/2 - 2]);
		else
			break;
	}
	if (dirent_left->d_type == DT_DIR) {
		attroff(A_BOLD);
	}
	for (; x < length_name + COLS/2 + 1; ++x)
		mvaddch(y, x, ' ');
	mvaddch(y, x, ACS_VLINE);
}
void draw_size_right(void) {
//	unsigned short temp = dirent_right->d_reclen;
//	char reclen[20], iter;
//	for (iter = 6; iter >= 0; --iter) {
//		reclen[iter] = temp % 10 + '0';
//		temp /= 10;		
//	}
//	++iter;
	for (x = length_name + COLS/2 + 2; x < length_name + length_size + COLS/2 + 2; ++x) {
//		mvaddch(y, x, reclen[iter++]);
//		if (iter == 7)
			break;
	}
	for (; x < length_name + length_size + COLS/2 + 2; ++x)
		mvaddch(y, x, ' ');
	mvaddch(y, x, ACS_VLINE);	
}
void draw_time_right(void) {
	for (x = length_name + length_size + COLS/2 + 3; x < COLS - 1; ++x) {
		mvaddch(y, x, ' ');
	}	
}
void draw_space_right(void) {
	for (x = COLS/2 + 1; x < COLS - 1; ++x) {
		if (x == length_name + COLS/2 + 1 || x == length_name + length_size + COLS/2 + 2) {
			mvaddch(y, x, ACS_VLINE);
			continue;
		}
		mvaddch(y, x, ' ');
	}
}

void draw_dirent(void) {
	/* LEFT */
	dir_left = opendir(path_left);
	seekdir(dir_left, list_left->here);
	for (y = 3; y < LINES - 6; ++y) {
		if ((dirent_left = readdir(dir_left))) {
			if (y == pos_left % (LINES - 9) + 3) {
				attroff(COLOR_PAIR(1));
				attron(COLOR_PAIR(2));
			}
			draw_name_left();
			draw_size_left();
			draw_time_left();
			if (y == pos_left % (LINES - 9) + 3) {
				attroff(COLOR_PAIR(2));
				attron(COLOR_PAIR(1));
				for (x = 1; x < COLS/2 - 1; ++x) {
					if (dirent_left->d_name[x - 1])
						mvaddch(LINES - 5, x, dirent_left->d_name[x - 1]);
					else
						break;
				}
				for (; x < COLS/2 - 1; ++x)
					mvaddch(LINES - 5, x, ' ');
			}
		} else {
			break;
		}
	}
	for (; y < LINES - 6; ++y) {
		if (y == pos_left % (LINES - 9) + 3) {
			attroff(COLOR_PAIR(1));
			attron(COLOR_PAIR(2));
			--pos_left;
			--y;
			draw_name_left();
			draw_size_left();
			draw_time_left();
			attroff(COLOR_PAIR(2));
			attron(COLOR_PAIR(1));
		} else {
			draw_space_left();
		}
	}
	closedir(dir_left);
	/* RIGHT */
	dir_right = opendir(path_right);
	seekdir(dir_right, list_right->here);
	for (y = 3; y < LINES - 6; ++y) {
		if ((dirent_right = readdir(dir_right))) {
			if (y == pos_right % (LINES - 9) + 3)
				attron(COLOR_PAIR(2));
			draw_name_right();
			draw_size_right();
			draw_time_right();
			if (y == pos_right % (LINES - 9) + 3) {
				attron(COLOR_PAIR(1));
				for (x = COLS/2 + 1; x < COLS - 1; ++x) {
					if (dirent_right->d_name[x - COLS/2 -1])
						mvaddch(LINES - 5, x, dirent_right->d_name[x - COLS/2 - 1]);
					else
						break;
				}
				for (; x < COLS - 1; ++x)
					mvaddch(LINES - 5, x, ' ');
			}
		} else {
			break;
		}
	}
	for (; y < LINES - 6; ++y) {
		if (y == pos_left % (LINES - 9) + 3) {
			attroff(COLOR_PAIR(1));
			attron(COLOR_PAIR(2));
			--pos_right;
			--y;
			draw_name_right();
			draw_size_right();
			draw_time_right();
			attroff(COLOR_PAIR(2));
			attron(COLOR_PAIR(1));
		} else {
			draw_space_right();
		}
	}
	closedir(dir_right);
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
	const char str_help[] = "  W/S - MOVE UP/DOWN | ENTER - JUMP IN | TAB - SWITCH SIDE | CONSOLE - OFF  ";
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
	dirent_init();
	page_size = LINES - 8;
	page_left_step = 0;
	page_right_step = 0;
	pos_left_step = 0;
	pos_right_step = 0;
}
void window_main(void) {
	window_side = 0;
	initscr();
	cbreak();
	noecho();
	colors_pairs_init();
	window_init();
	char max_curr, max_prev;
	while(1) {
		length_calc();
		if (LINES - 8 != page_size) {
		}
		window_redraw();
		switch (getch()) {
		case 's':
			if (window_side) {
				++pos_right;
				if (pos_right % (LINES - 9) == 0) {
					list_next();
				}
			} else {
				++pos_left;
				if (pos_left % (LINES - 9) == 0) {
					list_next();
				}
			}
			break;
		case 'w':
			if (window_side) {
				if (pos_right)
					--pos_right;
			} else if (pos_left)
					--pos_left;
			break;
		case '\t':
			(window_side) ? (window_side = 0) : (window_side = 1);
			break;
		case '\n':
		case '\r':
			dirent_in();
			list_open();
			break;
		default:
			break;
		}
	}
	endwin();
	window_side = 0;
	list_free();
	free(list_left);
	window_side = 1;
	list_free();
	free(list_right);
}
void window_redraw(void) {
	draw_line_top();
	draw_line_path();
	draw_line_cols();
	draw_main();
	draw_dirent();
	draw_console();
	draw_line_bottom();
	refresh();
}