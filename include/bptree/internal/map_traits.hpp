/************************************************
 *  map_traits.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#ifndef BPTREE_INTERNAL_MAP_TRAITS_HPP_
#define BPTREE_INTERNAL_MAP_TRAITS_HPP_

#include <functional>
#include <utility>

namespace bptree {

namespace internal {

/************************************************
 * Declaration: struct map_traits<K, T, C>
 ************************************************/

template <typename Key, typename T, typename Compare = std::less<Key>>
struct map_traits {
 public:  // Public Type(s)
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<key_type const, mapped_type>;

    using key_compare = Compare;

    class value_compare : protected key_compare {
     protected:  // Protected Method(s)
        explicit value_compare(key_compare comp)
          : key_compare(comp)
            { /* do nothing */ }

     public:  // Public Method(s)
        bool operator()(value_type const& lhs, value_type const& rhs) const {
            return key_compare::operator()(lhs.first, rhs.first);
        }
    };

 protected:  // Protected Type(s)
    class core_compare {
     public:  // Public Method(s)
        explicit core_compare(key_compare comp)
          : comp_(comp)
            { /* do nothing */ }

        template <typename K>
        bool operator()(K const& lhs, value_type const& rhs) const {
            return comp_(lhs, rhs.first);
        }

        template <typename K>
        bool operator()(value_type const& lhs, K const& rhs) const {
            return comp_(lhs.first, rhs);
        }

     private:  // Private Method(s)
        key_compare const& comp_;
    };
};

}  // namespace internal

}  // namespace bptree

#endif  // BPTREE_INTERNAL_MAP_TRAITS_HPP_
