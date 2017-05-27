/************************************************
 *  static_vector_test.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#include <cstddef>

#include <stdexcept>

#include <gtest/gtest.h>

#include <bptree/internal/static_vector.hpp>

using bptree::internal::static_vector;

class custom_type {
 public:  // Public Method(s)
    custom_type()
      : custom_type(0)
        { /* do nothing */ }

    explicit custom_type(int n)
      : val_(n)
        { ++num_instances_; }

    ~custom_type()
        { --num_instances_; }

    int get() const
        { return val_; }

 public:  // Public Static Method(s)
    static std::size_t num_instances()
        { return num_instances_; }

 private:  // Private Property(ies)
    int val_;

 private:  // Private Static Property(ies)
    static std::size_t num_instances_;
};

std::size_t custom_type::num_instances_ = 0;
std::size_t const N = 10;

TEST(StaticVectorTest, EmptyVector) {
    static_vector<custom_type, N> v;

    EXPECT_EQ(0, custom_type::num_instances());

    EXPECT_TRUE(v.empty());
    EXPECT_FALSE(v.full());
    EXPECT_EQ(0, v.size());
    EXPECT_EQ(N, v.max_size());
    EXPECT_EQ(N, v.capacity());
    EXPECT_THROW(v.at(0), std::out_of_range);
}
