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
    static custom_type construct_with_default_ctor(int val)
        { return custom_type(constructed_with::default_ctor, val); }

    static custom_type construct_with_copy_ctor(int val)
        { return custom_type(constructed_with::copy_ctor, val); }

    static custom_type construct_with_move_ctor(int val)
        { return custom_type(constructed_with::move_ctor, val); }

    static custom_type construct_with_value_ctor(int val)
        { return custom_type(constructed_with::value_ctor, val); }

    static void reset_num_instances()
        { num_instances_ = 0; }

    static std::size_t num_instances()
        { return num_instances_; }

 private:  // Private Property(ies)
    int val_;
    constructed_with ctor_;

 private:  // Private Static Property(ies)
    static std::size_t num_instances_;
};

class StaticVectorTest : public ::testing::Test {
 protected:
     virtual void SetUp() {
         custom_type::reset_num_instances();
     }
};

std::size_t custom_type::num_instances_ = 0;

#define VA_NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, n, ...) n
#define VA_NARGS(...) VA_NARGS_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define WRAP_VALUES_IMPL_1(wrap, value) wrap(value)
#define WRAP_VALUES_IMPL_2(wrap, _1, value) \
        WRAP_VALUES_IMPL_1(wrap, _1), wrap(value)
#define WRAP_VALUES_IMPL_3(wrap, _1, _2, value) \
        WRAP_VALUES_IMPL_2(wrap, _1, _2), wrap(value)
#define WRAP_VALUES_IMPL_4(wrap, _1, _2, _3, value) \
        WRAP_VALUES_IMPL_3(wrap, _1, _2, _3), wrap(value)
#define WRAP_VALUES_IMPL_5(wrap, _1, _2, _3, _4, value) \
        WRAP_VALUES_IMPL_4(wrap, _1, _2, _3, _4), wrap(value)
#define WRAP_VALUES_IMPL_6(wrap, _1, _2, _3, _4, _5, value) \
        WRAP_VALUES_IMPL_5(wrap, _1, _2, _3, _4, _5), wrap(value)
#define WRAP_VALUES_IMPL_7(wrap, _1, _2, _3, _4, _5, _6, value) \
        WRAP_VALUES_IMPL_6(wrap, _1, _2, _3, _4, _5, _6), wrap(value)
#define WRAP_VALUES_IMPL_8(wrap, _1, _2, _3, _4, _5, _6, _7, value) \
        WRAP_VALUES_IMPL_7(wrap, _1, _2, _3, _4, _5, _6, _7), wrap(value)
#define WRAP_VALUES_IMPL_9(wrap, _1, _2, _3, _4, _5, _6, _7, _8, value) \
        WRAP_VALUES_IMPL_8(wrap, _1, _2, _3, _4, _5, _6, _7, _8), wrap(value)
#define WRAP_VALUES_IMPL_10(wrap, _1, _2, _3, _4, _5, _6, _7, _8, _9, value) \
        WRAP_VALUES_IMPL_9(wrap, _1, _2, _3, _4, _5, _6, _7, _8, _9), wrap(value)
#define WRAP_VALUES_IMPL_(n, wrap, ...) WRAP_VALUES_IMPL_##n(wrap, __VA_ARGS__)
#define WRAP_VALUES_IMPL(n, wrap, ...)  WRAP_VALUES_IMPL_(n, wrap, __VA_ARGS__)
#define WRAP_VALUES(wrap, ...)          WRAP_VALUES_IMPL(VA_NARGS(__VA_ARGS__), wrap, __VA_ARGS__)

#define REPEAT_VALUE_IMPL_1(value)      value
#define REPEAT_VALUE_IMPL_2(value)      REPEAT_VALUE_IMPL_1(value), value
#define REPEAT_VALUE_IMPL_3(value)      REPEAT_VALUE_IMPL_2(value), value
#define REPEAT_VALUE_IMPL_4(value)      REPEAT_VALUE_IMPL_3(value), value
#define REPEAT_VALUE_IMPL_5(value)      REPEAT_VALUE_IMPL_4(value), value
#define REPEAT_VALUE_IMPL_6(value)      REPEAT_VALUE_IMPL_5(value), value
#define REPEAT_VALUE_IMPL_7(value)      REPEAT_VALUE_IMPL_6(value), value
#define REPEAT_VALUE_IMPL_8(value)      REPEAT_VALUE_IMPL_7(value), value
#define REPEAT_VALUE_IMPL_9(value)      REPEAT_VALUE_IMPL_8(value), value
#define REPEAT_VALUE_IMPL_10(value)     REPEAT_VALUE_IMPL_9(value), value
#define REPEAT_VALUE_IMPL(n, value)     REPEAT_VALUE_IMPL_##n(value)
#define REPEAT_VALUE(n, value)          REPEAT_VALUE_IMPL(n, value)

