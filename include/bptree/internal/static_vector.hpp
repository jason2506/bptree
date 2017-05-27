/************************************************
 *  static_vector.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#ifndef BPTREE_INTERNAL_STATIC_VECTOR_HPP_
#define BPTREE_INTERNAL_STATIC_VECTOR_HPP_

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

    reference at(size_type pos);
    const_reference at(size_type pos) const;

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
    auto first = reinterpret_cast<value_type*>(data_);
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
    auto first = reinterpret_cast<value_type*>(data_);
    auto last = first + size_;
    std::uninitialized_fill(first, last, value);
}

template <typename T, std::size_t N>
template <typename InputIt>
inline static_vector<T, N>::static_vector(InputIt first, InputIt last)
  : static_vector() {
    auto data_first = reinterpret_cast<value_type*>(data_);
    auto data_last = std::uninitialized_copy(first, last, data_first);
    size_ = data_last - data_first;
}

template <typename T, std::size_t N>
inline static_vector<T, N>::static_vector(std::initializer_list<value_type> il)
  : static_vector(il.begin(), il.end()) {
    // do nothing
}

template <typename T, std::size_t N>
inline static_vector<T, N>::static_vector(static_vector const& other)
  : static_vector(reinterpret_cast<value_type const*>(other.data_),
                  reinterpret_cast<value_type const*>(other.data_ + other.size())) {
    // do nothing
}

template <typename T, std::size_t N>
inline static_vector<T, N>::~static_vector() {
    for (size_type pos = 0; pos < size(); ++pos) {
        at(pos).~value_type();
    }

    size_ = 0;
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

    return *reinterpret_cast<value_type const*>(data_ + pos);
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
