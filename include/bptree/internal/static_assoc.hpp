/************************************************
 *  static_assoc.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#ifndef BPTREE_INTERNAL_STATIC_ASSOC_HPP_
#define BPTREE_INTERNAL_STATIC_ASSOC_HPP_

#include <cstddef>

#include "./static_vector.hpp"

namespace bptree {

namespace internal {

/************************************************
 * Declaration: class static_assoc<T, U, N>
 ************************************************/

template <typename ValueTraits, bool Unique, std::size_t N>
class static_assoc
  : public ValueTraits,
    private ValueTraits::value_compare {
 private:  // Private Type(s)
    using value_traits = ValueTraits;
    using underlying_type = static_vector<typename value_traits::value_type, N>;

 public:  // Public Type(s)
    using key_type = typename value_traits::key_type;
    using value_type = typename value_traits::value_type;
    using key_compare = typename value_traits::key_compare;
    using value_compare = typename value_traits::value_compare;

    using reference = typename underlying_type::reference;
    using const_reference = typename underlying_type::const_reference;
    using pointer = typename underlying_type::pointer;
    using const_pointer = typename underlying_type::const_pointer;
    using size_type = typename underlying_type::size_type;
    using difference_type = typename underlying_type::difference_type;

    using iterator = typename underlying_type::iterator;
    using const_iterator = typename underlying_type::const_iterator;
    using reverse_iterator = typename underlying_type::reverse_iterator;
    using const_reverse_iterator = typename underlying_type::const_reverse_iterator;

 public:  // Public Method(s)
    static_assoc();
    explicit static_assoc(key_compare comp);

    bool empty() const noexcept;
    bool full() const noexcept;
    size_type size() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr size_type capacity() const noexcept;

    key_compare key_comp() const;
    value_compare value_comp() const;

 private:  // Private Property(ies)
    underlying_type values_;
};

/************************************************
 * Implementation: class static_assoc<T, U, N>
 ************************************************/

template <typename T, bool U, std::size_t N>
inline static_assoc<T, U, N>::static_assoc()
  : static_assoc(key_compare()) {
    // do nothing
}

template <typename T, bool U, std::size_t N>
inline static_assoc<T, U, N>::static_assoc(key_compare comp)
  : value_compare(comp), values_() {
    // do nothing
}

template <typename T, bool U, std::size_t N>
inline bool static_assoc<T, U, N>::empty() const noexcept {
    return values_.empty();
}

template <typename T, bool U, std::size_t N>
inline bool static_assoc<T, U, N>::full() const noexcept {
    return values_.full();
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::size_type
static_assoc<T, U, N>::size() const noexcept {
    return values_.size();
}

template <typename T, bool U, std::size_t N>
inline constexpr typename static_assoc<T, U, N>::size_type
static_assoc<T, U, N>::max_size() const noexcept {
    return values_.max_size();
}

template <typename T, bool U, std::size_t N>
inline constexpr typename static_assoc<T, U, N>::size_type
static_assoc<T, U, N>::capacity() const noexcept {
    return values_.capacity();
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::key_compare
static_assoc<T, U, N>::key_comp() const {
    return key_compare(*this);
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::value_compare
static_assoc<T, U, N>::value_comp() const {
    return value_compare(*this);
}

}  // namespace internal

}  // namespace bptree

#endif  // BPTREE_INTERNAL_STATIC_ASSOC_HPP_
