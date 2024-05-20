#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"
#include <climits>
#include <cstddef>
#include <memory>
namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template <typename T> class vector {
public:
  /**
   * TODO
   * a type for actions of the elements of a vector, and you should write
   *   a class named const_iterator with same interfaces.
   */
  /**
   * you can see RandomAccessIterator at CppReference for help.
   */

  class const_iterator;
  class iterator {
    // The following code is written for the C++ type_traits library.
    // Type traits is a C++ feature for describing certain properties of a type.
    // For instance, for an iterator, iterator::value_type is the type that the
    // iterator points to.
    // STL algorithms and containers may use these type_traits (e.g. the
    // following typedef) to work properly. In particular, without the following
    // code,
    // @code{std::sort(iter, iter1);} would not compile.
    // See these websites for more information:
    // https://en.cppreference.com/w/cpp/header/type_traits
    // About value_type:
    // https://blog.csdn.net/u014299153/article/details/72419713 About
    // iterator_category: https://en.cppreference.com/w/cpp/iteratover
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

  private:
    /**
     * TODO add data members
     *   just add whatever you want.
     */
    pointer ptr;
    pointer vec;

  public:
    /**
     * return a new iterator which pointer n-next elements
     * as well as operator-
     */
    iterator(pointer p, pointer vec) : ptr(p), vec(vec) {}
    iterator operator+(const int &n) const {
      // TODO
      iterator res(*this);
      res.ptr += n;
      return res;
    }
    iterator operator-(const int &n) const {
      // TODO
      iterator res(*this);
      res.ptr -= n;
      return res;
    }
    // return the distance between two iterators,
    // if these two iterators point to different vectors, throw
    // invaild_iterator.
    int operator-(const iterator &rhs) const {
      // TODO
      if (vec != rhs.vec)
        throw invalid_iterator();
      else
        return ptr - rhs.ptr;
    }
    iterator &operator+=(const int &n) {
      // TODO
      ptr += n;
      return *this;
    }
    iterator &operator-=(const int &n) {
      // TODO
      ptr -= n;
      return *this;
    }
    /**
     * TODO iter++
     */
    iterator operator++(int) {
      iterator res(*this);
      ptr++;
      return res;
    }
    /**
     * TODO ++iter
     */
    iterator &operator++() {
      ptr++;
      return *this;
    }
    /**
     * TODO iter--
     */
    iterator operator--(int) {
      iterator res(*this);
      ptr--;
      return res;
    }
    /**
     * TODO --iter
     */
    iterator &operator--() {
      ptr--;
      return *this;
    }
    /**
     * TODO *it
     */
    T &operator*() const { return *ptr; }
    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory address).
     */
    bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }
    bool operator==(const const_iterator &rhs) const { return ptr == rhs.ptr; }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
    bool operator!=(const const_iterator &rhs) const { return ptr != rhs.ptr; }
  };
  /**
   * TODO
   * has same function as iterator, just for a const object.
   */
  class const_iterator {
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

  private:
    /*TODO*/

    const T *ptr;
    const T *vec;

  public:
    const_iterator(const pointer p, const pointer vec) : ptr(p), vec(vec) {}
    operator iterator() const { return iterator(ptr, vec); }
    const_iterator operator+(const int &n) const {
      const_iterator res(*this);
      res.ptr += n;
      return res;
    }
    const_iterator operator-(const int &n) const {
      const_iterator res(*this);
      res.ptr -= n;
      return res;
    }
    int operator-(const const_iterator &rhs) const {
      if (vec != rhs.vec)
        throw invalid_iterator();
      return ptr - rhs.ptr;
    }
    const_iterator &operator+=(const int &n) {
      ptr += n;
      return *this;
    }
    const_iterator &operator-=(const int &n) {
      ptr -= n;
      return *this;
    }
    const_iterator operator++(int) {
      const_iterator res(*this);
      ptr++;
      return res;
    }
    const_iterator &operator++() {
      ptr++;
      return *this;
    }
    const_iterator operator--(int) {
      const_iterator res(*this);
      ptr--;
      return res;
    }
    const_iterator &operator--() {
      ptr--;
      return *this;
    }
    const T &operator*() const { return *ptr; }
    bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }
    bool operator==(const const_iterator &rhs) const { return ptr == rhs.ptr; }
    bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
    bool operator!=(const const_iterator &rhs) const { return ptr != rhs.ptr; }
  };
  size_t max_size, current_size;
  T *begin_ptr;
  /**
   * TODO Constructs
   * At least two: default constructor, copy constructor
   */
  std::allocator<T> alloc;
  vector() : max_size(0), current_size(0), begin_ptr(nullptr) {}
  vector(size_t n)
      : max_size(n), current_size(n), begin_ptr(alloc.allocate(n)) {}
  vector(size_t n, const T &value)
      : max_size(n), current_size(n), begin_ptr(alloc.allocate(n)) {
    for (size_t i = 0; i < n; i++) {
      std::construct_at(begin_ptr + i, value);
    }
  }
  vector(const vector &other) {
    max_size = other.max_size;
    current_size = other.current_size;
    begin_ptr = alloc.allocate(max_size);
    for (size_t i = 0; i < current_size; i++) {
      std::construct_at(begin_ptr + i, other.begin_ptr[i]);
    }
  }
  /**
   * TODO Destructor
   */
  ~vector() {
    for (int i = 0; i < current_size; i++) {
      std::destroy_at(begin_ptr + i);
    }
    alloc.deallocate(begin_ptr, max_size);
  }
  /**
   * TODO Assignment operator
   */
  vector &operator=(const vector &other) {
    if (this == &other)
      return *this;
    for (size_t i = 0; i < current_size; i++) {
      std::destroy_at(begin_ptr + i);
    }
    alloc.deallocate(begin_ptr, max_size);

    max_size = other.max_size;
    current_size = other.current_size;
    begin_ptr = alloc.allocate(max_size);
    for (size_t i = 0; i < current_size; i++) {
      std::construct_at(begin_ptr + i, other.begin_ptr[i]);
    }
    return *this;
  }
  /**
   * assigns specified element with bounds checking
   * throw index_out_of_bound if pos is not in [0, size)
   */
  T &at(const size_t &pos) {
    if (pos < 0 || pos >= current_size)
      throw index_out_of_bound();
    return *(begin_ptr + pos);
  }
  const T &at(const size_t &pos) const {
    if (pos < 0 || pos >= current_size)
      throw index_out_of_bound();
    return *(begin_ptr + pos);
  }
  /**
   * assigns specified element with bounds checking
   * throw index_out_of_bound if pos is not in [0, size)
   * !!! Pay attentions
   *   In STL this operator does not check the boundary but I want you to do.
   */
  T &operator[](const size_t &pos) {
    if (pos < 0 || pos >= current_size)
      throw index_out_of_bound();
    return *(begin_ptr + pos);
  }
  const T &operator[](const size_t &pos) const {
    if (pos < 0 || pos >= current_size)
      throw index_out_of_bound();
    return *(begin_ptr + pos);
  }
  /**
   * access the first element.
   * throw container_is_empty if size == 0
   */
  const T &front() const {
    if (current_size == 0)
      throw container_is_empty();
    return *begin_ptr;
  }
  /**
   * access the last element.
   * throw container_is_empty if size == 0
   */
  const T &back() const {
    if (current_size == 0)
      throw container_is_empty();
    return *(begin_ptr + current_size - 1);
  }
  /**
   * returns an iterator to the beginning.
   */
  iterator begin() { return iterator(begin_ptr, begin_ptr); }
  const_iterator cbegin() const { return const_iterator(begin_ptr, begin_ptr); }
  /**
   * returns an iterator to the end.
   */
  iterator end() { return iterator(begin_ptr + current_size, begin_ptr); }
  const_iterator cend() const {
    return const_iterator(begin_ptr + current_size, begin_ptr);
  }
  /**
   * checks whether the container is empty
   */
  bool empty() const { return current_size == 0; }
  /**
   * returns the number of elements
   */
  size_t size() const { return current_size; }
  /**
   * clears the contents
   */
  void clear() {
    for (int i = 0; i < (int)current_size; i++) {
      std::destroy_at(begin_ptr + i);
    }
    alloc.deallocate(begin_ptr, max_size);
    current_size = max_size = 0;
    begin_ptr = nullptr;
  }
  /**
   * inserts value before pos
   * returns an iterator pointing to the inserted value.
   */
  void grow_capacity() {
    // if(current_size>max_size){
    // 	// std::cerr<<"!!!!!";
    // 	exit(0);
    // }
    // getchar();
    // std::cerr<<max_size<<' '<<current_size	<<std::endl;
    size_t new_max_size = (max_size == 0) ? 1 : max_size * 2;
    T *new_begin_ptr = alloc.allocate(new_max_size);
    for (size_t i = 0; i < current_size; i++) {
      std::construct_at(new_begin_ptr + i, begin_ptr[i]);
    }
    for (size_t i = 0; i < current_size; i++) {
      std::destroy_at(begin_ptr + i);
    }
    alloc.deallocate(begin_ptr, max_size);
    max_size = new_max_size;
    begin_ptr = new_begin_ptr;
  }
  void shrink_capacity() {
    size_t new_max_size = max_size >> 1;
    T *new_begin_ptr = alloc.allocate(new_max_size);
    for (size_t i = 0; i < current_size; i++) {
      std::construct_at(new_begin_ptr + i, begin_ptr[i]);
    }
    for (int i = 0; i < current_size; i++) {
      std::destroy_at(begin_ptr + i);
    }
    alloc.deallocate(begin_ptr, max_size);
    max_size = new_max_size;
    begin_ptr = new_begin_ptr;
  }
  iterator insert(iterator pos, const T &value) {
    return insert(pos - begin(), value);
  }
  /**
   * inserts value at index ind.
   * after inserting, this->at(ind) == value
   * returns an iterator pointing to the inserted value.
   * throw index_out_of_bound if ind > size (in this situation ind can be size
   * because after inserting the size will increase 1.)
   */
  iterator insert(const size_t &ind, const T &value) {
    if (ind > current_size)
      throw index_out_of_bound();
    if (current_size >= max_size)
      grow_capacity();
    for (size_t i = current_size; i > ind; i--) {
      begin_ptr[i] = begin_ptr[i - 1];
    }
    begin_ptr[ind] = value;
    current_size++;
    return iterator(begin_ptr + ind, begin_ptr);
  }
  /**
   * removes the element at pos.
   * return an iterator pointing to the following element.
   * If the iterator pos refers the last element, the end() iterator is
   * returned.
   */
  iterator erase(iterator pos) { return erase(pos - begin()); }
  /**
   * removes the element with index ind.
   * return an iterator pointing to the following element.
   * throw index_out_of_bound if ind >= size
   */
  iterator erase(const size_t &ind) {
    if (ind >= current_size)
      throw index_out_of_bound();
    for (size_t i = ind; i < current_size - 1; i++) {
      begin_ptr[i] = begin_ptr[i + 1];
    }
    current_size--;
    if (current_size <= (max_size >> 2))
      shrink_capacity();
    return iterator(begin_ptr + ind, begin_ptr);
  }
  /**
   * adds an element to the end.
   */
  void push_back(const T &value) {
    if (current_size >= max_size)
      grow_capacity();
    std::construct_at(begin_ptr + current_size, value);
    current_size++;
  }
  /**
   * remove the last element from the end.
   * throw container_is_empty if size() == 0
   */
  void pop_back() {
    if (current_size == 0)
      throw container_is_empty();
    std::destroy_at(begin_ptr + current_size - 1);
    current_size--;
    if (current_size <= (max_size >> 2))
      shrink_capacity();
  }
};

} // namespace sjtu

#endif
