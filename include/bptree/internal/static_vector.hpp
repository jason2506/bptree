/************************************************
 *  static_vector.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#ifndef BPTREE_INTERNAL_STATIC_VECTOR_HPP_
#define BPTREE_INTERNAL_STATIC_VECTOR_HPP_

#include <cassert>
#include <cstddef>

#include <memory>
#include <stdexcept>
#include <type_traits>

namespace bptree {

namespace internal {

/************************************************
 * Declaration: class static_vector<T, N>
 ************************************************/

template <typename T, std::size_t N>
class static_vector {
 public:  // Public Type(s)
    using value_type = T;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

 public:  // Public Method(s)
    static_vector();
    explicit static_vector(size_type count);
    static_vector(size_type count, value_type const& value);
    template <typename InputIt>
    static_vector(InputIt first, InputIt last);
    static_vector(std::initializer_list<value_type> il);
    static_vector(static_vector const& other);
    ~static_vector();

    static_vector& operator=(std::initializer_list<value_type> il);
    static_vector& operator=(static_vector const& other);
    void assign(size_type count, value_type const& value);
    void assign(std::initializer_list<value_type> il);
    template <typename InputIt>
    void assign(InputIt first, InputIt last);

    void push_back(value_type const& value);
    void push_back(value_type&& value);
    void clear() noexcept;

    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    value_type* data() noexcept;
    value_type const* data() const noexcept;

    bool empty() const noexcept;
    bool full() const noexcept;
    size_type size() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr size_type capacity() const noexcept;

 private:  // Private Property(ies)
    size_type size_;
    std::aligned_storage_t<sizeof(T), alignof(T)> data_[N];
};

/************************************************
 * Implementation: class static_vector<T, N>
 ************************************************/

template <typename T, std::size_t N>
inline static_vector<T, N>::static_vector()
  : size_(0), data_() {
    // do nothing
}

template <typename T, std::size_t N>
inline static_vector<T, N>::static_vector(size_type count)
  : size_(count), data_() {
    assert(count <= max_size());

    auto first = data();
    auto last = first + size_;
    auto current = first;
    try {
        for (; current != last; ++current) {
            ::new(current) value_type;
        }
    } catch (...) {
        for (; first != current; ++first) {
            first->~value_type();
        }
        throw;
    }
}

template <typename T, std::size_t N>
inline static_vector<T, N>::static_vector(size_type count, value_type const& value)
  : size_(count), data_() {
    assert(count <= max_size());

    auto first = data();
    auto last = first + size_;
    std::uninitialized_fill(first, last, value);
}

template <typename T, std::size_t N>
template <typename InputIt>
inline static_vector<T, N>::static_vector(InputIt first, InputIt last)
  : static_vector() {
    auto data_first = data();
    auto data_last = std::uninitialized_copy(first, last, data_first);
    size_ = data_last - data_first;
    assert(size_ <= max_size());
}

template <typename T, std::size_t N>
inline static_vector<T, N>::static_vector(std::initializer_list<value_type> il)
  : static_vector(il.begin(), il.end()) {
    // do nothing
}

template <typename T, std::size_t N>
inline static_vector<T, N>::static_vector(static_vector const& other)
  : static_vector(other.data(), other.data() + other.size()) {
    // do nothing
}

template <typename T, std::size_t N>
inline static_vector<T, N>::~static_vector() {
    clear();
}

template <typename T, std::size_t N>
inline static_vector<T, N>& static_vector<T, N>::operator=(std::initializer_list<value_type> il) {
    assign(il);
    return *this;
}

template <typename T, std::size_t N>
inline static_vector<T, N>& static_vector<T, N>::operator=(static_vector const& other) {
    assign(other.data(), other.data() + other.size());
    return *this;
}

template <typename T, std::size_t N>
inline void static_vector<T, N>::assign(size_type count, value_type const& value) {
    assert(count <= max_size());

    clear();

    auto first = data();
    auto last = first + count;
    size_ = count;
    std::uninitialized_fill(first, last, value);
}

template <typename T, std::size_t N>
inline void static_vector<T, N>::assign(std::initializer_list<value_type> il) {
    assign(il.begin(), il.end());
}

template <typename T, std::size_t N>
template <typename InputIt>
inline void static_vector<T, N>::assign(InputIt first, InputIt last) {
    clear();

    auto ptr = std::uninitialized_copy(first, last, data());
    size_ = ptr - data();
    assert(size_ <= max_size());
}

template <typename T, std::size_t N>
inline void static_vector<T, N>::push_back(value_type const& value) {
    assert(size_ < max_size());
    ::new(data() + size_) value_type(value);
    ++size_;
}

template <typename T, std::size_t N>
inline void static_vector<T, N>::push_back(value_type&& value) {
    assert(size_ < max_size());
    ::new(data() + size_) value_type(std::move(value));
    ++size_;
}

template <typename T, std::size_t N>
inline void static_vector<T, N>::clear() noexcept {
    for (size_type pos = 0; pos < size(); ++pos) {
        at(pos).~value_type();
    }

    size_ = 0;
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::reference
static_vector<T, N>::operator[](size_type pos) {
    assert(pos < size());
    return *(data() + pos);
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::const_reference
static_vector<T, N>::operator[](size_type pos) const {
    assert(pos < size());
    return *(data() + pos);
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::reference static_vector<T, N>::at(size_type pos) {
    return const_cast<reference>(
        static_cast<static_vector const*>(this)->at(pos));
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::const_reference static_vector<T, N>::at(size_type pos) const {
    if (pos >= size()) {
        throw std::out_of_range("index out of bounds");
    }

    return operator[](pos);
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::value_type* static_vector<T, N>::data() noexcept {
    return reinterpret_cast<value_type*>(data_);
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::value_type const* static_vector<T, N>::data() const noexcept {
    return reinterpret_cast<value_type const*>(data_);
}

template <typename T, std::size_t N>
inline bool static_vector<T, N>::empty() const noexcept {
    return size() == 0;
}

template <typename T, std::size_t N>
inline bool static_vector<T, N>::full() const noexcept {
    return size() >= max_size();
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::size_type static_vector<T, N>::size() const noexcept {
    return size_;
}

template <typename T, std::size_t N>
inline constexpr typename static_vector<T, N>::size_type
static_vector<T, N>::max_size() const noexcept {
    return N;
}

template <typename T, std::size_t N>
inline constexpr typename static_vector<T, N>::size_type
static_vector<T, N>::capacity() const noexcept {
    return max_size();
}

}  // namespace internal

}  // namespace bptree

#endif  // BPTREE_INTERNAL_STATIC_VECTOR_HPP_
