#ifndef ELTEX_STRING_H_
#define ELTEX_STRING_H_

#define ELTEX_STRING_CHUNK_LENGTH 64

char *eltex_string_get(char **string);
char *eltex_string_free(char **string);

#endif  /* ELTEX_STRING_H_ */
