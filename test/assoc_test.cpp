/************************************************
 *  assoc_test.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#include <functional>

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

TEST(AssocTest, ConstructWithComp) {
    using key = std::pair<double, double>;
    auto compare = [](key const& lhs, key const& rhs) {
        return (lhs.first - lhs.second) < (rhs.first - rhs.second);
    };

    using map = map<key, int, decltype(compare)>;
    map m(compare);

    map::key_compare key_comp = m.key_comp();
    EXPECT_TRUE(key_comp({0.5, 0.3}, {0.7, 0.4}));
    EXPECT_FALSE(key_comp({0.2, 0.1}, {0.3, 0.3}));

    map::value_compare value_comp = m.value_comp();
    EXPECT_TRUE(value_comp({{0.5, 0.3}, 'a'}, {{0.7, 0.4}, 'b'}));
    EXPECT_FALSE(value_comp({{0.2, 0.1}, 'a'}, {{0.3, 0.3}, 'b'}));
    EXPECT_FALSE(value_comp({{0, 0}, 'a'}, {{0, 0}, 'b'}));
}
