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

enum class constructed_with {
    default_ctor,
    copy_ctor,
    move_ctor,
    value_ctor
};

class custom_type {
 public:  // Public Method(s)
    custom_type()
      : custom_type(constructed_with::default_ctor, 0)
        { /* do nothing */ }

    custom_type(custom_type const& other)
      : custom_type(constructed_with::copy_ctor, other.val_)
        { /* do nothing */ }

    custom_type(custom_type&& other)
      : custom_type(constructed_with::move_ctor, other.val_)
        { other.val_ = 0; }

    explicit custom_type(int n)
      : custom_type(constructed_with::value_ctor, n)
        { /* do nothing */ }

    ~custom_type()
        { --num_instances_; }

    custom_type& operator=(custom_type const& other)
        { val_ = other.val_; return *this; }

    custom_type& operator=(custom_type&& other)
        { val_ = other.val_; other.val_ = 0; return *this; }

    int get() const
        { return val_; }

    constructed_with ctor() const
        { return ctor_; }

 public:  // Public Static Method(s)
    static std::size_t num_instances()
        { return num_instances_; }

 private:  // Private Method(s)
    custom_type(constructed_with ctor, int n)
      : ctor_(ctor), val_(n)
        { ++num_instances_; }

 private:  // Private Property(ies)
    int val_;
    constructed_with ctor_;

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

TEST(StaticVectorTest, ConstructWithCount) {
    std::size_t const COUNT = 3;
    static_vector<custom_type, N> v(COUNT);

    EXPECT_EQ(COUNT, custom_type::num_instances());

    EXPECT_FALSE(v.empty());
    EXPECT_FALSE(v.full());
    EXPECT_EQ(COUNT, v.size());
    EXPECT_EQ(N, v.max_size());
    EXPECT_EQ(N, v.capacity());
    EXPECT_THROW(v.at(COUNT), std::out_of_range);

    for (std::size_t pos = 0; pos < COUNT; ++pos) {
        custom_type& obj = v.at(pos);
        EXPECT_EQ(0, obj.get());
        EXPECT_EQ(constructed_with::default_ctor, obj.ctor());
    }
}

TEST(StaticVectorTest, DestructValues) {
    {
        static_vector<custom_type, N> v(3);
        EXPECT_EQ(3, custom_type::num_instances());
    }

    EXPECT_EQ(0, custom_type::num_instances());
}

TEST(StaticVectorTest, ConstructWithCountAndValue) {
    std::size_t const COUNT = 3;
    int const VALUE = 10;
    static_vector<custom_type, N> v(COUNT, custom_type(VALUE));

    EXPECT_EQ(COUNT, custom_type::num_instances());

    EXPECT_FALSE(v.empty());
    EXPECT_FALSE(v.full());
    EXPECT_EQ(COUNT, v.size());
    EXPECT_EQ(N, v.max_size());
    EXPECT_EQ(N, v.capacity());
    EXPECT_THROW(v.at(COUNT), std::out_of_range);

    for (std::size_t pos = 0; pos < COUNT; ++pos) {
        custom_type& obj = v.at(pos);
        EXPECT_EQ(VALUE, obj.get());
        EXPECT_EQ(constructed_with::copy_ctor, obj.ctor());
    }
}
