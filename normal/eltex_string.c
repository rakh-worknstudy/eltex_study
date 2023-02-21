#include "eltex_string.h"
#include <stdio.h>
#include <malloc.h>

char *eltex_string_get(char **string) {
  if (string == NULL)
    return NULL;
  *string = (char *)malloc(ELTEX_STRING_CHUNK_LENGTH * sizeof(char));
  if (*string) {
    unsigned int length;
    char *string_pos = *string;
    unsigned char chunk_free = ELTEX_STRING_CHUNK_LENGTH;
    while (1) {
      if (!chunk_free) {
        *string = (char *)realloc(*string, (length + ELTEX_STRING_CHUNK_LENGTH) * sizeof(char));
        if (!*string) {
#         ifdef _ELTEX_DEBUG
          printf("ERROR: Realloc failed\n");
#         endif /* _ELTEX_DEBUG */
          break;
        }
        chunk_free = ELTEX_STRING_CHUNK_LENGTH;
      }
      char buffer = getchar();
      if (buffer == '\n' || buffer == '\0') {
        *string_pos = '\0';
        if (string_pos == *string)
          *string = eltex_string_free(string);
        break;
      }
      *(string_pos++) = buffer;
      --chunk_free;
      ++length;
    }
  } else {
#   ifdef _ELTEX_DEBUG
    printf("ERROR: Malloc failed\n");
#   endif /* _ELTEX_DEBUG */
  }
  return *string;
}

char *eltex_string_free(char **string) {
  if (string != NULL) {
    free(*string);
    string = NULL;
  }
  return NULL;
}
