#ifndef HASHFUNCS_H_
#define HASHFUNCS_H_

#include <stdlib.h>

/**
 * Integers simple hash func.
 */
size_t hash_int(const void *elem){
  size_t hash = *((int *) elem);
  return hash;
}

/**
 * Chars simple hash func.
 */
size_t hash_char(const void *elem){
  size_t hash = *((char *) elem);
  return hash;
}

#endif // HASHFUNCS_H_