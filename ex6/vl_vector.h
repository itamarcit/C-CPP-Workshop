//
// Created by Itamar on 19/06/2021.
//
#include <iostream>
#include <algorithm>
#include <iterator>
#include <exception>
#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_
#define DEF_STATIC_CAPACITY 16
#define CAPACITY_THREE 3
#define CAPACITY_TWO 2
#define OUT_OF_RANGE_ERR "Exception: Index out of range."
enum data_type {
    STACK,
    HEAP
};

/**
 * Checks the distance between two forward iterators
 * @tparam ForwardIterator Template for a Forward Iterator
 * @param first Forward Iterator, which is before last (or equal)
 * @param last Forward Iterator which is after first (or equal)
 * @return Distance between the iterators
 */
template<class ForwardIterator>
size_t check_distance (ForwardIterator first, ForwardIterator last)
{
  size_t counter = 0;
  while (first != last)
    {
      first++;
      counter++;
    }
  return counter;
}

/**
 * vl_vector class
 * @tparam T Generic value
 * @tparam StaticCapacity Max static capacity for the vector
 */
template<class T, size_t StaticCapacity = DEF_STATIC_CAPACITY>
class vl_vector {
 protected:
  size_t _vec_size{};
  size_t _vec_capacity{};
  data_type _type;
  T *_heap_data;
  T _static_data[StaticCapacity];

