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
#include <algorithm>
#include <sstream>
#include <stdexcept>

#include <gtest/gtest.h>

#include <bptree/internal/static_vector.hpp>

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

#define SIZE_VECTOR                     10
#define REPEAT_COUNT                    3
#define TEST_VALUES_BEFORE_INSERTED_POS 1, 2, 3
#define TEST_VALUES_AFTER_INSERTED_POS  5, 8
#define TEST_VALUES_INSERTED_POS        VA_NARGS(TEST_VALUES_BEFORE_INSERTED_POS)
#define TEST_VALUES                     TEST_VALUES_BEFORE_INSERTED_POS, \
                                        TEST_VALUES_AFTER_INSERTED_POS
#define EXTRA_TEST_VALUES               13, 21, 34

using bptree::internal::static_vector;

enum class constructed_with {
    skipped,
    default_ctor,
    copy_ctor,
    move_ctor,
    value_ctor
};

class custom_type {
    template <std::size_t N>
    friend class expected_result;

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

    custom_type(constructed_with ctor, int n)
      : ctor_(ctor), val_(n)
        { ++num_instances_; }

    ~custom_type()
        { --num_instances_; }

    custom_type& operator=(custom_type const& other) {
        val_ = other.val_;
        ctor_ = other.ctor_ == constructed_with::skipped
            ? constructed_with::copy_ctor
            : other.ctor_;

        assert(ctor_ != constructed_with::skipped);
        return *this;
    }

    custom_type& operator=(custom_type&& other) {
        val_ = other.val_;
        ctor_ = other.ctor_ == constructed_with::skipped
            ? constructed_with::move_ctor
            : other.ctor_;

        other.val_ = 0;
        other.ctor_ = constructed_with::skipped;

        assert(ctor_ != constructed_with::skipped);
        return *this;
    }

    bool operator==(custom_type const& other) const
        { return val_ == other.val_ && ctor_ == other.ctor_; }

    void skip_ctor()
        { ctor_ = constructed_with::skipped; }

    constructed_with ctor() const
        { return ctor_; }

 public:  // Public Static Method(s)
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

template <std::size_t N>
class expected_result {
 public:  // Public Method(s)
    expected_result() = default;

    expected_result(int value, constructed_with ctor) {
        assign(0, N, value, ctor);
    }

    expected_result(int const values[], constructed_with ctor) {
        assign(0, N, values, ctor);
    }

    void assign(std::size_t pos, std::size_t count, int value, constructed_with ctor) {
        for (decltype(count) offset = 0; offset < count; ++offset) {
            values_[pos + offset].val_ = value;
            values_[pos + offset].ctor_ = ctor;
        }
    }

    void assign(std::size_t pos, std::size_t count, int const values[], constructed_with ctor) {
        for (decltype(count) offset = 0; offset < count; ++offset) {
            values_[pos + offset].val_ = values[offset];
            values_[pos + offset].ctor_ = ctor;
        }
    }

    custom_type const& operator[](std::size_t pos) const {
        return values_[pos];
    }

 private:  // Private Property(ies)
    std::array<custom_type, N> values_;
};

class StaticVectorTest : public ::testing::Test {
 protected:
     virtual void SetUp() {
         custom_type::reset_num_instances();
     }
};

std::size_t custom_type::num_instances_ = 0;
int const inserted_value = 99;

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
void assert_static_vector_values(static_vector<T, N> const& v, expected_result<M> const& expected) {
    assert_static_vector_size(v, M);

    typename static_vector<T, N>::const_pointer data_ptr = v.data();
    for (std::size_t pos = 0; pos < M; ++pos, ++data_ptr) {
        std::ostringstream ss;
        ss << "pos = " << pos;
        SCOPED_TRACE(ss.str());

        EXPECT_EQ(expected[pos], v[pos]);
        EXPECT_EQ(expected[pos], v.at(pos));
        EXPECT_EQ(expected[pos], *data_ptr);
    }
}

template <typename Insert, typename GetInsertPos, std::size_t N>
void test_insert_at(Insert insert, GetInsertPos get_insert_pos,
                    std::size_t num_extra_instances, expected_result<N> const& expected) {
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };
    std::for_each(v.begin(), v.end(), [](custom_type& item) { item.skip_ctor(); });

    insert(v, get_insert_pos(v));

    EXPECT_EQ(N * 2 + num_extra_instances, custom_type::num_instances());
    assert_static_vector_values(v, expected);
}

