#include "test_suite.h"
#include "test_pairs.h"
#include "hash_funcs.h"

void *get_new_pair_char_int (char key, int val)
{
  void *new_pair = pair_alloc (&key, &val,
                               char_key_cpy, int_value_cpy,
                               char_key_cmp, int_value_cmp,
                               char_key_free, int_value_free);
  if (new_pair == NULL)
    {
      return NULL;
    }
  return new_pair;
}

void *get_new_pair_int_char (int key, char val)
{
  void *new_pair = pair_alloc (&key, &val,
                               int_value_cpy, char_key_cpy,
                               int_value_cmp, char_key_cmp,
                               int_value_free, char_key_free);
  if (new_pair == NULL)
    {
      return NULL;
    }
  return new_pair;
}

void *get_new_pair_int_int (int key, int val)
{
  void *new_pair = pair_alloc (&key, &val,
                               int_value_cpy, int_value_cpy,
                               int_value_cmp, int_value_cmp,
                               int_value_free, int_value_free);
  if (new_pair == NULL)
    {
      return NULL;
    }
  return new_pair;
}

/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions
 * exits with exit code 1.
 */
void test_hash_map_insert (void)
{
  hashmap *hm = hashmap_alloc (hash_char);
  char temp;
  void *my_pair;
  int letter;
  for (int i = 0; i < 8; ++i) // adding pairs of type char:int to hashmap,
    // successful insertions
    {
      letter = 'a' + i;
      temp = (char) letter;
      my_pair = get_new_pair_char_int (temp, -i);
      assert(hashmap_insert (hm, my_pair) == 1);
      pair_free (&my_pair);
    }
  assert(hm->size == 8); // check size
  // Unsuccessful insertions
  my_pair = get_new_pair_char_int ('a', -1);
  assert(hashmap_insert (hm, NULL) == 0);
  assert(hashmap_insert (NULL, NULL) == 0);
  assert(hashmap_insert (NULL, my_pair) == 0);
  pair_free (&my_pair);

  // testing increasing the capacity of the hashmap

  for (int i = 0; i < 5; ++i)
    {
      letter = 'a' + 8 + i;
      char new_letter = (char) letter;
      my_pair = get_new_pair_char_int (new_letter, i + 1);
      assert(hashmap_insert (hm, my_pair) == 1);
      pair_free (&my_pair);
      assert((int)hm->size == 8 + i + 1);
    }
  assert(hm->capacity == HASH_MAP_GROWTH_FACTOR * HASH_MAP_INITIAL_CAP);

  hashmap_free (&hm);
}

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at (void)
{
  hashmap *hm = hashmap_alloc (hash_int);
  char temp;
  void *my_pair;
  int letter;
  int num;
  for (int i = 0; i < 8; ++i) // adding pairs of type char:int to hashmap,
    // successful insertions
    {
      letter = 'a' + i;
      temp = (char) letter;
      my_pair = get_new_pair_int_char (i, temp);
      assert(hashmap_insert (hm, my_pair) == 1);
      pair_free (&my_pair);
    }
  for (int i = 0; i < 8; ++i)
    {
      num = i;
      assert(hashmap_at (hm, &num) != NULL);
      num = -i - 1;
      assert(hashmap_at (hm, &num) == NULL);
    }
  // Check for bad input:
  assert(hashmap_at (hm, NULL) == NULL);
  assert(hashmap_at (NULL, 0) == NULL);
  assert(hashmap_at (NULL, NULL) == NULL);
  hashmap_free (&hm);
}