 public:
  /* Begin and End */
  using const_iterator = const T *;
  using iterator = T *;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  iterator begin ()
  {
    return data ();
  }
  iterator end ()
  {
    return data () + _vec_size;
  }
  const_iterator begin () const
  {
    return data ();
  }
  const_iterator end () const
  {
    return data () + _vec_size;
  }
  const_iterator cbegin () const
  {
    return data ();
  }
  const_iterator cend () const
  {
    return data () + _vec_size;
  }
  reverse_iterator rbegin ()
  {
    return reverse_iterator (end ());
  }
  const_reverse_iterator rbegin () const
  {
    return const_reverse_iterator (end ());
  }
  const_reverse_iterator crbegin () const
  {
    return const_reverse_iterator (cend ());
  }
  reverse_iterator rend ()
  {
    return reverse_iterator (begin ());
  }
  const_reverse_iterator rend () const
  {
    return const_reverse_iterator (begin ());
  }
  const_reverse_iterator crend () const
  {
    return const_reverse_iterator (cbegin ());
  }
 protected:
  /* Protected Functions */
  /**
 * Gets more capacity for INSERT 2
 * @param pos const_iterator
 * @param amount_to_add size_t
 * @return const_iterator to the new data pointer
 */
  const_iterator get_more_capacity (const_iterator pos, size_t amount_to_add)
  {
    size_t new_index_jump = 0;
    bool was_stack = false;
    if (_type == STACK)
      {
        was_stack = true;
      }
    else
      {
        new_index_jump = this->get_new_ptr_index (pos);
      }
    size_t new_cap = check_capacity (amount_to_add);
    this->add_capacity (new_cap);
    if (was_stack) // Fix the position of pos accordingly to the heap/stack
      {
        pos = this->convert_stack_to_heap (pos);
      }
    else
      {
        pos = this->_heap_data + new_index_jump;
      }
    return pos;
  }
  /**
   * Checks if two vectors are equal (by size, and each element)
   * @param v1 First vector
   * @param rhs Second vector
   * @return True if equal, false otherwise
   */
  bool are_vectors_equal (const vl_vector &rhs) const
  {
    if (_vec_size != rhs._vec_size)
      {
        return false;
      }
    for (size_t i = 0; i < _vec_size; ++i)
      {
        if (this->data ()[i] != rhs.data ()[i])
          {
            return false;
          }
      }
    return true;
  }
  /**
   * Gets the index of the heap_ptr compared to data's begin
   * @param heap_ptr const iterator pointing to a place in data
   * @return the distance from begin to heap_ptr
   */
  size_t get_new_ptr_index (const_iterator heap_ptr)
  {
    const_iterator it = this->_heap_data;
    size_t result = 0;
    while (it != heap_ptr)
      {
        ++it, ++result;
      }
    return result;
  }
  /**
   * Gets an iterator to the stack based on an iterator to the heap with the
   * same distance to begin
   * @param heap_ptr iterator to a place in the heap
   * @return iterator to a place in the stack
   */
  const_iterator convert_heap_to_stack (const_iterator heap_ptr) const
  {
    const_iterator result = this->_heap_data;
    int result_index = 0;
    while (result != heap_ptr)
      {
        ++result_index, ++result;
      }
    return this->_static_data + result_index;
  }
  /**
   * Converts an iterator to the stack to point to the same spot in the heap
   * @param stack_ptr Pointer to a stack element
   * @return const_iterator which is a pointer to a place in the heap
   */
  const_iterator convert_stack_to_heap (const_iterator stack_ptr) const
  {
    const_iterator result = this->_static_data;
    int result_index = 0;
    while (result != stack_ptr)
      {
        ++result_index, result++;
      }
    return this->_heap_data + result_index;
  }
  /**
   * Converts the vector's data back to static data. Copies the remaining
   * elements to the stack
   * @param remaining_elements Remaining elements to copy when lowering the
   * size
   */
  void back_to_static (size_t remaining_elements)
  {
    iterator beg = this->begin ();
    iterator end = beg + remaining_elements;
    iterator result = _static_data;
    std::copy (beg, end, result);
    _vec_capacity = StaticCapacity;
    _type = STACK;
    delete[] _heap_data;
    _heap_data = nullptr;
  }
  /**
   * Adds capacity accordingly with new_cap, copies all elements to the new
   * data spot
   * @param new_cap New capacity
   */
  void add_capacity (size_t new_cap)
  {
    T *temp;
    iterator old = this->begin ();
    iterator old_end = this->end ();
    temp = new T[new_cap];
    iterator copied = temp;
    std::copy (old, old_end, copied);
    if (_type == HEAP)
      {
        delete[] _heap_data;
      }
    else //if (_type == STACK)
      {
        _type = HEAP;
      }
    _heap_data = temp;
    _vec_capacity = new_cap;
  }
  /**
   * Applies Cap_C function
   * @param elements_to_add Elements being currently added
   * @return Cap_C result
   */
  size_t check_capacity (int elements_to_add) const
  {
    if (_vec_size + elements_to_add <= StaticCapacity)
      {
        return StaticCapacity;
      }
    return (float) CAPACITY_THREE / CAPACITY_TWO
           * (_vec_size + elements_to_add);
  }
 public:
  /* vl_vector Constructors */
  vl_vector () // Default Constructor
      : _vec_size (0), _vec_capacity (StaticCapacity), _type (STACK),
        _heap_data (nullptr)
  {}
  vl_vector (const vl_vector &other) // Copy Constructor
      : _vec_size (other._vec_size), _vec_capacity (other._vec_capacity),
        _type (other._type)
  {
    const_iterator beg = other.begin ();
    const_iterator end = other.end ();
    if (_type == HEAP)
      {
        _heap_data = new T[_vec_capacity]; // handle exceptions`?`
      }
    std::copy (beg, end, data ());
  }
  /**
   * Constructor which adds the elements from first to last to the vector
   * Copies the elements into it
   * @tparam ForwardIterator Forward iterator of elements to be added
   * @param first First element in to be added (iterator)
   * @param last Last element to be added (iterator)
   */
  template<class ForwardIterator>
  vl_vector (ForwardIterator first, ForwardIterator last)
      : _vec_size (0), _vec_capacity (StaticCapacity), _type (STACK),
        _heap_data (nullptr)
  {
    size_t amount_to_add = check_distance (first, last);
    int index = 0;
    if (amount_to_add > StaticCapacity)
      {
        size_t new_cap = this->check_capacity (amount_to_add);
        this->add_capacity (new_cap);
      }
    while (first != last)
      {
        data ()[index] = *first;
        ++index, ++first, ++_vec_size;
      }
  }
  /**
   * Single value initialized constructor
   * @param count Amount of times v is to be added
   * @param v element to be added count times
   */
  vl_vector (unsigned int count, const T &v)
      : _vec_size (0), _vec_capacity (StaticCapacity), _type (STACK),
        _heap_data (nullptr)
  {
    if (count > StaticCapacity)
      {
        size_t new_cap = this->check_capacity (count);
        this->add_capacity (new_cap);
      }
    for (size_t i = 0; i < count; ++i)
      {
        data ()[i] = T (v);
        _vec_size++;
      }

  }
  virtual ~vl_vector () // Destructor
  {
    if (_type == HEAP)
      {
        delete[] _heap_data;
        _heap_data = nullptr;
      }
  }
  /* vl_vector Operators */
  vl_vector &operator= (const vl_vector<T> &other)
  {
    if (this == &other)
      {
        return *this;
      }
    _vec_size = other._vec_size;
    _vec_capacity = other._vec_capacity;
    _type = other._type;
    if (_type == HEAP)
      {
        if (_heap_data != nullptr)
          {
            delete[] _heap_data;
          }
        _heap_data = new T[_vec_capacity];
      }
    for (size_t i = 0; i < _vec_size; ++i)
      {
        data ()[i] = other.data ()[i];
      }
    return *this;
  }
  bool operator== (const vl_vector &rhs) const
  {
    return this->are_vectors_equal (rhs);
  }
  bool operator!= (const vl_vector &rhs) const
  {
    return !(this->are_vectors_equal (rhs));
  }
  T &operator[] (size_t index)
  {
    return data ()[index];
  }
  const T &operator[] (size_t index) const
  {
    return data ()[index];
  }
  /* Getters */
  virtual size_t size () const
  {
    return _vec_size;
  }
  size_t capacity () const
  {
    return _vec_capacity;
  }
  T *data ()
  {
    if (_type == HEAP)
      {
        return _heap_data;
      }
    else
      {
        return _static_data;
      }
  }
  const T *data () const
  {
    if (_type == HEAP)
      {
        return _heap_data;
      }
    else
      {
        return _static_data;
      }
  }
  /* Public Functions */
  /**
   * Checks if the vector's empty
   * @return True if the vector's empty, false otherwise
   */
  virtual bool empty () const
  {
    return (_vec_size == 0);
  }
  /**
   * Returns the reference of the vector in index
   * @param index Index to be returned
   * @return Reference to T of vector at index
   */
  virtual T &at (int index)
  {
    if (index < 0 || (size_t) index >= _vec_size)
      {
        throw std::out_of_range (OUT_OF_RANGE_ERR);
      }
    return data ()[index];
  }
  /**
   * Returns the value of the vector in index
   * @param index Index to be returned
   * @return Value of T of vector at index
   */
  virtual const T &at (int index) const
  {
    if (index < 0 || (size_t) index >= _vec_size)
      {
        throw std::out_of_range (OUT_OF_RANGE_ERR);
      }
    return data ()[index];
  }
  /**
   * Adds a new element to the end of the vector
   * @param element Element to be added
   */
  virtual void push_back (const T &element)
  {
    if (_vec_size + 1 > _vec_capacity)
      {
        size_t new_cap = this->check_capacity (1);
        this->add_capacity (new_cap);
      }
    data ()[_vec_size] = T (element);
    ++_vec_size;
  }

