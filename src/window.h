#ifndef _WINDOW_H
#define _WINDOW_H

//	================================================================
//	                            DIRENT
//	================================================================
#include <dirent.h>
#include <sys/types.h>

void dirent_init(void);
void dirent_out(void);
void dirent_in(void);
void dirent_prev(void);
char path_left[1024], path_right[1024];
DIR *dir_left, *dir_right;
struct dirent *dirent_left, *dirent_right;
short page_left, page_right;
short pos_left,  pos_right;
char page_left_step, page_right_step;
char pos_left_step,  pos_right_step;
char page_size;

struct dirent_list {
	off_t here;
	struct dirent_list *prev;
	struct dirent_list *next;
} *list_left, *list_right, *list_new;
void list_free(void);
void list_open(void);
void list_add(void);
void list_next(void);
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


//	================================================================
//	                            WINDOW
//	================================================================
void window_init(void);
void window_main(void);
void window_redraw(void);
char window_side;	//	Working side

#endif /* _WINDOW_H */