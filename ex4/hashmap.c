#include "hashmap.h"
#define HASH_MAP_MIN_SIZE 1
/**
 * allocates memory for a vector** and allocates vectors inside each ptr,
 * in size of HASH_MAP_INITIAL_CAP
 * @param ptr - given ptr to allocate memory in
 * @return vector** if successful, NULL otherwise.
 */
vector **buckets_alloc (int cap)
{
  vector **ptr = (vector **) malloc (sizeof (vector * ) * cap);
  if (ptr == NULL)
    {
      return NULL;
    }
  for (int i = 0; i < cap; ++i)
    {
      ptr[i] = vector_alloc (pair_copy, pair_cmp, pair_free);
      if (ptr[i] == NULL)
        {
          for (int j = 0; j < i; ++j)
            {
              vector_free (&(ptr[j]));
            }
          free (ptr);
          return NULL;
        }
    }
  return ptr;
}

void buckets_free (vector ***ptr, int cap)
{
  for (int i = 0; i < cap; ++i)
    {
      vector_free (&((*ptr)[i]));
    }
  free (*ptr);
  ptr = NULL;
}

int complete_hash_func (size_t hash_func_res, int cap)
{
  int index;
  size_t altered_cap = cap - 1;
  size_t s_index = hash_func_res & altered_cap;
  index = (int) s_index;
  return index;
}

hashmap *hashmap_alloc (hash_func func)
{
  hashmap *new_hash = (hashmap *) malloc (sizeof (hashmap));
  if (new_hash == NULL)
    {
      return NULL;
    }
  new_hash->buckets = buckets_alloc (HASH_MAP_INITIAL_CAP);
  if (new_hash->buckets == NULL)
    {
      free (new_hash);
      return NULL;
    }
  new_hash->hash_func = func;
  new_hash->capacity = HASH_MAP_INITIAL_CAP;
  new_hash->size = 0;
  return new_hash;
}

void hashmap_free (hashmap **p_hashmap)
{
  int cap = (int) (*p_hashmap)->capacity;
  if (p_hashmap == NULL || *p_hashmap == NULL)
    {
      return;
    }
  buckets_free (&((*p_hashmap)->buckets), cap);
  free (*p_hashmap);
  *p_hashmap = NULL;
}
/**
 * re-organizes the hash-table after a change of size
 * @param hash hash table
 * @param mode 0-decrease size 1-increase size
 * @return 0 if failed, 1 if successful
 */
int reorganize_hash (hashmap *hash, int mode)
{
  int orig_cap;
  int hash_cap = (int) hash->capacity;
  if (mode == 1)
    {
      orig_cap = (int) (hash_cap / HASH_MAP_GROWTH_FACTOR);
    }
  else
    {
      orig_cap = (int) (hash_cap * HASH_MAP_GROWTH_FACTOR);
    }
  int vec_size, hash_res;
  pair *p_temp;
  void *v_temp;
  void *p_copied;
  vector **temp = buckets_alloc (hash->capacity);
  if (temp == NULL)
    {
      return 0;
    }
  for (int i = 0; i < orig_cap; ++i)
    {
      vec_size = (hash->buckets)[i]->size;
      for (int j = 0; j < vec_size; ++j)
        {
          v_temp = (void *) hash->buckets[i]->data[j];
          p_temp = (pair *) v_temp;
          p_copied = pair_copy (v_temp);
          if (p_copied == NULL)
            {
              return 0;
            }
          hash_res = complete_hash_func
              (hash->hash_func (p_temp->key), hash_cap);
          if (vector_push_back (temp[hash_res], p_copied) == 0)
            {
              pair_free (&p_copied);
              buckets_free (&temp, hash_cap);
              return 0;
            }
          pair_free (&p_copied);
        }
    }
  buckets_free (&(hash->buckets), orig_cap);
  hash->buckets = temp;
  return 1;
}
int hashmap_insert (hashmap *hash_map, const pair *in_pair)
{
  int index, cap;
  if (hash_map == NULL || in_pair == NULL)
    {
      return 0;
    }
  if (hashmap_at (hash_map, in_pair->key) != NULL)
    {
      return 0;
    }
//  pair *p_copied = (pair *) in_pair;
  int orig_cap = (int) hash_map->capacity;
  if (hashmap_get_load_factor (hash_map)
      < HASH_MAP_MAX_LOAD_FACTOR) // no need to add to capacity
    {
      index = complete_hash_func
          (hash_map->hash_func (in_pair->key), orig_cap);
      if (vector_push_back (hash_map->buckets[index], in_pair) == 0)
        {
          return 0;
        }
    }
  else // add to capacity
    {
      hash_map->capacity *= HASH_MAP_GROWTH_FACTOR;
      cap = (int) hash_map->capacity;
      if (reorganize_hash (hash_map, 1) == 0)
        {
          hash_map->capacity /= HASH_MAP_GROWTH_FACTOR;
          return 0;
        }
      index = complete_hash_func (hash_map->hash_func (in_pair->key), cap);
      if (vector_push_back (hash_map->buckets[index], in_pair) == 0)
        {
          hash_map->capacity /= HASH_MAP_GROWTH_FACTOR;
          return 0;
        }
    }
  hash_map->size++;
  return 1;
}

