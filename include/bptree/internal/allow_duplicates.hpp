/************************************************
 *  allow_duplicates.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#ifndef BPTREE_INTERNAL_ALLOW_DUPLICATES_HPP_
#define BPTREE_INTERNAL_ALLOW_DUPLICATES_HPP_

#include <utility>

namespace bptree {

namespace internal {

/************************************************
 * Declaration: struct allow_duplicates<Container, Compare>
 ************************************************/

template <typename Container, typename Compare>
struct allow_duplicates {
 public:  // Public Type(s)
    using insert_result_t = typename Container::iterator;

 public:  // Public Method(s)
    template <typename V>
    static insert_result_t try_insert(  // NOLINTNEXTLINE(runtime/references)
            Container& c, Compare compare, typename Container::const_iterator pos, V&& value);

    template <typename V>
    static typename Container::iterator insert(  // NOLINTNEXTLINE(runtime/references)
            Container& c, Compare compare, typename Container::const_iterator pos, V&& value);
};

/************************************************
 * Implementation: struct allow_duplicates<Container, Compare>
 ************************************************/

template <typename Container, typename Compare>
template <typename V>
inline typename allow_duplicates<Container, Compare>::insert_result_t
allow_duplicates<Container, Compare>::try_insert(  // NOLINTNEXTLINE(runtime/references)
        Container& c, Compare compare, typename Container::const_iterator pos, V&& value) {
    return c.insert(pos, std::forward<V>(value));
}

template <typename Container, typename Compare>
template <typename V>
inline typename Container::iterator
allow_duplicates<Container, Compare>::insert(  // NOLINTNEXTLINE(runtime/references)
        Container& c, Compare compare, typename Container::const_iterator pos, V&& value) {
    return try_insert(c, compare, pos, std::forward<V>(value));
}

}  // namespace internal

}  // namespace bptree

#endif  // BPTREE_INTERNAL_ALLOW_DUPLICATES_HPP_