  /**
   * Adds a new element at pos's position
   * @param pos Iterator that points to an element in vector
   * @param elem Element to be added
   * @return Iterator that points to the newly added element
   */
  /* INSERT 1 */
  iterator insert (const_iterator pos, T elem)
  {
    iterator return_val = iterator (pos);
    int jump;
    bool was_stack;
    if (_vec_size + 1 > _vec_capacity)
      {
        if (_type == STACK)
          {
            was_stack = true;
          }
        else
          {
            was_stack = false;
            jump = get_new_ptr_index (pos);
          }
        size_t new_cap = this->check_capacity (1);
        this->add_capacity (new_cap);
        if (was_stack)
          {
            pos = convert_stack_to_heap (pos);
          }
        else
          {
            pos = this->_heap_data + jump;
          }
        return_val = iterator (pos);
      }
    iterator beg = this->begin ();
    iterator end = this->end ();
    int index = 0, test_index = _vec_size;
    while (beg != pos)
      {
        index++;
        beg++;
      }
    while (beg != end)
      {
        data ()[test_index] = data ()[test_index - 1];
        ++beg, --test_index;
      }
    data ()[index] = T (elem);
    ++_vec_size;
    return return_val;
  }
  /**
   * Adds first - last elements to the vector at position
   * @tparam ForwardIterator Forward Iterator template
   * @param pos Iterator that points to an element in vector
   * @param first Forward Iterator that points to the first element to be added
   * @param last Forward Iterator that points to the last element to be added
   * @return Iterator that points to the first newly added element
   */
/* INSERT 2 */
  template<class ForwardIterator>
  iterator
  insert (const_iterator pos, ForwardIterator first, ForwardIterator last)
  {
    size_t amount_to_add = check_distance (first, last);
    if (amount_to_add + _vec_size > _vec_capacity)
      {
        pos = get_more_capacity (pos, amount_to_add);
      }
    iterator beg = this->begin (), end = this->end (),
        return_val = iterator (pos);
    int index = pos - beg, test_index = _vec_size - 1;
    while (pos != end)
      {
        data ()[test_index + amount_to_add] = data ()[test_index];
        --end, --test_index;
      }
    while (first != last)
      {
        data ()[index] = *first;
        ++index, ++first;
      }
    _vec_size += amount_to_add;
    return return_val;
  }
  virtual void pop_back ()
  {
    if (_vec_size == 0)
      {
        return;
      }
    if (_type == HEAP && _vec_size - 1 <= StaticCapacity)
      {
        size_t remaining = _vec_size - 1;
        this->back_to_static (remaining);
      }
    --_vec_size;
  }
  /**
   * Removes the element at pos
   * @param pos Iterator to an element in the vector
   * @return Iterator to the element to the right of the removed elem
   */
/* ERASE 1 */
  iterator erase (const_iterator pos)
  {
    if (_vec_size == 0)
      {
        return iterator (pos);
      }
    iterator return_val = iterator (pos);
    iterator beg = this->begin ();
    iterator end = this->end ();
    int index = 0;
    while (beg != pos)
      {
        return_val = ++beg;
        ++index;
      }
    while (pos != end)
      {
        data ()[index] = data ()[index + 1];
        ++pos, ++index;
      }
    if (_type == HEAP && _vec_size - 1 <= StaticCapacity)
      {
        return_val = iterator (convert_heap_to_stack (return_val));
        size_t remaining = _vec_size - 1;
        this->back_to_static (remaining);
      }
    --_vec_size;
    return return_val;
  }
  /**
   * Removes first - last elements from vector. first needs to be before last
   * @param first Iterator to the first element to be removed
   * @param last Iterator to the last element to be removed
   * @return Iterator to the right of the last removed element
   */
/* ERASE 2 */
  iterator erase (const_iterator first, const_iterator last)
  {
    size_t amount_to_remove = check_distance (first, last);
    iterator return_val = iterator (first);
    iterator beg = this->begin ();
    iterator end = this->end ();
    int index = 0;
    index = check_distance (beg, iterator (first));
    beg = iterator (last);
    while (beg != end)
      {
        data ()[index] = data ()[index + amount_to_remove];
        ++index, ++beg;
      }
    if (_type == HEAP && _vec_size - amount_to_remove <= StaticCapacity)
      {
        return_val = iterator (convert_heap_to_stack (return_val));
        this->back_to_static (_vec_size - amount_to_remove);
      }
    _vec_size -= amount_to_remove;
    return return_val;
  }
  /**
   * Deletes all elements in the vector
   */
  virtual void clear ()
  {
    if (_type == HEAP)
      {
        delete[] _heap_data;
        _heap_data = nullptr;
      }
    _vec_size = 0;
    _vec_capacity = StaticCapacity;
    _type = STACK;
  }
  /**
   * Checks if elem is in the vector
   * @param elem Given element
   * @return True if vector contains elem, false otherwise
   */
  bool contains (const T &elem) const
  {
    for (size_t i = 0; i < _vec_size; ++i)
      {
        if (elem == data ()[i])
          {
            return true;
          }
      }
    return false;
  }

};
#endif //_VL_VECTOR_H_

int a = 0;
int b = 0;
std::cout << ++a;
  std::cout << b++;