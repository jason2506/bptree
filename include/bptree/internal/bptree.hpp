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
};

}  // namespace internal

}  // namespace bptree

#endif  // BPTREE_INTERNAL_BPTREE_HPP_
