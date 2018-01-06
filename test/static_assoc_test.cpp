/************************************************
 *  static_assoc_test.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#include <cstddef>

#include <algorithm>

#include <gtest/gtest.h>

#include <bptree/internal/map_traits.hpp>
#include <bptree/internal/set_traits.hpp>
#include <bptree/internal/static_assoc.hpp>

using bptree::internal::map_traits;
using bptree::internal::set_traits;
using bptree::internal::static_assoc;

template <typename Key, typename T, std::size_t N, typename Compare = std::less<Key>>
using static_map = static_assoc<map_traits<Key, T, Compare>, true, N>;

template <typename Key, typename T, std::size_t N, typename Compare = std::less<Key>>
using static_multimap = static_assoc<map_traits<Key, T, Compare>, false, N>;

template <typename T, std::size_t N, typename Compare = std::less<T>>
using static_set = static_assoc<set_traits<T, Compare>, true, N>;

template <typename T, std::size_t N, typename Compare = std::less<T>>
using static_multiset = static_assoc<set_traits<T, Compare>, false, N>;

std::size_t constexpr assoc_size = 10;

TEST(StaticAssocBaseTest, EmptyAssoc) {
    static_map<int, char, assoc_size> map;

    EXPECT_TRUE(map.empty());
    EXPECT_FALSE(map.full());
    EXPECT_EQ(0, map.size());
    EXPECT_EQ(assoc_size, map.max_size());
    EXPECT_EQ(assoc_size, map.capacity());
}
