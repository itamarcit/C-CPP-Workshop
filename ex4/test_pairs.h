#ifndef _TEST_PAIRS_H_
#define _TEST_PAIRS_H_

/**
 * Copies the char key of the pair.
 */
void *char_key_cpy (const_keyT key)
{
  char *new_char = malloc (sizeof (char));
  *new_char = *((char *) key);
  return new_char;
}

/**
 * Copies the int value of the pair.
 */
void *int_value_cpy (const_valueT value)
{
  int *new_int = malloc (sizeof (int));
  *new_int = *((int *) value);
  return new_int;
}

/**
 * Compares the char key of the pair.
 */
int char_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return *(char *) key_1 == *(char *) key_2;
}

/**
 * Compares the int value of the pair.
 */
int int_value_cmp (const_valueT val_1, const_valueT val_2)
{
  return *(int *) val_1 == *(int *) val_2;
}

/**
 * Frees the char key of the pair.
 */
void char_key_free (keyT *key)
{
  if (key && *key)
    {
      free (*key);
      *key = NULL;
    }
}

/**
 * Frees the int value of the pair.
 */
void int_value_free (valueT *val)
{
  if (val && *val)
    {
      free (*val);
      *val = NULL;
    }
}

/**
 * @param elem pointer to an int
 * @return 1 if the int is even, else - 0
 */
int is_even (const_keyT elem)
{
  int c = *((int *) elem);
  if ((c % 2) == 0)
    {
      return 1;
    }
  return 0;
}

/**
 * @param elem pointer to an int
 * @return 1 if the int is odd, else - 0
 */
int is_odd (const_keyT elem)
{
  int c = *((int *) elem);
  if ((c % 2) == 1)
    {
      return 1;
    }
  return 0;
}

/**
 * doubles the value pointed to by the given pointer
 * @param elem pointer to an integer (valT of pair_char_int)
 */
void double_value (valueT elem)
{
  *((int *) elem) *= 2;
}

void negative_value(valueT elem) {
  *((int *) elem) = - (*(int *) elem);
}

#endif //_TEST_PAIRS_H_