#define SIZE_VECTOR                     10
#define TEST_VALUES                     1, 2, 3, 5, 8
#define EXTRA_TEST_VALUES               13, 21, 34

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

template <typename T, std::size_t N, std::size_t M>
void assert_static_vector_values(static_vector<T, N> const& v, T const (&expected)[M]) {
    assert_static_vector_size(v, M);

    typename static_vector<T, N>::const_pointer data_ptr = v.data();
    for (std::size_t pos = 0; pos < M; ++pos, ++data_ptr) {
        EXPECT_EQ(expected[pos], v[pos]);
        EXPECT_EQ(expected[pos], v.at(pos));
        EXPECT_EQ(expected[pos], *data_ptr);
    }
}

TEST_F(StaticVectorTest, EmptyVector) {
    static_vector<custom_type, SIZE_VECTOR> v;

    EXPECT_EQ(0, custom_type::num_instances());
    assert_static_vector_size(v, 0);
}

TEST_F(StaticVectorTest, ConstructWithCount) {
#define COUNT 3
    static_vector<custom_type, SIZE_VECTOR> v(COUNT);

    EXPECT_EQ(COUNT, custom_type::num_instances());
    assert_static_vector_values(v, {
        REPEAT_VALUE(COUNT, custom_type(constructed_with::default_ctor, 0))
    });
#undef COUNT
}

TEST_F(StaticVectorTest, ConstructEmptyVector) {
    static_vector<custom_type, SIZE_VECTOR> v(0);

    EXPECT_EQ(0, custom_type::num_instances());
    assert_static_vector_size(v, 0);
}

TEST_F(StaticVectorTest, ConstructFullVector) {
    static_vector<custom_type, SIZE_VECTOR> v(SIZE_VECTOR);

    EXPECT_EQ(SIZE_VECTOR, custom_type::num_instances());
    assert_static_vector_values(v, {
        REPEAT_VALUE(SIZE_VECTOR, custom_type(constructed_with::default_ctor, 0))
    });
}

TEST_F(StaticVectorTest, ConstructWithCountAndValue) {
#define COUNT 3
    int const value = 10;
    static_vector<custom_type, SIZE_VECTOR> v(COUNT, custom_type(value));

    EXPECT_EQ(COUNT, custom_type::num_instances());
    assert_static_vector_values(v, {
        REPEAT_VALUE(COUNT, custom_type(constructed_with::copy_ctor, value))
    });
#undef COUNT
}

TEST_F(StaticVectorTest, ConstructWithIteratorPair) {
    std::size_t constexpr size = VA_NARGS(TEST_VALUES);
    std::array<custom_type, size> arr = { WRAP_VALUES(custom_type, TEST_VALUES) };
    static_vector<custom_type, SIZE_VECTOR> v(arr.begin(), arr.end());

    EXPECT_EQ(size * 2, custom_type::num_instances());
    assert_static_vector_values(v, {
        WRAP_VALUES(custom_type::construct_with_copy_ctor, TEST_VALUES)
    });
}

TEST_F(StaticVectorTest, ConstructWithInitializerList) {
    std::size_t const size = VA_NARGS(TEST_VALUES);
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };

    EXPECT_EQ(size, custom_type::num_instances());
    assert_static_vector_values(v, {
        WRAP_VALUES(custom_type::construct_with_copy_ctor, TEST_VALUES)
    });
}

TEST_F(StaticVectorTest, ConstructWithAnotherVector) {
    std::size_t const size = VA_NARGS(TEST_VALUES);
    static_vector<custom_type, SIZE_VECTOR> v1 = { WRAP_VALUES(custom_type, TEST_VALUES) };
    decltype(v1) v2 = v1;

    EXPECT_EQ(size * 2, custom_type::num_instances());
    assert_static_vector_values(v2, {
        WRAP_VALUES(custom_type::construct_with_copy_ctor, TEST_VALUES)
    });
}

