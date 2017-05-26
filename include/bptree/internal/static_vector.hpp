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
    static_vector() noexcept;

    bool empty() const noexcept;
    bool full() const noexcept;
    size_type size() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr size_type capacity() const noexcept;

 private:  // Private Property(ies)
    size_type size_;
};

/************************************************
 * Implementation: class static_vector<T, N>
 ************************************************/

template <typename T, std::size_t N>
inline static_vector<T, N>::static_vector() noexcept
  : size_(0) {
    // do nothing
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
