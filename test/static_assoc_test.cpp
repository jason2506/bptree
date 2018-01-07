/************************************************
 *  static_assoc_test.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#include <cstddef>

#include <algorithm>
#include <list>
#include <memory>
#include <sstream>
#include <type_traits>

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

using test_map = static_map<int, char, assoc_size>;
using test_multimap = static_multimap<int, char, assoc_size>;
using test_value_type = typename test_map::value_type;
using test_value_list = std::initializer_list<test_value_type>;

test_value_list constexpr test_values = {
    test_value_type(6, 'a'),
    test_value_type(1, 'b'),
    test_value_type(5, 'c'),
    test_value_type(7, 'd'),
    test_value_type(3, 'e')
};

test_value_list constexpr sorted_test_values = {
    test_value_type(1, 'b'),
    test_value_type(3, 'e'),
    test_value_type(5, 'c'),
    test_value_type(6, 'a'),
    test_value_type(7, 'd')
};

test_value_type constexpr duplicated_inserted_value(6, 'x');
std::size_t constexpr duplicated_index = 3;

test_value_list constexpr sorted_test_values_with_inserted_duplication = {
    test_value_type(1, 'b'),
    test_value_type(3, 'e'),
    test_value_type(5, 'c'),
    test_value_type(6, 'a'),
    duplicated_inserted_value,
    test_value_type(7, 'd')
};

test_value_list constexpr test_values_with_duplications = {
    test_value_type(2, 'a'),
    test_value_type(2, 'b'),
    test_value_type(1, 'c'),
    test_value_type(2, 'd'),
    test_value_type(1, 'e')
};

test_value_list constexpr sorted_test_values_with_duplications = {
    test_value_type(1, 'c'),
    test_value_type(1, 'e'),
    test_value_type(2, 'a'),
    test_value_type(2, 'b'),
    test_value_type(2, 'd')
};

test_value_list constexpr sorted_test_values_without_duplications = {
    test_value_type(1, 'c'),
    test_value_type(2, 'a')
};

test_value_list constexpr extra_test_values = {
    test_value_type(8, 'f'),
    test_value_type(2, 'g'),
    test_value_type(4, 'h')
};

test_value_list constexpr all_sorted_test_values = {
    test_value_type(1, 'b'),
    test_value_type(2, 'g'),
    test_value_type(3, 'e'),
    test_value_type(4, 'h'),
    test_value_type(5, 'c'),
    test_value_type(6, 'a'),
    test_value_type(7, 'd'),
    test_value_type(8, 'f'),
};

std::size_t constexpr num_test_values = test_values.size();

template <typename Assoc, typename ValueList>
void assert_assoc_values(Assoc const& assoc, ValueList list) {
    auto map_it = assoc.begin();
    auto expected_it = list.begin();
    while (expected_it != list.end()) {
        EXPECT_EQ(*expected_it++, *map_it++);
    }

    EXPECT_EQ(assoc.end(), map_it);
}

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

TEST(StaticAssocTest, ConstructWithSortedValues) {
    test_map map(sorted_test_values.begin(), sorted_test_values.end());
    assert_assoc_values(map, sorted_test_values);
}

TEST(StaticAssocTest, ConstructWithUnsortedValues) {
    test_map map(test_values.begin(), test_values.end());
    assert_assoc_values(map, sorted_test_values);
}

TEST(StaticAssocTest, ConstructWithInitializerList) {
    test_map map(test_values);
    assert_assoc_values(map, sorted_test_values);
}

TEST(StaticAssocTest, ConstructMapWithDuplicatedKeys) {
    test_map map(test_values_with_duplications);
    assert_assoc_values(map, sorted_test_values_without_duplications);
}

TEST(StaticAssocTest, ConstructMultiMapWithDuplicatedKeys) {
    test_multimap map(test_values_with_duplications);
    assert_assoc_values(map, sorted_test_values_with_duplications);
}

TEST(StaticAssocTest, AssignToInitializerList) {
    test_map map(extra_test_values);
    map = test_values;
    assert_assoc_values(map, sorted_test_values);
}

TEST(StaticAssocTest, EmplaceSingleValue) {
    for (std::size_t selected_idx = 0; selected_idx < num_test_values; ++selected_idx) {
        // construct `value_list` with `test_values` except for value at `selected_idx`
        auto selected_it = test_values.begin() + selected_idx;
        std::list<test_value_type> value_list;
        value_list.insert(value_list.end(), test_values.begin(), selected_it);
        value_list.insert(value_list.end(), selected_it + 1, test_values.end());

        {
            auto assoc_ptr = std::make_unique<test_map>(value_list.begin(), value_list.end());
            auto result = assoc_ptr->emplace(*selected_it);

            assert_assoc_values(*assoc_ptr, sorted_test_values);
            EXPECT_EQ(test_value_type(*selected_it), *result.first);
            EXPECT_TRUE(result.second);
        }

        for (std::size_t hint_offset = 0; hint_offset < num_test_values; ++hint_offset) {
            std::ostringstream ss;
            ss << "selected_idx = " << selected_idx << "\thint_offset = " << hint_offset;
            SCOPED_TRACE(ss.str());

            // try to emplace selected value with hint (may be incorrect)
            auto assoc_ptr = std::make_unique<test_map>(value_list.begin(), value_list.end());
            auto it = assoc_ptr->emplace_hint(assoc_ptr->begin() + hint_offset, *selected_it);

            assert_assoc_values(*assoc_ptr, sorted_test_values);
            EXPECT_EQ(test_value_type(*selected_it), *it);
        }
    }
}

TEST(StaticAssocTest, EmplaceValueWithDuplicatedKeyIntoMap) {
    {
        test_map map(test_values);
        auto duplicated_it = map.begin() + duplicated_index;

        auto result = map.emplace(duplicated_inserted_value);

        EXPECT_EQ(duplicated_it, result.first);
        EXPECT_FALSE(result.second);
        assert_assoc_values(map, sorted_test_values);
    }

    for (std::size_t hint_offset = 0; hint_offset < num_test_values; ++hint_offset) {
        std::ostringstream ss;
        ss << "hint_offset = " << hint_offset;
        SCOPED_TRACE(ss.str());

        // try to emplace selected value with hint (may be incorrect)
        test_map map(test_values);
        auto duplicated_it = map.begin() + duplicated_index;

        auto it = map.emplace_hint(map.begin() + hint_offset, duplicated_inserted_value);

        EXPECT_EQ(duplicated_it, it);
        assert_assoc_values(map, sorted_test_values);
    }
}

TEST(StaticAssocTest, EmplaceValueWithDuplicatedKeyIntoMultiMap) {
    {
        test_multimap map(test_values);
        auto duplicated_it = map.begin() + duplicated_index;

        auto result = map.emplace(duplicated_inserted_value);

        EXPECT_EQ(duplicated_it + 1, result);
        assert_assoc_values(map, sorted_test_values_with_inserted_duplication);
    }

    for (std::size_t hint_offset = 0; hint_offset < num_test_values; ++hint_offset) {
        std::ostringstream ss;
        ss << "hint_offset = " << hint_offset;
        SCOPED_TRACE(ss.str());

        // try to emplace selected value with hint (may be incorrect)
        test_multimap map(test_values);
        auto duplicated_it = map.begin() + duplicated_index;

        auto it = map.emplace_hint(map.begin() + hint_offset, duplicated_inserted_value);

        EXPECT_EQ(duplicated_it + 1, it);
        assert_assoc_values(map, sorted_test_values_with_inserted_duplication);
    }
}

TEST(StaticAssocTest, InsertValueConstructible) {
    struct foo {
        explicit foo(int i) : i(i) { /* do nothing */ }
        int i;
    };

    struct bar {
        explicit bar(int i) : v(foo(i)) { /* do nothing */ }
        explicit bar(foo const& v) : v(v) { /* do nothing */ }
        bool operator==(bar const& other) const { return v.i == other.v.i; }
        foo v;
    };

    auto compare = [](bar const& x, bar const& y) { return x.v.i < y.v.i; };

    auto init_values = { bar(3), bar(1), bar(4) };
    foo inserted_value(2);
    auto expected_values = { bar(1), bar(2), bar(3), bar(4) };

    using set_type = static_multiset<bar, assoc_size, decltype(compare)>;

    // insert without hint
    {
        set_type set(init_values, compare);

        auto it = set.insert(inserted_value);

        EXPECT_EQ(bar(inserted_value), *it);
        assert_assoc_values(set, expected_values);
    }

    // insert with hint
    for (std::size_t hint_offset = 0; hint_offset < init_values.size(); ++hint_offset) {
        std::ostringstream ss;
        ss << "hint_offset = " << hint_offset;
        SCOPED_TRACE(ss.str());

        set_type set(init_values, compare);

        auto it = set.insert(set.begin() + hint_offset, inserted_value);

        EXPECT_EQ(bar(inserted_value), *it);
        assert_assoc_values(set, expected_values);
    }
}