valueT hashmap_at (const hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL || key == NULL)
    {
      return NULL;
    }
  int cap = (int) hash_map->capacity;
  int index = complete_hash_func (hash_map->hash_func (key), cap);
  int vec_size = (int) hash_map->buckets[index]->size;
  pair *temp;
  for (int i = 0; i < vec_size; ++i)
    {
      temp = (pair *) hash_map->buckets[index]->data[i];
      if (temp->key_cmp (key, temp->key) == 1)
        {
          return temp->value;
        }
    }
  return NULL;
}

int hashmap_erase (hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL || key == NULL)
    {
      return 0;
    }
  if (hashmap_at (hash_map, key) == NULL)
    {
      return 0;
    }
  pair *temp;
  int cap = (int) hash_map->capacity;
  int index = complete_hash_func (hash_map->hash_func (key), cap);
  int vec_size = (int) hash_map->buckets[index]->size;
  if (hashmap_get_load_factor (hash_map)
      > HASH_MAP_MIN_LOAD_FACTOR
      || hash_map->capacity == HASH_MAP_MIN_SIZE) // no need to lower cap
    {
      for (int i = 0; i < vec_size; ++i)
        {
          temp = (pair *) hash_map->buckets[index]->data[i];
          if (temp->key_cmp (key, temp->key) == 1)
            {
//              size = (int) hash_map->buckets[index]->size;
              if (vector_erase (hash_map->buckets[index], i) == 0)
                {
                  return 0;
                }
              --hash_map->size;
              break;
            }
        }
    }
  else // lower cap
    {
      hash_map->capacity /= HASH_MAP_GROWTH_FACTOR;
      if (reorganize_hash (hash_map, 0) == 0)
        {
          hash_map->capacity *= HASH_MAP_GROWTH_FACTOR;
          return 0;
        }
      cap = (int) hash_map->capacity;
      index = complete_hash_func (hash_map->hash_func (key), cap);
      vec_size = (int) hash_map->buckets[index]->size;
      for (int i = 0; i < vec_size; ++i)
        {
          temp = (pair *) hash_map->buckets[index]->data[i];
          if (temp->key_cmp (key, temp->key) == 1)
            {
              if (vector_erase (hash_map->buckets[index], i) == 0)
                {
                  hash_map->capacity /= HASH_MAP_GROWTH_FACTOR;
                  return 0;
                }
              --hash_map->size;
              break;
            }
        }
    }
  return 1;

}

double hashmap_get_load_factor (const hashmap *hash_map)
{
  if (hash_map == NULL || hash_map->capacity == 0)
    {
      return -1;
    }
  return (double) hash_map->size / hash_map->capacity;
}

int
hashmap_apply_if (const hashmap *hash_map,
                  keyT_func keyT_func, valueT_func valT_func)
{
  if (hash_map == NULL || keyT_func == NULL || valT_func == NULL)
    {
      return -1;
    }
  pair *temp;
  int count = 0;
  int vec_size, hash_cap = (int) hash_map->capacity;
  for (int i = 0; i < hash_cap; ++i)
    {
      vec_size = (int) hash_map->buckets[i]->size;
      for (int j = 0; j < vec_size; ++j)
        {
          temp = hash_map->buckets[i]->data[j];
          if (keyT_func (temp->key) == 1)
            {
              valT_func (temp->value);
              ++count;
            }
        }
    }
  return count;
}