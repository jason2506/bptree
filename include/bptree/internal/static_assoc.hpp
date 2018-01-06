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
    template <typename InputIt>
    static_assoc(InputIt first, InputIt last, key_compare const& comp = key_compare());
    static_assoc(std::initializer_list<value_type> il, key_compare const& comp = key_compare());
    static_assoc(static_assoc const&) = default;
    static_assoc(static_assoc&&) = default;

    bool empty() const noexcept;
    bool full() const noexcept;
    size_type size() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr size_type capacity() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    key_compare key_comp() const;
    value_compare value_comp() const;

 private:  // Private Method(s)
    bool is_equal(value_type const& x, value_type const& y);

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
template <typename InputIt>
static_assoc<T, U, N>::static_assoc(InputIt first, InputIt last, key_compare const& comp)
  : static_assoc(comp) {
    while (first != last) {
        auto pos = std::upper_bound(values_.cbegin(), values_.cend(), *first, value_comp());
        if (!U || pos == begin() || !is_equal(*(pos - 1), *first)) {
            values_.insert(pos, *first);
        }

        ++first;
    }
}

template <typename T, bool U, std::size_t N>
inline static_assoc<T, U, N>::static_assoc(std::initializer_list<value_type> il,
                                           key_compare const& comp)
  : static_assoc(il.begin(), il.end(), comp) {
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
inline typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::begin() noexcept {
    return values_.begin();
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::const_iterator
static_assoc<T, U, N>::begin() const noexcept {
    return cbegin();
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::const_iterator
static_assoc<T, U, N>::cbegin() const noexcept {
    return values_.cbegin();
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::end() noexcept {
    return values_.end();
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::const_iterator
static_assoc<T, U, N>::end() const noexcept {
    return cend();
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::const_iterator
static_assoc<T, U, N>::cend() const noexcept {
    return values_.cend();
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::reverse_iterator
static_assoc<T, U, N>::rbegin() noexcept {
    return values_.rbegin();
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::const_reverse_iterator
static_assoc<T, U, N>::rbegin() const noexcept {
    return crbegin();
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::const_reverse_iterator
static_assoc<T, U, N>::crbegin() const noexcept {
    return values_.crbegin();
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::reverse_iterator
static_assoc<T, U, N>::rend() noexcept {
    return values_.rend();
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::const_reverse_iterator
static_assoc<T, U, N>::rend() const noexcept {
    return crend();
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::const_reverse_iterator
static_assoc<T, U, N>::crend() const noexcept {
    return values_.crend();
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

template <typename T, bool U, std::size_t N>
inline bool static_assoc<T, U, N>::is_equal(value_type const& x, value_type const& y) {
    return !value_compare::operator()(x, y) && !value_compare::operator()(y, x);
}

}  // namespace internal

}  // namespace bptree

#endif  // BPTREE_INTERNAL_STATIC_ASSOC_HPP_
