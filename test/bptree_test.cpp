/************************************************
 *  bptree_test.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

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
