/************************************************
 *  bptree_test.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#include <utility>

#include <gtest/gtest.h>

#include <bptree/internal/bptree.hpp>

using bptree::internal::bptree;

std::size_t constexpr degree = 4;
std::size_t constexpr num_slots = 8;

TEST(BPTreeTest, EmptyTree) {
    bptree<char, int, degree, num_slots> tree;

    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(0, tree.size());
}

TEST(BPTreeTest, ConstructTreeWithComp) {
    using key = std::pair<double, char>;
    auto compare = [](key const& lhs, key const& rhs) {
        return lhs.first < rhs.first;
    };

    using bptree = bptree<key, int, degree, num_slots, decltype(compare)>;
    bptree tree(compare);

    bptree::key_compare key_comp = tree.key_comp();
    bptree::value_compare value_comp = tree.value_comp();

    bptree::value_type v1({{1.0, 'c'}, 3});
    bptree::value_type v2({{-3.5, 'a'}, 8});

    EXPECT_FALSE(key_comp(v1.first, v2.first));
    EXPECT_TRUE(key_comp(v2.first, v1.first));
    EXPECT_FALSE(value_comp(v1, v2));
    EXPECT_TRUE(value_comp(v2, v1));
}