/**
 * This function checks the hashmap_erase function of the hashmap library.
 * If hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase (void)
{
  hashmap *hm = hashmap_alloc (hash_int);
  int keys[16];
  void *my_pair;
  int value;
  for (int i = 0; i < 16; ++i)
    {
      keys[i] = i;
    }
  //Simple test, remove 8 pairs
  for (int i = 0; i < 8; ++i)
    {
      value = -i * 100;
      my_pair = get_new_pair_int_int (keys[i], value);
      assert(hashmap_insert (hm, my_pair) == 1);
      pair_free (&my_pair);
    }
  for (int i = 0; i < 8; ++i)
    {
      assert(hashmap_erase (hm, &(keys[i])) == 1);
    }
  int key = -100;
  my_pair = get_new_pair_int_int (key, 123);
  assert(hashmap_insert (hm, my_pair) == 1);
  pair_free(&my_pair);
  assert(hashmap_erase (hm, &key) == 1);
  assert(hm->capacity == 2);
  assert(hm->size == 0);
  // insert enough pairs so it'll increase the capacity
  for (int i = 0; i < 16; ++i)
    {
      value = i * 100;
      my_pair = get_new_pair_int_int (keys[i], value);
      assert(hashmap_insert (hm, my_pair) == 1);
      pair_free (&my_pair);
    }
  assert(hm->capacity == 32);
  assert(hm->size == 16);
  // Remove enough pairs to lower capacity
  for (int i = 8; i < 16; ++i)
    {
      assert(hashmap_erase (hm, &(keys[i])) == 1);
    }
  assert(hashmap_erase (hm, &(keys[7])) == 1);
  assert(hm->capacity == 16);
  assert(hm->size == 7);
  for (int i = 4; i < 7; ++i)
    {
      assert(hashmap_erase (hm, &keys[i]) == 1);
    }
  assert(hashmap_erase (hm, &(keys[3])) == 1);
  assert(hm->capacity == 8);
  assert(hm->size == 3);
  for (int i = 1; i < 3; ++i)
    {
      assert(hashmap_erase (hm, &(keys[i])) == 1);
    }
  assert(hm->capacity == 4);
  assert(hm->size == 1);
  for (int i = 0; i < 1; ++i)
    {
      assert(hashmap_erase (hm, &(keys[i])) == 1);
    }
  assert(hm->capacity == 2);
  assert(hm->size == 0);

  // Test bad input and non-existing keys
  assert(hashmap_erase (hm, NULL) == 0);
  assert(hashmap_erase (NULL, &(keys[0])) == 0);
  assert(hashmap_erase (NULL, NULL) == 0);
  double wrong_key = -100.4;
  assert(hashmap_erase (hm, &wrong_key) == 0);

  hashmap_free (&hm);
}

/**
 * This function checks the hashmap_get_load_factor function of the
 * hashmap library.
 * If hashmap_get_load_factor fails at some points,
 * the functions exits with exit code 1.
 */
void test_hash_map_get_load_factor (void)
{
  hashmap *hm = hashmap_alloc (hash_char);
  assert(hashmap_get_load_factor (hm) == 0);
  void *my_pair;
  int key, value;
  for (int i = 0; i < 1000; ++i)
    {
      key = -i;
      value = i * i;
      my_pair = get_new_pair_int_int (key, value);
      hashmap_insert (hm, my_pair);
      pair_free (&my_pair);
      assert((float) hashmap_get_load_factor (hm)
             == (float) hm->size / hm->capacity);
    }
  assert(
      (float) hashmap_get_load_factor (hm) == (float) hm->size / hm->capacity);
  hashmap_free (&hm);
}

/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points, the functions
 * exits with exit code 1.
 */
void test_hash_map_apply_if ()
{
  hashmap *hm = hashmap_alloc (hash_int);
  void *my_pair;
  int key, value;
  for (int i = 0; i < 500; ++i)
    {
      key = i;
      value = i;
      my_pair = get_new_pair_int_int (key, value);
      hashmap_insert (hm, my_pair);
      pair_free (&my_pair);
    }
  key = 2;
  int *old_value = hashmap_at (hm, &key);
  int val2 = (*old_value);
  assert(hashmap_apply_if (hm, is_even, negative_value) == 250);
  //check that the function worked
  int val1 = *((int *) (hashmap_at (hm, &key)));
  assert(val1 == -val2);
  key = 1;
  old_value = hashmap_at (hm, &key);
  val2 = (*old_value);
  assert(hashmap_apply_if (hm, is_odd, double_value) == 250);
  val1 = *((int *) (hashmap_at (hm, &key)));
  assert(val1 == 2 * val2);
  hashmap_free (&hm);
}