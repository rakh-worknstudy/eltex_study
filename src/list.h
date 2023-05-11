#ifndef _LIST_H
#define _LIST_H

#define DIRLIST_CHUNK_SIZE 8

typedef struct dirlist_chunk {
	struct dirlist_chunk *prev;
	struct dirlist_chunk *next;
	char 			name[DIRLIST_CHUNK_SIZE][257];
	unsigned long 	size[DIRLIST_CHUNK_SIZE];
	long 			time[DIRLIST_CHUNK_SIZE];
	char 			type[DIRLIST_CHUNK_SIZE];
	unsigned char count;
} dirchunk_t;

typedef struct dirlist {
	dirchunk_t *head;
	dirchunk_t *curr;
	dirchunk_t *last;
	unsigned char iter;
} dirlist_t;

typedef struct dirinfo {
	unsigned long size;
	long time;
	char name[257];
	char type;
} dirinfo_t;

dirlist_t *dirlist_make(const char *path);
dirlist_t *dirlist_stepback(dirlist_t *this, unsigned char step);
dirinfo_t *dirlist_read(dirlist_t *this);
int dirlist_free(dirlist_t *this);

dirinfo_t *page_make(char page_size);
dirinfo_t *page_fill(dirinfo_t *page, char page_size, dirlist_t *list);
int page_fill_last;
int page_free(dirinfo_t *this);

#endif /* _LIST_H */