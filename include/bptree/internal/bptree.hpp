/************************************************
 *  bptree.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#ifndef BPTREE_INTERNAL_BPTREE_HPP_
#define BPTREE_INTERNAL_BPTREE_HPP_

#include <cstddef>

#include <array>
#include <functional>
#include <vector>
#include <utility>

namespace bptree {

namespace internal {

/************************************************
 * Declaration: class bptree
 ************************************************/

template <typename Key, typename T, std::size_t Degree, std::size_t NumSlots,
          typename Compare = std::less<Key>>
class bptree {
 public:  // Public Type(s)
    using key_type = Key;
    using key_compare = Compare;
    using mapped_type = T;
    using value_type = std::pair<key_type const, mapped_type>;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    class value_compare;

 public:  // Public Method(s)
    bptree();
    explicit bptree(key_compare const& comp);

    bool empty() const noexcept;
    size_type size() const noexcept;

    key_compare key_comp() const;
    value_compare value_comp() const;

 private:  // Private Property(ies)
    value_compare comp_;
};

/************************************************
 * Declaration: class bptree::value_compare
 ************************************************/

template <typename K, typename T, std::size_t D, std::size_t N, typename C>
class bptree<K, T, D, N, C>::value_compare {
    friend bptree<K, T, D, N, C>;

 public:  // Public Method(s)
    value_compare(key_compare comp)
      : comp_(comp)
        { /* do nothing */ }

    bool operator()(value_type const& lhs, value_type const& rhs) {
        return comp_(lhs.first, rhs.first);
    }

 private:  // Private Property(ies)
    key_compare comp_;
};

/************************************************
 * Implementation: class bptree
 ************************************************/

template <typename K, typename T, std::size_t D, std::size_t N, typename C>
bptree<K, T, D, N, C>::bptree()
  : bptree(key_compare()) {
      // do nothing
}

template <typename K, typename T, std::size_t D, std::size_t N, typename C>
bptree<K, T, D, N, C>::bptree(key_compare const& comp)
  : comp_(comp) {
    // do nothing
}

template <typename K, typename T, std::size_t D, std::size_t N, typename C>
bool bptree<K, T, D, N, C>::empty() const noexcept {
    return true;
}

template <typename K, typename T, std::size_t D, std::size_t N, typename C>
typename bptree<K, T, D, N, C>::size_type
bptree<K, T, D, N, C>::size() const noexcept {
    return 0;
}

template <typename K, typename T, std::size_t D, std::size_t N, typename C>
typename bptree<K, T, D, N, C>::key_compare bptree<K, T, D, N, C>::key_comp() const {
    return comp_.comp_;
}

template <typename K, typename T, std::size_t D, std::size_t N, typename C>
typename bptree<K, T, D, N, C>::value_compare bptree<K, T, D, N, C>::value_comp() const {
    return comp_;
}

}  // namespace internal

}  // namespace bptree

#endif  // BPTREE_INTERNAL_BPTREE_HPP_