TEST_F(StaticVectorTest, DestructValues) {
    {
        static_vector<custom_type, SIZE_VECTOR> v(3);
        EXPECT_EQ(3, custom_type::num_instances());
    }

    EXPECT_EQ(0, custom_type::num_instances());
}

TEST_F(StaticVectorTest, ClearValues) {
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };

    v.clear();

    EXPECT_EQ(0, custom_type::num_instances());
    assert_static_vector_size(v, 0);
}

TEST_F(StaticVectorTest, AssignWithOperatorAndInitializerList) {
    std::size_t const size = VA_NARGS(EXTRA_TEST_VALUES);
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };
    v = { WRAP_VALUES(custom_type, EXTRA_TEST_VALUES) };

    EXPECT_EQ(size, custom_type::num_instances());
    assert_static_vector_values(v, {
        WRAP_VALUES(custom_type::construct_with_copy_ctor, EXTRA_TEST_VALUES)
    });
}

TEST_F(StaticVectorTest, AssignWithOperatorAndAnotherVector) {
    std::size_t const size = VA_NARGS(EXTRA_TEST_VALUES);
    static_vector<custom_type, SIZE_VECTOR> v1 = { WRAP_VALUES(custom_type, TEST_VALUES) };
    decltype(v1) v2 = { WRAP_VALUES(custom_type, EXTRA_TEST_VALUES) };
    v1 = v2;

    EXPECT_EQ(size * 2, custom_type::num_instances());
    assert_static_vector_values(v2, {
        WRAP_VALUES(custom_type::construct_with_copy_ctor, EXTRA_TEST_VALUES)
    });
}

TEST_F(StaticVectorTest, AssignWithMethodAndCountAndValues) {
#define COUNT 3
    int const value = 10;
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };
    v.assign(COUNT, custom_type(value));

    EXPECT_EQ(COUNT, custom_type::num_instances());
    assert_static_vector_values(v, {
        REPEAT_VALUE(COUNT, custom_type(constructed_with::copy_ctor, value))
    });
#undef COUNT
}

TEST_F(StaticVectorTest, AssignWithMethodAndInitializerList) {
    std::size_t const size = VA_NARGS(EXTRA_TEST_VALUES);
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };
    v.assign({ WRAP_VALUES(custom_type, EXTRA_TEST_VALUES) });

    EXPECT_EQ(size, custom_type::num_instances());
    assert_static_vector_values(v, {
        WRAP_VALUES(custom_type::construct_with_copy_ctor, EXTRA_TEST_VALUES)
    });
}

TEST_F(StaticVectorTest, AssignWithMethodAndIteratprPair) {
    std::size_t const size = VA_NARGS(EXTRA_TEST_VALUES);
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };
    std::array<custom_type, size> arr = { WRAP_VALUES(custom_type, EXTRA_TEST_VALUES) };
    v.assign(arr.begin(), arr.end());

    EXPECT_EQ(size * 2, custom_type::num_instances());
    assert_static_vector_values(v, {
        WRAP_VALUES(custom_type::construct_with_copy_ctor, EXTRA_TEST_VALUES)
    });
}

TEST_F(StaticVectorTest, PushBackCopiedValue) {
    std::size_t const size = VA_NARGS(TEST_VALUES) + 1;
    int const pushed_value = 99;
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };

    custom_type item(pushed_value);
    v.push_back(item);

    EXPECT_EQ(size + 1, custom_type::num_instances());
    assert_static_vector_values(v, {
        WRAP_VALUES(custom_type::construct_with_copy_ctor, TEST_VALUES),
        custom_type(constructed_with::copy_ctor, pushed_value)
    });
}

TEST_F(StaticVectorTest, PushBackMovedValue) {
    std::size_t const size = VA_NARGS(TEST_VALUES) + 1;
    int const pushed_value = 99;
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };
    v.push_back(custom_type(pushed_value));

    EXPECT_EQ(size, custom_type::num_instances());
    assert_static_vector_values(v, {
        WRAP_VALUES(custom_type::construct_with_copy_ctor, TEST_VALUES),
        custom_type(constructed_with::move_ctor, pushed_value)
    });
}
