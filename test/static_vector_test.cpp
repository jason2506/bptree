/************************************************
 *  static_vector_test.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#include <cassert>
#include <cstddef>

#include <array>
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

    // used only for generating expected results in tests
    custom_type(constructed_with ctor, int n)
      : ctor_(ctor), val_(n)
        { ++num_instances_; }

    ~custom_type()
        { --num_instances_; }

    custom_type& operator=(custom_type const& other)
        { val_ = other.val_; return *this; }

    custom_type& operator=(custom_type&& other)
        { val_ = other.val_; other.val_ = 0; return *this; }

    bool operator==(custom_type const& other) const
        { return val_ == other.val_ && ctor_ == other.ctor_; }

    int get() const
        { return val_; }

    constructed_with ctor() const
        { return ctor_; }

 public:  // Public Static Method(s)
    static std::size_t num_instances()
        { return num_instances_; }

 private:  // Private Property(ies)
    int val_;
    constructed_with ctor_;

 private:  // Private Static Property(ies)
    static std::size_t num_instances_;
};

std::size_t custom_type::num_instances_ = 0;
std::size_t const SIZE_VECTOR = 10;

template <typename T, std::size_t N>
void assert_static_vector_size(static_vector<T, N> const& v, std::size_t size) {
    assert(size <= N);

    EXPECT_EQ(size == 0, v.empty());
    EXPECT_EQ(size == N, v.full());
    EXPECT_EQ(size, v.size());
    EXPECT_EQ(N, v.max_size());
    EXPECT_EQ(N, v.capacity());

    EXPECT_THROW(v.at(size), std::out_of_range);
}

template <typename T, std::size_t N>
void assert_static_vector_values(static_vector<T, N> const& v, std::size_t count, T const& value) {
    assert_static_vector_size(v, count);

    for (std::size_t pos = 0; pos < count; ++pos) {
        EXPECT_EQ(value, v.at(pos));
    }
}

TEST(StaticVectorTest, EmptyVector) {
    static_vector<custom_type, SIZE_VECTOR> v;

    EXPECT_EQ(0, custom_type::num_instances());

    assert_static_vector_size(v, 0);
}

TEST(StaticVectorTest, ConstructWithCount) {
    std::size_t const COUNT = 3;
    static_vector<custom_type, SIZE_VECTOR> v(COUNT);

    EXPECT_EQ(COUNT, custom_type::num_instances());

    auto expected = custom_type(constructed_with::default_ctor, 0);
    assert_static_vector_values(v, COUNT, expected);
}

TEST(StaticVectorTest, DestructValues) {
    {
        static_vector<custom_type, SIZE_VECTOR> v(3);
        EXPECT_EQ(3, custom_type::num_instances());
    }

    EXPECT_EQ(0, custom_type::num_instances());
}

TEST(StaticVectorTest, ConstructFullVector) {
    static_vector<custom_type, SIZE_VECTOR> v(SIZE_VECTOR);

    EXPECT_EQ(SIZE_VECTOR, custom_type::num_instances());

    auto expected = custom_type(constructed_with::default_ctor, 0);
    assert_static_vector_values(v, SIZE_VECTOR, expected);
}

TEST(StaticVectorTest, ConstructWithCountAndValue) {
    std::size_t const COUNT = 3;
    int const VALUE = 10;
    static_vector<custom_type, SIZE_VECTOR> v(COUNT, custom_type(VALUE));

    EXPECT_EQ(COUNT, custom_type::num_instances());

    auto expected = custom_type(constructed_with::copy_ctor, VALUE);
    assert_static_vector_values(v, COUNT, expected);
}

TEST(StaticVectorTest, ConstructWithIteratorPair) {
    std::array<custom_type, 5> arr = {
        custom_type(1),
        custom_type(2),
        custom_type(3),
        custom_type(5),
        custom_type(8)
    };

    static_vector<custom_type, SIZE_VECTOR> v(arr.begin(), arr.end());

    EXPECT_EQ(arr.size() * 2, custom_type::num_instances());
    assert_static_vector_size(v, arr.size());

    for (std::size_t pos = 0; pos < arr.size(); ++pos) {
        auto expected = custom_type(constructed_with::copy_ctor, arr[pos].get());
        EXPECT_EQ(expected, v.at(pos));
    }
}
