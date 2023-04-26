#include "window.h"
#include <dirent.h>

//	================================================================
//	                        DIRECTORY LIST
//	================================================================
struct dirlist_chunk *dirlist_chunk_make(void) {
	struct dirlist_chunk *this = (struct dirlist_chunk *)malloc(sizeof(dirlist_chunk));
	this->size = 0;
	this->iter = 0;
	return this;
}
void dirlist_chunk_free(struct dirlist_chunk *this) {
	for (this->iter = 0; this->iter < size; ++this->iter)
		free(this->name[this->iter]);
	free(this);
}
struct dirlist *dirlist_make(void) {
	struct dirlist *this = (struct dirlist *)malloc(sizeof(dirlist));
	this->head = dirlist_chunk_make();
	this->head->next = NULL;
	this->head->prev = NULL;
	this->curr = this->head;
	this->last = this->head;
	return this;
}
struct dirlist *dirlist_add(struct dirlist *this, const char name[]) {
	long length = 0;
	for (char *ptr = (char *)name; *ptr; ++ptr)
		++length;
	this->last->name[this->last->size] = (char *)malloc((length + 1) * sizeof(char));
	for (char iter = 0; iter < length; ++iter)
		this->last->name[this->last->size][iter] = name[iter];
	if (++(this->last->size) % 32 == 0) {
		struct dirlist_chunk *chunk_new = dirlist_chunk_make();
		chunk_new->prev = this->curr;
		chunk_new->next = NULL;
		this->last->next = chunk_new;
		this->last = chunk_new;
	}
}
void dirlist_free(struct dirlist *this) {
	while (dirlist->last->prev) {
		dirlist->curr = dirlist->last->prev;
		dirlist_chunk_free(dirlist->last);
		dirlist->last = dirlist->curre;
	}
	free(this);
}
char *dirlist_read(dirlist *this) {
	if (this->curr->iter == this->curr->size) {
		this->curr->iter = 0;
		if (this->curr->next == NULL)
			return NULL;
		this->curr = this->curr->next;
	}
	return this->curr->names[(this->curr->iter)++];
}
void dirlist_back(dirlist *this, long step) {
	while (step) {
		if (this->curr->iter) {
			--this->curr->iter;
		} else {
			this->curr = this->curr->prev;
		}
	}
}