TEST(StaticAssocTest, InsertIteratorPair) {
    test_map map(test_values.begin(), test_values.end());
    map.insert(extra_test_values.begin(), extra_test_values.end());
    assert_assoc_values(map, all_sorted_test_values);
}

TEST(StaticAssocTest, EraseWithKey) {
    using value_type = std::pair<int const, char>;
    static_multimap<int, char, assoc_size> map({
        value_type(1, 'a'),
        value_type(4, 'b'),
        value_type(4, 'c'),
        value_type(6, 'd'),
        value_type(6, 'e'),
        value_type(6, 'f'),
        value_type(7, 'g'),
        value_type(7, 'h'),
        value_type(7, 'i'),
        value_type(7, 'j')
    });

    auto num_erased = map.erase(6);
    auto expected_values = {
        value_type(1, 'a'),
        value_type(4, 'b'),
        value_type(4, 'c'),
        value_type(7, 'g'),
        value_type(7, 'h'),
        value_type(7, 'i'),
        value_type(7, 'j')
    };

    assert_assoc_values(map, expected_values);
    EXPECT_EQ(3, num_erased);
}

TEST(StaticAssocTest, CountAndFindValues) {
    using value_type = std::pair<int const, char>;
    static_multimap<int, char, assoc_size> map({
        value_type(1, 'a'),
        value_type(4, 'b'),
        value_type(4, 'c'),
        value_type(6, 'd'),
        value_type(6, 'e'),
        value_type(6, 'f'),
        value_type(7, 'g'),
        value_type(7, 'h'),
        value_type(7, 'i'),
        value_type(7, 'j')
    });

    EXPECT_EQ(1, map.count(1));
    EXPECT_EQ(2, map.count(4));
    EXPECT_EQ(3, map.count(6));
    EXPECT_EQ(4, map.count(7));

    EXPECT_EQ(map.begin(), map.find(1));
    EXPECT_EQ(map.begin() + 1, map.find(4));
    EXPECT_EQ(map.begin() + 1 + 2, map.find(6));
    EXPECT_EQ(map.begin() + 1 + 2 + 3, map.find(7));
}
