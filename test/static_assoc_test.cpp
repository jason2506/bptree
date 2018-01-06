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

TEST(StaticAssocTest, EmptyAssoc) {
    static_map<int, char, assoc_size> map;

    EXPECT_TRUE(map.empty());
    EXPECT_FALSE(map.full());
    EXPECT_EQ(0, map.size());
    EXPECT_EQ(assoc_size, map.max_size());
    EXPECT_EQ(assoc_size, map.capacity());

    auto key_comp = map.key_comp();
    EXPECT_TRUE(key_comp(1, 3));
    EXPECT_FALSE(key_comp(3, 1));
    EXPECT_FALSE(key_comp(2, 2));

    auto value_comp = map.value_comp();
    EXPECT_TRUE(value_comp({1, 'a'}, {2, 'b'}));
    EXPECT_FALSE(value_comp({2, 'b'}, {1, 'a'}));
    EXPECT_FALSE(value_comp({2, 'a'}, {1, 'b'}));
}

TEST(StaticAssocTest, ConstructWithComp) {
    using key = std::pair<double, double>;
    auto compare = [](key const& lhs, key const& rhs) {
        return (lhs.first - lhs.second) < (rhs.first - rhs.second);
    };

    static_map<key, char, assoc_size, decltype(compare)> map(compare);

    auto key_comp = map.key_comp();
    EXPECT_TRUE(key_comp({0.5, 0.3}, {0.7, 0.4}));
    EXPECT_FALSE(key_comp({0.2, 0.1}, {0.3, 0.3}));

    auto value_comp = map.value_comp();
    EXPECT_TRUE(value_comp({{0.5, 0.3}, 'a'}, {{0.7, 0.4}, 'b'}));
    EXPECT_FALSE(value_comp({{0.2, 0.1}, 'a'}, {{0.3, 0.3}, 'b'}));
    EXPECT_FALSE(value_comp({{0, 0}, 'a'}, {{0, 0}, 'b'}));
}
