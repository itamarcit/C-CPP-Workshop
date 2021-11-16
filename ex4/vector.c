#include "vector.h"

#define VECTOR_MIN_SIZE 1
#define VEC_TRUE 1
#define VEC_FALSE 0
vector *
vector_alloc (vector_elem_cpy elem_copy_func, vector_elem_cmp elem_cmp_func,
              vector_elem_free elem_free_func)
{
  if (elem_copy_func == NULL || elem_free_func == NULL
      || elem_cmp_func == NULL)
    {
      return NULL;
    }
  vector *result = malloc (sizeof (vector));
  if (result == NULL)
    {
      return NULL;
    }
  result->elem_copy_func = elem_copy_func;
  result->elem_cmp_func = elem_cmp_func;
  result->elem_free_func = elem_free_func;
  result->capacity = VECTOR_INITIAL_CAP;
  result->size = 0;
  result->data = (void **) malloc (sizeof (void *) * VECTOR_INITIAL_CAP);
  if (result->data == NULL)
    {
      free (result);
      return NULL;
    }
  return result;
}

void vector_free (vector **p_vector)
{
  if (p_vector == NULL || *p_vector == NULL)
    {
      return;
    }
  vector_clear (*p_vector);
  free ((*p_vector)->data);
  free (*p_vector);
  *p_vector = NULL;
}

void *vector_at (const vector *vector, size_t ind)
{
  if (vector == NULL || ind >= vector->size)
    {
      return NULL;
    }
  return vector->data[ind];
}

int vector_find (const vector *vector, const void *value)
{
  if (vector == NULL || value == NULL)
    {
      return -1;
    }
  int vec_size = (int) vector->size;
  for (int i = 0; i < vec_size; ++i)
    {
      if (vector->elem_cmp_func ((vector->data)[i], value) == 1)
        {
          return i;
        }
    }
  return -1;
}

/**
 * lowers the capacity of the vector
 * @param vec given vector
 * @return 0 if failed, 1 if successful
 */
int lower_cap (vector *vec)
{
  void **temp = realloc (vec->data,
                         (vec->capacity / VECTOR_GROWTH_FACTOR)
                         * sizeof (void *));
  if (temp == NULL)
    {
      return 0;
    }
  vec->data = temp;
  temp = NULL;
  vec->capacity /= VECTOR_GROWTH_FACTOR;
  return 1;
}

int vector_push_back (vector *vector, const void *value)
{
  int flag = VEC_FALSE;
  if (vector == NULL || value == NULL)
    {
      return 0;
    }
  if (VECTOR_MAX_LOAD_FACTOR <= vector_get_load_factor (vector))
    {
      void **temp = realloc (vector->data,
                             (vector->capacity * VECTOR_GROWTH_FACTOR)
                             * sizeof (void *));
      if (temp == NULL)
        {
          return 0;
        }
      vector->data = temp;
      temp = NULL;
      vector->capacity *= VECTOR_GROWTH_FACTOR;
      flag = VEC_TRUE;
    }
  void *cpy = vector->elem_copy_func (value);
  if (cpy == NULL)
    {
      if (flag == VEC_TRUE)
        {
          lower_cap (vector);
        }
      return 0;
    }
  (vector->data)[vector->size] = cpy;
  ++vector->size;
  return 1;
}

double vector_get_load_factor (const vector *vector)
{
  if (vector == NULL || vector->capacity == 0)
    {
      return -1;
    }
  double result = (double) vector->size / vector->capacity;
  if (result < 0)
    {
      return -1;
    }
  return result;
}

int vector_erase (vector *vector, size_t ind)
{
  if (vector == NULL || ind >= vector->size)
    {
      return 0;
    }
  else
    {
      void *ptr = vector->data[ind];
      int vector_size = (int) vector->size;
      for (int i = ind; i < vector_size; ++i)
        {
          if (i == vector_size - 1)
            {
              vector->elem_free_func (&ptr);
              vector->data[vector_size - 1] = NULL;
              break;
            }
          (vector->data)[i] = (vector->data)[i + 1];
        }
      if (vector_get_load_factor (vector) <= VECTOR_MIN_LOAD_FACTOR
          && vector->capacity != VECTOR_MIN_SIZE)
        {
          void **temp = realloc (vector->data,
                                 (vector->capacity / VECTOR_GROWTH_FACTOR)
                                 * sizeof (void *));
          if (temp == NULL)
            {
              return 0;
            }
          vector->data = temp;
          temp = NULL;
          vector->capacity /= VECTOR_GROWTH_FACTOR;
        }
      --vector->size;
    }
  return 1;
}
/**
 * frees all the data inside the vector
 * @param vec given vector
 */
void
remove_vector (vector *vec) // used in order to remove the
// entire vector data without changing capacity
{
  int vector_size = (int) vec->size;
  for (int i = 0; i < vector_size; ++i)
    {
      vec->elem_free_func (&(vec->data[i]));
    }
}
void vector_clear (vector *vector)
{
  if (vector == NULL)
    {
      return;
    }
  remove_vector (vector);
  vector->size = 0;
  void **temp = (void **) realloc (vector->data,
                                   VECTOR_INITIAL_CAP * sizeof (void *));
  if (temp == NULL)
    {
      return;
    }
  vector->data = temp;
  temp = NULL;
  vector->capacity = VECTOR_INITIAL_CAP;
}