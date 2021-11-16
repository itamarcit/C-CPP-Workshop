//
// Created by Itamar on 21/06/2021.
//
#include "vl_vector.h"
#ifndef _VL_STRING_H_
#define _VL_STRING_H_
#include "cstring"
/* vec_size, _vec_capacity, _type, _heap_data, _static_data */
template<size_t StaticCapacity = DEF_STATIC_CAPACITY>
class vl_string : public vl_vector<char, StaticCapacity> {
 private:

 public:
  /* vl_string Constructors */
  vl_string () : vl_vector<char, StaticCapacity> () // Default constructor
  {
    if (StaticCapacity > 0)
      {
        this->_static_data[0] = '\0';
      }
    else
      {
        size_t new_cap = this->check_capacity (1);
        this->add_capacity (new_cap);
        this->_type = HEAP;
      }
    ++this->_vec_size;
  }
  vl_string (const vl_string &other) : vl_vector<char, StaticCapacity> (other)
  {} // Copy constructor
  vl_string (const char *other) : vl_vector<char, // Implicit Constructor
      StaticCapacity> (other,
                       other + strlen (other) + 1)
  {}
  /* Public Functions */
  /**
   * Same as vl_vector push_back, but adds the element before the \0
   * @param element Element to be added
   */
  void push_back (const char &element) override
  {
    if (this->_vec_size + 1 > this->_vec_capacity)
      {
        size_t new_cap = this->check_capacity (1);
        this->add_capacity (new_cap);
      }
    this->data ()[this->_vec_size] = '\0';
    this->data ()[this->_vec_size - 1] = char (element);
    ++this->_vec_size;
  }
  /**
   * Same as vl_vector's pop_back but doesn't remove the \0 element
   */
  void pop_back () override
  {
    if (this->_vec_size == 1)
      {
        return;
      }
    if (this->_type == HEAP && this->_vec_size - 1 <= StaticCapacity)
      {
        size_t remaining = this->_vec_size - 1;
        this->back_to_static (remaining);
      }
    --this->_vec_size;
    this->data ()[this->_vec_size - 1] = '\0';
  }
  /**
   * Same as vl_vector's size, but doesn't count \0.
   * @return Vector's size without the null terminator
   */
  size_t size () const override
  {
    return this->_vec_size - 1;
  }
  /**
   * Same as vl_vector's empty but doesn't count \0
   * @return True if vector's empty, false otherwise
   */
  bool empty () const override
  {
    return (this->_vec_size == 1);
  }
  /**
   * Clears the entire vector, except the \0
   */
  void clear () override
  {
    this->_vec_size = 1;
    if (StaticCapacity == 0)
      {
        this->data ()[0] = '\0';
        return;
      }
    if (this->_type == HEAP)
      {
        delete[] this->_heap_data;
      }
    this->_vec_size = 1;
    this->_vec_capacity = StaticCapacity;
    this->_type = STACK;
    this->data ()[0] = '\0';
  }
  //  /**
  //   * Same as vl_vector's at, but the boundaries don't include \0
  //   */
  //  char &at (int index) override
  //  {
  //    if (index < 0 || (size_t) index >= this->_vec_size - 1)
  //      {
  //        throw std::out_of_range (OUT_OF_RANGE_ERR);
  //      }
  //    return this->data()[index];
  //  }
  //  /**
  //   * Same as vl_vector's at, but the boundaries don't include \0
  //   */
  //  const char &at (int index) const
  //  {
  //    if (index < 0 || (size_t) index >= this->_vec_size - 1)
  //      {
  //        throw std::out_of_range (OUT_OF_RANGE_ERR);
  //      }
  //    return this->data()[index];
  //  }
  /**
   * Checks if the vector contains a sequence of chars in it.
   * @param elem Sequence of chars
   * @return True if it contains it, false otherwise
   */
  bool contains (const char *elem) const
  {
    return (strstr (this->data (), elem) != nullptr);
  }
  /* vl_string Operators */
  vl_string &operator+= (const vl_string &rhs)
  {
    if (this->_vec_size + rhs._vec_size > this->capacity ())
      {
        size_t new_cap = this->check_capacity (rhs.size ());
        this->add_capacity (new_cap);
      }
    strcat (this->data (), rhs.data ());
    this->_vec_size += rhs.size ();
    return *this;
  }
  vl_string &operator+= (const char *rhs)
  {
    vl_string<StaticCapacity> val = rhs;
    *this += val;
    return *this;
  }
  vl_string &operator+= (const char rhs)
  {
    this->push_back (rhs);
    return *this;
  }
  vl_string operator+ (const vl_string &rhs) const
  {
    vl_string<StaticCapacity> val = *this;
    val += rhs;
    return val;
  }
  vl_string operator+ (const char *rhs) const
  {
    vl_string<StaticCapacity> val = *this;
    val += rhs;
    return val;
  }
  vl_string operator+ (const char rhs) const
  {
    vl_string<StaticCapacity> val = *this;
    val += rhs;
    return val;
  }
  operator char * ()
  {
    return (char *) this->data ();
  }
  operator char * () const
  {
    return (char *) this->data ();
  }
};

#endif //_VL_STRING_H_
