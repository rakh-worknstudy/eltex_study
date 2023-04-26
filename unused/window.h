#ifndef _WINDOW_H
#define _WINDOW_H
//	================================================================
//	                        DIRECTORY LIST
//	================================================================
struct dirlist_chunk {
	struct dirlist_chunk *prev;		//	previous chunk
	struct dirlist_chunk *next;		//	next chunk
	char size;						//	names count
	char iter;						//	names iter;
	char *name[32];					//	names
};
struct dirlist_chunk *dirlist_chunk_make(void);
void                  dirlist_chunk_free(struct dirlist_chunk * this);
struct dirlist {
	struct dirlist_chunk *head;		//	dirlist head
	struct dirlist_chunk *curr;		//	dirlist iter
	struct dirlist_chunk *last;
};
struct dirlist *dirlist_make(void);
struct dirlist *dirlist_add(const char name[]);
void            dirlist_free(struct dirlist *this);
char           *dirlist_read(dirlist *this);
void            dirlist_back(dirlist *this, long step);

#endif /* _WINDOW_H */