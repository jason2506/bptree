/************************************************
 *  assoc.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#ifndef BPTREE_INTERNAL_ASSOC_HPP_
#define BPTREE_INTERNAL_ASSOC_HPP_

namespace bptree {

namespace internal {

/************************************************
 * Declaration: class assoc<T, U, D, N, A>
 ************************************************/

template <typename ValueTraits, bool Unique, std::size_t Degree, std::size_t NumSlots,
          typename Allocator = std::allocator<typename ValueTraits::value_type>>
class assoc
  : public ValueTraits,
    private ValueTraits::value_compare,
    private Allocator {
 private:  // Private Type(s)
    using value_traits = ValueTraits;

 public:  // Public Type(s)
    using key_type = typename value_traits::key_type;
    using value_type = typename value_traits::value_type;
    using key_compare = typename value_traits::key_compare;
    using value_compare = typename value_traits::value_compare;

    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using allocator_type = Allocator;

 public:  // Public Method(s)
    assoc();

    bool empty() const noexcept;
    size_type size() const noexcept;
};

/************************************************
 * Implementation: class assoc<T, U, D, N, A>
 ************************************************/

template <typename T, bool U, std::size_t D, std::size_t N, typename A>
inline assoc<T, U, D, N, A>::assoc()
  : value_compare(key_compare()) {
    // do nothing
}

template <typename T, bool U, std::size_t D, std::size_t N, typename A>
inline bool assoc<T, U, D, N, A>::empty() const noexcept {
    return size() == 0;
}

template <typename T, bool U, std::size_t D, std::size_t N, typename A>
inline typename assoc<T, U, D, N, A>::size_type assoc<T, U, D, N, A>::size() const noexcept {
    return 0;
}

}  // namespace internal

}  // namespace bptree

#endif  // BPTREE_INTERNAL_ASSOC_HPP_