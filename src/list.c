#include "list.h"
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <malloc.h>

//	DEBUG
#include <stdio.h>
FILE *logfd;

dirchunk_t *dirchunk_init(void) {
	dirchunk_t *this = (dirchunk_t *)malloc(sizeof(dirchunk_t));
	this->prev  = NULL;
	this->next  = NULL;
	this->count = 0;
	return this;
}
int dirchunk_free(dirchunk_t *this) {
	if (!this)
		return -1;
	free(this);
	return 0;
}

dirlist_t *dirlist_stepback(dirlist_t *this, unsigned char step) {
	while (step) {
		if (this->iter > step) {
			this->iter -= step;
			step = 0;
		} else if (this->curr->prev) {
			step -= this->iter;
			this->curr = this->curr->prev;
			this->iter = DIRLIST_CHUNK_SIZE;
		} else {
			this->iter = 0;
			step = 0;
		}
	}
	return this;
}

dirlist_t *dirlist_make(const char *path) {
	if (!(*path))
		return NULL;
	DIR *dir = opendir(path);
	if (!dir)
		return NULL;
	dirlist_t *this = (dirlist_t *)malloc(sizeof(dirlist_t));
	//	this->head = dirchunk_init();
	this->head = dirchunk_init();
	this->curr = this->head;
	this->last = this->head;
	this->iter = 0;
	char iter = 0;
	char filepath[PATH_MAX + 1];
	strcpy(filepath, path);
	struct dirent *entry;
	unsigned long pathlen = strlen(filepath);
	//	logfd = fopen("log.log", "w");
	while ((entry = readdir(dir))) {
		if (!strcmp(entry->d_name, "."))
			continue;
		filepath[pathlen] = 0;
		strcat(filepath, entry->d_name);
		strcpy(this->last->name[iter], entry->d_name);
		//	fprintf(logfd, "%s %s\n", this->last->name[iter], entry->d_name);
		this->last->type[iter] = entry->d_type;
		struct stat st;
		stat(filepath, &st);
		this->last->size[iter] = st.st_size;
		this->last->time[iter] = st.st_mtime;
		if (++iter >= DIRLIST_CHUNK_SIZE) {
			this->last->count = iter;
			//	fprintf(logfd, "%d %d\n", iter, this->last->count);
			this->last->next = (dirchunk_t *)malloc(sizeof(dirchunk_t));
			this->last->next->prev = this->last;
			this->last->next->next = NULL;
			this->last->next->count = 0;
			this->last = this->last->next;
			iter = 0;
			//	fprintf(logfd, "%p %p\n", this->curr->next, this->last);
		}
	}
	this->last->count = iter;
	//	fprintf(logfd, "%d %d\n", iter, this->last->count);
	//	fclose(logfd);
	return this;
}
dirinfo_t dinf;
dirinfo_t *dirlist_read(dirlist_t *this) {
	if (!this->curr || (this->curr->count <= this->iter))
		return NULL;
	dinf.time = this->curr->time[this->iter];
	dinf.size = this->curr->size[this->iter];
	strcpy(dinf.name, this->curr->name[this->iter]);
	dinf.type = this->curr->type[this->iter];
	if (++(this->iter) >= DIRLIST_CHUNK_SIZE && this->curr->next) {
		this->curr = this->curr->next;
		this->iter = 0;
	}
	return &dinf;
}

int dirlist_free(dirlist_t *this) {
	if (!this)
		return -1;
	while(this->last != NULL) {
		this->curr = this->last->prev;
		dirchunk_free(this->last);
		this->last = this->curr;
	}
	return 0;
}

dirinfo_t *page_make(char page_size) {
	dirinfo_t *page = (dirinfo_t *)malloc(page_size * sizeof(dirinfo_t));
	return page;
}
dirinfo_t *page_fill(dirinfo_t *page, char page_size, dirlist_t *list) {
	if (!(page && page_size && list && list->curr))
		return NULL;
	//	logfd = fopen("log.log", "w");
	page_fill_last = 0;
	for (char iter = 0; iter < page_size; ++iter) {
		if (dirlist_read(list) != NULL) {
			page[iter].time = dinf.time;
			page[iter].size = dinf.size;
			strcpy(page[iter].name, dinf.name);
			page[iter].type = dinf.type;
			++page_fill_last;
			//	fprintf(logfd, "%s %s", page[iter].name, dinf.name);
		} else {
			page[iter].type = -1;
		}
	}
	//	fclose(logfd);
	return page;
}
int page_free(dirinfo_t *this) {
	if (!this)
		return -1;
	free(this);
	return 0;
}