template <std::size_t NumInserted, typename Insert>
void test_insert_at_begin(Insert insert, std::size_t num_extra_instances, constructed_with ctor) {
    int const values[] = { TEST_VALUES };

    constexpr std::size_t size = VA_NARGS(TEST_VALUES) + NumInserted;
    expected_result<size> expected;
    expected.assign(0, NumInserted, inserted_value, ctor);
    expected.assign(NumInserted, size - NumInserted, values, constructed_with::move_ctor);

    using vector = static_vector<custom_type, SIZE_VECTOR>;
    auto get_insert_pos = [](vector& v) { return v.begin(); };
    test_insert_at(insert, get_insert_pos, num_extra_instances, expected);
}

template <std::size_t NumInserted, typename Insert>
void test_insert_at_end(Insert insert, std::size_t num_extra_instances, constructed_with ctor) {
    int const values[] = { TEST_VALUES };

    constexpr std::size_t size = VA_NARGS(TEST_VALUES) + NumInserted;
    expected_result<size> expected;
    expected.assign(0, size - NumInserted, values, constructed_with::skipped);
    expected.assign(size - NumInserted, NumInserted, inserted_value, ctor);

    using vector = static_vector<custom_type, SIZE_VECTOR>;
    auto get_insert_pos = [](vector& v) { return v.end(); };
    test_insert_at(insert, get_insert_pos, num_extra_instances, expected);
}

template <std::size_t NumInserted, typename Insert>
void test_insert_at_middle(Insert insert, std::size_t num_extra_instances, constructed_with ctor) {
    int const values_before[] = { TEST_VALUES_BEFORE_INSERTED_POS };
    int const values_after[] = { TEST_VALUES_AFTER_INSERTED_POS };

    constexpr std::size_t size = VA_NARGS(TEST_VALUES) + NumInserted;
    expected_result<size> expected;
    expected.assign(0, TEST_VALUES_INSERTED_POS,
                    values_before, constructed_with::skipped);
    expected.assign(TEST_VALUES_INSERTED_POS, NumInserted,
                    inserted_value, ctor);
    expected.assign(TEST_VALUES_INSERTED_POS + NumInserted,
                    size - TEST_VALUES_INSERTED_POS - NumInserted,
                    values_after, constructed_with::move_ctor);

    using vector = static_vector<custom_type, SIZE_VECTOR>;
    auto get_insert_pos = [](vector& v) { return v.begin() + TEST_VALUES_INSERTED_POS; };
    test_insert_at(insert, get_insert_pos, num_extra_instances, expected);
}

template <std::size_t NumInserted, typename Insert>
void test_insert(Insert insert, std::size_t num_extra_instances, constructed_with ctor) {
    test_insert_at_begin<NumInserted>(insert, num_extra_instances, ctor);
    test_insert_at_end<NumInserted>(insert, num_extra_instances, ctor);
    test_insert_at_middle<NumInserted>(insert, num_extra_instances, ctor);
}

TEST_F(StaticVectorTest, EmptyVector) {
    static_vector<custom_type, SIZE_VECTOR> v;

    EXPECT_EQ(0, custom_type::num_instances());
    assert_static_vector_size(v, 0);
}

TEST_F(StaticVectorTest, ConstructWithCount) {
    static_vector<custom_type, SIZE_VECTOR> v(REPEAT_COUNT);

    EXPECT_EQ(REPEAT_COUNT, custom_type::num_instances());
    expected_result<REPEAT_COUNT> expected(0, constructed_with::default_ctor);
    assert_static_vector_values(v, expected);
}

TEST_F(StaticVectorTest, ConstructEmptyVector) {
    static_vector<custom_type, SIZE_VECTOR> v(0);

    EXPECT_EQ(0, custom_type::num_instances());
    assert_static_vector_size(v, 0);
}

TEST_F(StaticVectorTest, ConstructFullVector) {
    static_vector<custom_type, SIZE_VECTOR> v(SIZE_VECTOR);

    EXPECT_EQ(SIZE_VECTOR, custom_type::num_instances());
    expected_result<SIZE_VECTOR> expected(0, constructed_with::default_ctor);
    assert_static_vector_values(v, expected);
}

TEST_F(StaticVectorTest, ConstructWithCountAndValue) {
    static_vector<custom_type, SIZE_VECTOR> v(REPEAT_COUNT,
        custom_type(constructed_with::skipped, inserted_value));

    EXPECT_EQ(REPEAT_COUNT, custom_type::num_instances());
    expected_result<REPEAT_COUNT> expected(inserted_value, constructed_with::copy_ctor);
    assert_static_vector_values(v, expected);
}

