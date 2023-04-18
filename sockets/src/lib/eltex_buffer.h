#ifndef _ELTEX_BUFFER_H
#define _ELTEX_BUFFER_H

unsigned short eltex_strlen(const char *src, unsigned short max);
void *eltex_bzero(void *dst, const unsigned n);
int eltex_strcmp(const char *src, unsigned short length);

#endif /* _ELTEX_BUFFER_H */
