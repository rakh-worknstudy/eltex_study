#ifndef _WINDOW_H
#define _WINDOW_H

//	================================================================
//	                            DIRENT
//	================================================================
#include <sys/types.h>
#include <dirent.h>
#include "list.h"

void dirent_init(void);
void dirent_out(void);
void dirent_in(void);
void dirent_prev(void);
char path_left[PATH_MAX + 1], path_right[PATH_MAX + 1];
dirlist_t *dirlist_left, *dirlist_right;
short pos_left,  pos_right;
unsigned long pos_left_long, pos_right_long;
dirinfo_t *pageinfo_left, *pageinfo_right;
char pagesize_left, pagesize_right;
char page_size;
int page_left_size, page_right_size;

char need_redraw;

//	================================================================
//	                            LENGTH
//	================================================================
void length_calc(void);
short length_name;
short length_size;
short length_edit;

//	================================================================
//	                            COLORS
//	================================================================
void colors_pairs_init(void);

//	================================================================
//	                             DRAW
//	================================================================
void draw_line_top(void);
void draw_line_path(void);
void draw_line_cols(void);
void draw_main(void);
void draw_console(void);
void draw_line_bottom(void);
short x, y;
short cols_old, lines_old;

//	================================================================
//	                            WINDOW
//	================================================================
void window_init(void);
void window_main(void);
void window_redraw(void);
char window_side;	//	Working side

void window_resize_cols(void);
void window_resize_lines(void);

#endif /* _WINDOW_H */