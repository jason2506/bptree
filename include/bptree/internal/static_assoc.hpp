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

#include <algorithm>
#include <type_traits>
#include <utility>

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

    using insert_result_t = typename std::conditional<
            Unique,
            std::pair<typename underlying_type::iterator, bool>,
            typename underlying_type::iterator
        >::type;

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

    static_assoc& operator=(std::initializer_list<value_type> il);
    static_assoc& operator=(static_assoc const&) = default;
    static_assoc& operator=(static_assoc&&) = default;

    template <typename... Args>
    insert_result_t emplace(Args&&... args);
    template <typename... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args);
    void clear() noexcept;

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
    template <typename V>
    std::pair<iterator, bool> insert_uncheck(const_iterator pos, V&& value, std::true_type);
    template <typename V>
    iterator insert_uncheck(const_iterator pos, V&& value, std::false_type);

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
        emplace_hint(cend(), *first);
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
static_assoc<T, U, N>&
static_assoc<T, U, N>::operator=(std::initializer_list<value_type> il) {
    clear();

    for (auto& value : il) {
        emplace_hint(cend(), value);
    }

    return *this;
}

template <typename T, bool U, std::size_t N>
template <typename... Args>
typename static_assoc<T, U, N>::insert_result_t
static_assoc<T, U, N>::emplace(Args&&... args) {
    value_type value(std::forward<Args>(args)...);
    auto it = std::upper_bound(cbegin(), cend(), value, value_comp());
    return insert_uncheck(it, std::move(value), std::integral_constant<bool, U>());
}

template <typename T, bool U, std::size_t N>
template <typename... Args>
typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::emplace_hint(const_iterator hint, Args&&... args) {
    value_type value(std::forward<Args>(args)...);
    auto first = cbegin();
    auto last = cend();
    if (hint != first && value_compare::operator()(value, *(hint - 1))) {
        hint = std::upper_bound(first, hint, value, value_comp());
    } else if (hint != last && !value_compare::operator()(value, *hint)) {
        hint = std::upper_bound(hint + 1, last, value, value_comp());
    }

    if (!U || hint == first || value_comp()(*(hint - 1), value)) {
        return values_.insert(hint, std::move(value));
    } else {
        return begin() + (hint - first - 1);
    }
}

template <typename T, bool U, std::size_t N>
inline void static_assoc<T, U, N>::clear() noexcept {
    values_.clear();
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
template <typename V>
std::pair<typename static_assoc<T, U, N>::iterator, bool>
static_assoc<T, U, N>::insert_uncheck(const_iterator pos, V&& value, std::true_type) {
    if (pos == cbegin() || value_comp()(*(pos - 1), value)) {
        return {values_.insert(pos, value), true};
    } else {
        return {begin() + (pos - cbegin() - 1), false};
    }
}

template <typename T, bool U, std::size_t N>
template <typename V>
inline typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::insert_uncheck(const_iterator pos, V&& value, std::false_type) {
    return values_.insert(pos, value);
}

}  // namespace internal

}  // namespace bptree

#endif  // BPTREE_INTERNAL_STATIC_ASSOC_HPP_