TEST_F(StaticVectorTest, ConstructWithIteratorPair) {
    std::size_t constexpr size = VA_NARGS(TEST_VALUES);
    std::array<custom_type, size> arr = { WRAP_VALUES(custom_type, TEST_VALUES) };
    static_vector<custom_type, SIZE_VECTOR> v(arr.begin(), arr.end());

    EXPECT_EQ(size * 2, custom_type::num_instances());
    int values[] = { TEST_VALUES };
    expected_result<size> expected(values, constructed_with::copy_ctor);
    assert_static_vector_values(v, expected);
}

TEST_F(StaticVectorTest, ConstructWithInitializerList) {
    std::size_t const size = VA_NARGS(TEST_VALUES);
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };

    EXPECT_EQ(size, custom_type::num_instances());
    int values[] = { TEST_VALUES };
    expected_result<size> expected(values, constructed_with::copy_ctor);
    assert_static_vector_values(v, expected);
}

TEST_F(StaticVectorTest, ConstructWithAnotherVector) {
    std::size_t const size = VA_NARGS(TEST_VALUES);
    static_vector<custom_type, SIZE_VECTOR> v1 = { WRAP_VALUES(custom_type, TEST_VALUES) };
    decltype(v1) v2 = v1;

    EXPECT_EQ(size * 2, custom_type::num_instances());
    int values[] = { TEST_VALUES };
    expected_result<size> expected(values, constructed_with::copy_ctor);
    assert_static_vector_values(v2, expected);
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
    int values[] = { EXTRA_TEST_VALUES };
    expected_result<size> expected(values, constructed_with::copy_ctor);
    assert_static_vector_values(v, expected);
}

TEST_F(StaticVectorTest, AssignWithOperatorAndAnotherVector) {
    std::size_t const size = VA_NARGS(EXTRA_TEST_VALUES);
    static_vector<custom_type, SIZE_VECTOR> v1 = { WRAP_VALUES(custom_type, TEST_VALUES) };
    decltype(v1) v2 = { WRAP_VALUES(custom_type, EXTRA_TEST_VALUES) };
    v1 = v2;

    EXPECT_EQ(size * 2, custom_type::num_instances());
    int values[] = { EXTRA_TEST_VALUES };
    expected_result<size> expected(values, constructed_with::copy_ctor);
    assert_static_vector_values(v2, expected);
}

TEST_F(StaticVectorTest, AssignWithMethodAndCountAndValues) {
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };
    v.assign(REPEAT_COUNT, custom_type(constructed_with::skipped, inserted_value));

    EXPECT_EQ(REPEAT_COUNT, custom_type::num_instances());
    expected_result<REPEAT_COUNT> expected(inserted_value, constructed_with::copy_ctor);
    assert_static_vector_values(v, expected);
}

TEST_F(StaticVectorTest, AssignWithMethodAndInitializerList) {
    std::size_t const size = VA_NARGS(EXTRA_TEST_VALUES);
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };
    v.assign({ WRAP_VALUES(custom_type, EXTRA_TEST_VALUES) });

    EXPECT_EQ(size, custom_type::num_instances());
    int values[] = { EXTRA_TEST_VALUES };
    expected_result<size> expected(values, constructed_with::copy_ctor);
    assert_static_vector_values(v, expected);
}

TEST_F(StaticVectorTest, AssignWithMethodAndIteratprPair) {
    std::size_t const size = VA_NARGS(EXTRA_TEST_VALUES);
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };
    std::array<custom_type, size> arr = { WRAP_VALUES(custom_type, EXTRA_TEST_VALUES) };
    v.assign(arr.begin(), arr.end());

    EXPECT_EQ(size * 2, custom_type::num_instances());
    int values[] = { EXTRA_TEST_VALUES };
    expected_result<size> expected(values, constructed_with::copy_ctor);
    assert_static_vector_values(v, expected);
}

TEST_F(StaticVectorTest, PushBackCopiedValue) {
    std::size_t const size = VA_NARGS(TEST_VALUES) + 1;
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };

    custom_type item(constructed_with::skipped, inserted_value);
    v.push_back(item);

    EXPECT_EQ(size + 1, custom_type::num_instances());
    int values[] = { TEST_VALUES };
    expected_result<size> expected;
    expected.assign(0, size - 1, values, constructed_with::copy_ctor);
    expected.assign(size - 1, 1, inserted_value, constructed_with::copy_ctor);
    assert_static_vector_values(v, expected);
}

TEST_F(StaticVectorTest, PushBackMovedValue) {
    std::size_t const size = VA_NARGS(TEST_VALUES) + 1;
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };
    v.push_back(custom_type(constructed_with::skipped, inserted_value));

    EXPECT_EQ(size, custom_type::num_instances());
    int values[] = { TEST_VALUES };
    expected_result<size> expected;
    expected.assign(0, size - 1, values, constructed_with::copy_ctor);
    expected.assign(size - 1, 1, inserted_value, constructed_with::move_ctor);
    assert_static_vector_values(v, expected);
}

