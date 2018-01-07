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
#include <initializer_list>
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
    using core_compare = typename value_traits::core_compare;

    using insert_result_t = typename std::conditional<
            Unique,
            std::pair<typename underlying_type::iterator, bool>,
            typename underlying_type::iterator
        >::type;

    template <typename V, typename T>
    using enable_if_value_constructible_t = typename std::enable_if<
            std::is_constructible<typename value_traits::value_type, V&&>::value,
            T
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
    void swap(static_assoc& other);

    insert_result_t insert(value_type const& value);
    template <typename V>
    enable_if_value_constructible_t<V, insert_result_t> insert(V&& value);
    iterator insert(const_iterator hint, value_type const& value);
    template <typename V>
    enable_if_value_constructible_t<V, iterator> insert(const_iterator hint, V&& value);
    template <typename InputIt>
    void insert(InputIt first, InputIt last);
    void insert(std::initializer_list<value_type> il);
    template <typename... Args>
    insert_result_t emplace(Args&&... args);
    template <typename... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    size_type erase(key_type const& key);
    void clear() noexcept;

    bool empty() const noexcept;
    bool full() const noexcept;
    size_type size() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr size_type capacity() const noexcept;

    size_type count(key_type const& key) const;
    template <typename K, typename Compare = key_compare,
              typename = typename Compare::is_transparent>
    size_type count(K const& key) const;
    iterator find(key_type const& key);
    template <typename K, typename Compare = key_compare,
              typename = typename Compare::is_transparent>
    iterator find(K const& key);
    const_iterator find(key_type const& key) const;
    template <typename K, typename Compare = key_compare,
              typename = typename Compare::is_transparent>
    const_iterator find(K const& key) const;
    std::pair<iterator, iterator> equal_range(key_type const& key);
    template <typename K, typename Compare = key_compare,
              typename = typename Compare::is_transparent>
    std::pair<iterator, iterator> equal_range(K const& key);
    std::pair<const_iterator, const_iterator> equal_range(key_type const& key) const;
    template <typename K, typename Compare = key_compare,
              typename = typename Compare::is_transparent>
    std::pair<const_iterator, const_iterator> equal_range(K const& key) const;
    iterator lower_bound(key_type const& key);
    template <typename K, typename Compare = key_compare,
              typename = typename Compare::is_transparent>
    iterator lower_bound(K const& key);
    const_iterator lower_bound(key_type const& key) const;
    template <typename K, typename Compare = key_compare,
              typename = typename Compare::is_transparent>
    const_iterator lower_bound(K const& key) const;
    iterator upper_bound(key_type const& key);
    template <typename K, typename Compare = key_compare,
              typename = typename Compare::is_transparent>
    iterator upper_bound(K const& key);
    const_iterator upper_bound(key_type const& key) const;
    template <typename K, typename Compare = key_compare,
              typename = typename Compare::is_transparent>
    const_iterator upper_bound(K const& key) const;

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

    friend bool operator==(static_assoc const& x, static_assoc const& y)
        { return x.values_ == y.values_; }
    friend bool operator!=(static_assoc const& x, static_assoc const& y)
        { return x.values_ != y.values_; }
    friend bool operator> (static_assoc const& x, static_assoc const& y)
        { return x.values_ > y.values_; }
    friend bool operator< (static_assoc const& x, static_assoc const& y)
        { return x.values_ < y.values_; }
    friend bool operator>=(static_assoc const& x, static_assoc const& y)
        { return x.values_ >= y.values_; }
    friend bool operator<=(static_assoc const& x, static_assoc const& y)
        { return x.values_ <= y.values_; }

 private:  // Private Method(s)
    core_compare core_comp() const;
    template <typename V>
    std::pair<iterator, bool> insert_uncheck(const_iterator pos, V&& value, std::true_type);
    template <typename V>
    iterator insert_uncheck(const_iterator pos, V&& value, std::false_type);
    template <typename V>
    iterator insert_hint_uncheck(const_iterator pos, V&& value, std::true_type);
    template <typename V>
    iterator insert_hint_uncheck(const_iterator pos, V&& value, std::false_type);

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
inline void static_assoc<T, U, N>::swap(static_assoc& other) {
    value_traits::swap(other);
    value_compare::swap(other);
    values_.swap(other.values_);
}

