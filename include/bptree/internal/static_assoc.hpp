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

 private:  // Private Property(ies)
    underlying_type values_;
};

/************************************************
 * Implementation: class static_assoc<T, U, N>
 ************************************************/

template <typename T, bool U, std::size_t N>
inline static_assoc<T, U, N>::static_assoc()
  : value_compare(key_compare()), values_() {
    // do nothing
}

}  // namespace internal

}  // namespace bptree

#endif  // BPTREE_INTERNAL_STATIC_ASSOC_HPP_
