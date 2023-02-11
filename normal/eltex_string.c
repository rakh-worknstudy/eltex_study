#include "eltex_string.h"
#include <stdio.h>
#include <malloc.h>

char *eltex_string_get(void) {
  char *string = (char *)malloc(ELTEX_STRING_CHUNK_LENGTH * sizeof(char));
  if (string) {
    char *string_pos = string;
    unsigned char chunk_free = 0;
    while (1) {
      char buffer = getchar();
      if (buffer == '\n' || buffer == '\0') {
        if (string_pos == string) {
          free(string);
          string = NULL;
        }
        break;
      }
      if (!chunk_free) {
        string = (char *)realloc(string, ((string_pos - string) + ELTEX_STRING_CHUNK_LENGTH) * sizeof(char));
        if (!string) {
          printf("ERROR: Realloc failed\n");
          break;
        }
        chunk_free = 8;
      }
      *(string_pos++) = buffer;
      --chunk_free;
    }
  } else {
    printf("ERROR: Malloc failed\n");
  }
  return string;
}