template <typename T, bool U, std::size_t N>
typename static_assoc<T, U, N>::insert_result_t
static_assoc<T, U, N>::insert(value_type const& value) {
    auto it = std::upper_bound(cbegin(), cend(), value, value_comp());
    return insert_uncheck(it, value, std::integral_constant<bool, U>());
}

template <typename T, bool U, std::size_t N>
template <typename V>
inline static_assoc<T, U, N>::enable_if_value_constructible_t<
    V, typename static_assoc<T, U, N>::insert_result_t
>
static_assoc<T, U, N>::insert(V&& value) {
    return emplace(std::forward<V>(value));
}

template <typename T, bool U, std::size_t N>
typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::insert(const_iterator hint, value_type const& value) {
    auto first = cbegin();
    auto last = cend();
    if (hint != first && value_compare::operator()(value, *(hint - 1))) {
        hint = std::upper_bound(first, hint, value, value_comp());
    } else if (hint != last && !value_compare::operator()(value, *hint)) {
        hint = std::upper_bound(hint + 1, last, value, value_comp());
    }

    return insert_hint_uncheck(hint, value, std::integral_constant<bool, U>());
}

template <typename T, bool U, std::size_t N>
template <typename V>
inline static_assoc<T, U, N>::enable_if_value_constructible_t<
    V, typename static_assoc<T, U, N>::iterator
>
static_assoc<T, U, N>::insert(const_iterator hint, V&& value) {
    return emplace_hint(hint, std::forward<V>(value));
}

template <typename T, bool U, std::size_t N>
template <typename InputIt>
void static_assoc<T, U, N>::insert(InputIt first, InputIt last) {
    while (first != last) {
        insert(*first);
        ++first;
    }
}

