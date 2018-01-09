/************************************************
 *  deny_duplicates.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#ifndef BPTREE_INTERNAL_DENY_DUPLICATES_HPP_
#define BPTREE_INTERNAL_DENY_DUPLICATES_HPP_

#include <utility>

namespace bptree {

namespace internal {

/************************************************
 * Declaration: struct deny_duplicates<Container, Compare>
 ************************************************/

template <typename Container, typename Compare>
struct deny_duplicates {
 public:  // Public Type(s)
    using insert_result_t = std::pair<typename Container::iterator, bool>;

 public:  // Public Method(s)
    template <typename V>
    static insert_result_t try_insert(  // NOLINTNEXTLINE(runtime/references)
            Container& c, Compare comp, typename Container::const_iterator pos, V&& value);

    template <typename V>
    static typename Container::iterator insert(  // NOLINTNEXTLINE(runtime/references)
            Container& c, Compare comp, typename Container::const_iterator pos, V&& value);
};

/************************************************
 * Implementation: struct deny_duplicates<Container, Compare>
 ************************************************/

template <typename Container, typename Compare>
template <typename V>
inline typename deny_duplicates<Container, Compare>::insert_result_t
deny_duplicates<Container, Compare>::try_insert(  // NOLINTNEXTLINE(runtime/references)
        Container& c, Compare comp, typename Container::const_iterator pos, V&& value) {
    if (pos == c.cbegin() || comp(*(pos - 1), value)) {
        return {c.insert(pos, std::forward<V>(value)), true};
    } else {
        return {c.begin() + (pos - c.cbegin() - 1), false};
    }
}

template <typename Container, typename Compare>
template <typename V>
inline typename Container::iterator
deny_duplicates<Container, Compare>::insert(  // NOLINTNEXTLINE(runtime/references)
        Container& c, Compare comp, typename Container::const_iterator pos, V&& value) {
    return try_insert(c, comp, pos, std::forward<V>(value)).first;
}

}  // namespace internal

}  // namespace bptree

#endif  // BPTREE_INTERNAL_DENY_DUPLICATES_HPP_
