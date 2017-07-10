/************************************************
 *  set_traits.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#ifndef BPTREE_INTERNAL_SET_TRAITS_HPP_
#define BPTREE_INTERNAL_SET_TRAITS_HPP_

#include <functional>

namespace bptree {

namespace internal {

/************************************************
 * Declaration: struct set_traits<T, C>
 ************************************************/

template <typename T, typename Compare = std::less<T>>
struct set_traits {
 public:  // Public Type(s)
    using key_type = T;
    using value_type = key_type;

    using key_compare = Compare;
    using value_compare = key_compare;

 protected:  // Protected Type(s)
    class core_compare {
     public:  // Public Method(s)
        explicit core_compare(key_compare comp)
          : comp_(comp)
            { /* do nothing */ }

        template <typename K1, typename K2>
        bool operator()(K1 const& lhs, K2 const& rhs) const {
            return comp_(lhs, rhs);
        }

     private:  // Private Method(s)
        key_compare const& comp_;
    };
};

}  // namespace internal

}  // namespace bptree

#endif  // BPTREE_INTERNAL_SET_TRAITS_HPP_