template <typename T, bool U, std::size_t N>
inline void static_assoc<T, U, N>::insert(std::initializer_list<value_type> il) {
    insert(il.begin(), il.end());
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

    return insert_hint_uncheck(hint, std::move(value), std::integral_constant<bool, U>());
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::erase(const_iterator pos) {
    return values_.erase(pos);
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::erase(const_iterator first, const_iterator last) {
    return values_.erase(first, last);
}

template <typename T, bool U, std::size_t N>
typename static_assoc<T, U, N>::size_type
static_assoc<T, U, N>::erase(key_type const& key) {
    auto range = equal_range(key);
    erase(range.first, range.second);
    return range.second - range.first;
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
inline typename static_assoc<T, U, N>::size_type
static_assoc<T, U, N>::count(key_type const& key) const {
    auto range = equal_range(key);
    return range.second - range.first;
}

template <typename T, bool U, std::size_t N>
template <typename K, typename Compare, typename>
inline typename static_assoc<T, U, N>::size_type
static_assoc<T, U, N>::count(K const& key) const {
    auto range = equal_range(key);
    return range.second - range.first;
}

template <typename T, bool U, std::size_t N>
typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::find(key_type const& key) {
    auto it = lower_bound(key);
    if (it != end() && core_comp()(key, *it)) {
        it = end();
    }

    return it;
}

template <typename T, bool U, std::size_t N>
template <typename K, typename Compare, typename>
typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::find(K const& key) {
    auto it = lower_bound(key);
    if (it != end() && core_comp()(key, *it)) {
        it = end();
    }

    return it;
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::const_iterator
static_assoc<T, U, N>::find(key_type const& key) const {
    return const_cast<static_assoc*>(this)->find(key);
}

template <typename T, bool U, std::size_t N>
template <typename K, typename Compare, typename>
inline typename static_assoc<T, U, N>::const_iterator
static_assoc<T, U, N>::find(K const& key) const {
    return const_cast<static_assoc*>(this)->find(key);
}

template <typename T, bool U, std::size_t N>
inline std::pair<
    typename static_assoc<T, U, N>::iterator,
    typename static_assoc<T, U, N>::iterator
>
static_assoc<T, U, N>::equal_range(key_type const& key) {
    return std::equal_range(begin(), end(), key, core_comp());
}

template <typename T, bool U, std::size_t N>
template <typename K, typename Compare, typename>
inline std::pair<
    typename static_assoc<T, U, N>::iterator,
    typename static_assoc<T, U, N>::iterator
>
static_assoc<T, U, N>::equal_range(K const& key) {
    return std::equal_range(begin(), end(), key, core_comp());
}

template <typename T, bool U, std::size_t N>
inline std::pair<
    typename static_assoc<T, U, N>::const_iterator,
    typename static_assoc<T, U, N>::const_iterator
>
static_assoc<T, U, N>::equal_range(key_type const& key) const {
    return std::equal_range(cbegin(), cend(), key, core_comp());
}

template <typename T, bool U, std::size_t N>
template <typename K, typename Compare, typename>
inline std::pair<
    typename static_assoc<T, U, N>::const_iterator,
    typename static_assoc<T, U, N>::const_iterator
>
static_assoc<T, U, N>::equal_range(K const& key) const {
    return std::equal_range(cbegin(), cend(), key, core_comp());
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::lower_bound(key_type const& key) {
    return std::lower_bound(begin(), end(), key, core_comp());
}

template <typename T, bool U, std::size_t N>
template <typename K, typename Compare, typename>
inline typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::lower_bound(K const& key) {
    return std::lower_bound(begin(), end(), key, core_comp());
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::const_iterator
static_assoc<T, U, N>::lower_bound(key_type const& key) const {
    return std::lower_bound(cbegin(), cend(), key, core_comp());
}

template <typename T, bool U, std::size_t N>
template <typename K, typename Compare, typename>
inline typename static_assoc<T, U, N>::const_iterator
static_assoc<T, U, N>::lower_bound(K const& key) const {
    return std::lower_bound(cbegin(), cend(), key, core_comp());
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::upper_bound(key_type const& key) {
    return std::upper_bound(begin(), end(), key, core_comp());
}

template <typename T, bool U, std::size_t N>
template <typename K, typename Compare, typename>
inline typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::upper_bound(K const& key) {
    return std::upper_bound(begin(), end(), key, core_comp());
}

template <typename T, bool U, std::size_t N>
inline typename static_assoc<T, U, N>::const_iterator
static_assoc<T, U, N>::upper_bound(key_type const& key) const {
    return std::upper_bound(cbegin(), cend(), key, core_comp());
}

template <typename T, bool U, std::size_t N>
template <typename K, typename Compare, typename>
inline typename static_assoc<T, U, N>::const_iterator
static_assoc<T, U, N>::upper_bound(K const& key) const {
    return std::upper_bound(cbegin(), cend(), key, core_comp());
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
inline typename static_assoc<T, U, N>::core_compare
static_assoc<T, U, N>::core_comp() const {
    return core_compare(*this);
}

template <typename T, bool U, std::size_t N>
template <typename V>
std::pair<typename static_assoc<T, U, N>::iterator, bool>
static_assoc<T, U, N>::insert_uncheck(const_iterator pos, V&& value, std::true_type) {
    if (pos == cbegin() || value_comp()(*(pos - 1), value)) {
        return {values_.insert(pos, std::forward<V>(value)), true};
    } else {
        return {begin() + (pos - cbegin() - 1), false};
    }
}

template <typename T, bool U, std::size_t N>
template <typename V>
inline typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::insert_uncheck(const_iterator pos, V&& value, std::false_type) {
    return values_.insert(pos, std::forward<V>(value));
}

template <typename T, bool U, std::size_t N>
template <typename V>
inline typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::insert_hint_uncheck(const_iterator pos, V&& value, std::true_type) {
    return insert_uncheck(pos, std::forward<V>(value), std::true_type()).first;
}

template <typename T, bool U, std::size_t N>
template <typename V>
inline typename static_assoc<T, U, N>::iterator
static_assoc<T, U, N>::insert_hint_uncheck(const_iterator pos, V&& value, std::false_type) {
    return insert_uncheck(pos, std::forward<V>(value), std::false_type());
}

}  // namespace internal

}  // namespace bptree

/************************************************
 * Implementation: std::swap(static_assoc<T, U, N>&, static_assoc<T, U, N>&)
 ************************************************/

namespace std {

template <typename T, bool U, std::size_t N>
void swap(bptree::internal::static_assoc<T, U, N>& v1,
          bptree::internal::static_assoc<T, U, N>& v2) {
    v1.swap(v2);
}

}  // namespace std

#endif  // BPTREE_INTERNAL_STATIC_ASSOC_HPP_
