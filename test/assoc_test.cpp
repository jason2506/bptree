/************************************************
 *  assoc_test.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#include <gtest/gtest.h>


#include <bptree/internal/assoc.hpp>
#include <bptree/internal/map_traits.hpp>
#include <bptree/internal/set_traits.hpp>

using bptree::internal::assoc;
using bptree::internal::map_traits;
using bptree::internal::set_traits;

std::size_t constexpr degree = 4;
std::size_t constexpr num_slots = 8;

template <typename Key, typename T, typename Compare = std::less<Key>>
using map = assoc<map_traits<Key, T, Compare>, true, degree, num_slots>;

template <typename Key, typename T, typename Compare = std::less<Key>>
using multimap = assoc<map_traits<Key, T, Compare>, false, degree, num_slots>;

template <typename T, typename Compare = std::less<T>>
using set = assoc<set_traits<T, Compare>, true, degree, num_slots>;

template <typename T, typename Compare = std::less<T>>
using multiset = assoc<set_traits<T, Compare>, false, degree, num_slots>;

TEST(AssocTest, EmptyAssoc) {
    map<char, int> m;

    EXPECT_TRUE(m.empty());
    EXPECT_EQ(0, m.size());
}