TEST_F(StaticVectorTest, EmplaceBackValue) {
    std::size_t const size = VA_NARGS(TEST_VALUES) + 1;
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };
    v.emplace_back(inserted_value);

    EXPECT_EQ(size, custom_type::num_instances());
    int values[] = { TEST_VALUES };
    expected_result<size> expected;
    expected.assign(0, size - 1, values, constructed_with::copy_ctor);
    expected.assign(size - 1, 1, inserted_value, constructed_with::value_ctor);
    assert_static_vector_values(v, expected);
}

TEST_F(StaticVectorTest, PopBack) {
    std::size_t const size = VA_NARGS(TEST_VALUES) - 1;
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };
    v.pop_back();

    EXPECT_EQ(size, custom_type::num_instances());
    int values[] = { TEST_VALUES };
    expected_result<size> expected(values, constructed_with::copy_ctor);
    assert_static_vector_values(v, expected);
}

TEST_F(StaticVectorTest, TraverseWithIterator) {
    std::size_t const size = VA_NARGS(TEST_VALUES);
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };

    typename static_vector<custom_type, SIZE_VECTOR>::iterator it = v.begin();
    typename static_vector<custom_type, SIZE_VECTOR>::const_iterator cit = v.cbegin();
    for (std::size_t pos = 0; pos < size; ++pos, ++it, ++cit) {
        std::ostringstream ss;
        ss << "pos = " << pos;
        SCOPED_TRACE(ss.str());

        EXPECT_EQ(v[pos], *it);
        EXPECT_EQ(v[pos], *cit);
    }

    EXPECT_EQ(v.end(), it);
    EXPECT_EQ(v.cend(), cit);
}

TEST_F(StaticVectorTest, TraverseWithReverseIterator) {
    std::size_t const size = VA_NARGS(TEST_VALUES);
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };

    typename static_vector<custom_type, SIZE_VECTOR>::reverse_iterator rit = v.rbegin();
    typename static_vector<custom_type, SIZE_VECTOR>::const_reverse_iterator crit = v.crbegin();
    for (std::size_t pos = size; pos > 0; --pos, ++rit, ++crit) {
        std::ostringstream ss;
        ss << "pos = " << pos;
        SCOPED_TRACE(ss.str());

        EXPECT_EQ(v[pos - 1], *rit);
        EXPECT_EQ(v[pos - 1], *crit);
    }

    EXPECT_EQ(v.rend(), rit);
    EXPECT_EQ(v.crend(), crit);
}

TEST_F(StaticVectorTest, AccessFirstElement) {
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };
    EXPECT_EQ(v[0], v.front());
}

TEST_F(StaticVectorTest, AccessLastElement) {
    static_vector<custom_type, SIZE_VECTOR> v = { WRAP_VALUES(custom_type, TEST_VALUES) };
    EXPECT_EQ(v[v.size() - 1], v.back());
}

TEST_F(StaticVectorTest, InsertCopiedValue) {
    using vector = static_vector<custom_type, SIZE_VECTOR>;
    custom_type item(constructed_with::skipped, inserted_value);
    auto insert = [&item](vector& v, typename vector::iterator it) {
        v.insert(it, item);
    };

    test_insert<1>(insert, 1, constructed_with::copy_ctor);
}

TEST_F(StaticVectorTest, InsertMovedValue) {
    using vector = static_vector<custom_type, SIZE_VECTOR>;
    auto insert = [](vector& v, typename vector::iterator it) {
        v.insert(it, custom_type(constructed_with::skipped, inserted_value));
    };

    test_insert<1>(insert, 0, constructed_with::move_ctor);
}

TEST_F(StaticVectorTest, InsertRepeatedValues) {
    using vector = static_vector<custom_type, SIZE_VECTOR>;
    auto insert = [](vector& v, typename vector::iterator it) {
        v.insert(it, REPEAT_COUNT, custom_type(constructed_with::skipped, inserted_value));
    };

    test_insert<REPEAT_COUNT>(insert, 0, constructed_with::copy_ctor);
}

TEST_F(StaticVectorTest, EmplaceValue) {
    using vector = static_vector<custom_type, SIZE_VECTOR>;
    auto insert = [](vector& v, typename vector::iterator it) {
        v.emplace(it, inserted_value);
    };

    test_insert<1>(insert, 0, constructed_with::value_ctor);
}
