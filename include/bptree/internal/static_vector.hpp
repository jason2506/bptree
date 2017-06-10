/************************************************
 *  static_vector.hpp
 *  bptree
 *
 *  Copyright (c) 2017, Chi-En Wu
 *  Distributed under MIT License
 ************************************************/

#ifndef BPTREE_INTERNAL_STATIC_VECTOR_HPP_
#define BPTREE_INTERNAL_STATIC_VECTOR_HPP_

#include <cassert>
#include <cstddef>

#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace bptree {

namespace internal {

/************************************************
 * Declaration: class static_vector_iterator<P>
 ************************************************/

template <typename Pointer>
class static_vector_iterator {
 public:  // Public Type(s)
    using iterator_category = typename std::iterator_traits<Pointer>::iterator_category;
    using value_type = typename std::iterator_traits<Pointer>::value_type;
    using difference_type = typename std::iterator_traits<Pointer>::difference_type;
    using pointer = typename std::iterator_traits<Pointer>::pointer;
    using reference = typename std::iterator_traits<Pointer>::reference;

 public:  // Public Method(s)
    static_vector_iterator()
      : ptr_(nullptr)
        { /* do nothing */ }
    explicit static_vector_iterator(pointer ptr)
      : ptr_(ptr)
        { /* do nothing */ }

    reference operator*() const
        { return *ptr_; }
    pointer operator->() const
        { return ptr_; }
    reference operator[](difference_type n) const
        { return ptr_[n]; }

    static_vector_iterator& operator++()
        { ++ptr_; return *this; }
    static_vector_iterator& operator--()
        { --ptr_; return *this; }
    static_vector_iterator operator++(int) const
        { static_vector_iterator it(*this); ++ptr_; return it; }
    static_vector_iterator operator--(int) const
        { static_vector_iterator it(*this); --ptr_; return it; }

    static_vector_iterator& operator+=(difference_type n)
        { ptr_ += n; return *this; }
    static_vector_iterator& operator-=(difference_type n)
        { ptr_ -= n; return *this; }

    difference_type operator-(static_vector_iterator const& other) const
        { return ptr_ - other.ptr_; }
    static_vector_iterator operator+(difference_type n) const
        { return static_vector_iterator(ptr_ + n); }
    static_vector_iterator operator-(difference_type n) const
        { return static_vector_iterator(ptr_ - n); }
    friend static_vector_iterator operator+(difference_type n, static_vector_iterator const& other)
        { return static_vector_iterator(n + other.ptr_); }
    friend static_vector_iterator operator-(difference_type n, static_vector_iterator const& other)
        { return static_vector_iterator(n - other.ptr_); }

    bool operator==(static_vector_iterator const& other) const noexcept
        { return ptr_ == other.ptr_; }
    bool operator!=(static_vector_iterator const& other) const noexcept
        { return ptr_ != other.ptr_; }
    bool operator> (static_vector_iterator const& other) const noexcept
        { return ptr_ >  other.ptr_; }
    bool operator< (static_vector_iterator const& other) const noexcept
        { return ptr_ <  other.ptr_; }
    bool operator>=(static_vector_iterator const& other) const noexcept
        { return ptr_ >= other.ptr_; }
    bool operator<=(static_vector_iterator const& other) const noexcept
        { return ptr_ <= other.ptr_; }

    template <typename AnotherPointer>
    operator static_vector_iterator<AnotherPointer>() const noexcept
        { return static_vector_iterator<AnotherPointer>(ptr_); }

 private:  // Private Property(ies)
    pointer ptr_;
};

/************************************************
 * Declaration: class static_vector<T, N>
 ************************************************/

template <typename T, std::size_t N>
class static_vector {
 public:  // Public Type(s)
    using value_type = T;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using iterator = static_vector_iterator<pointer>;
    using const_iterator = static_vector_iterator<const_pointer>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

 public:  // Public Method(s)
    static_vector();
    explicit static_vector(size_type count);
    static_vector(size_type count, value_type const& value);
    template <typename InputIt>
    static_vector(InputIt first, InputIt last);
    static_vector(std::initializer_list<value_type> il);
    static_vector(static_vector const& other);
    static_vector(static_vector&& other);
    ~static_vector();

    static_vector& operator=(std::initializer_list<value_type> il);
    static_vector& operator=(static_vector const& other);
    static_vector& operator=(static_vector&& other);
    void assign(size_type count, value_type const& value);
    void assign(std::initializer_list<value_type> il);
    template <typename InputIt>
    void assign(InputIt first, InputIt last);
    void swap(static_vector& other);

    iterator insert(const_iterator pos, value_type const& value);
    iterator insert(const_iterator pos, value_type&& value);
    iterator insert(const_iterator pos, size_type count, value_type const& value);
    template <typename InputIt>
    std::enable_if_t<
        std::is_same<
            typename std::iterator_traits<InputIt>::iterator_category,
            std::input_iterator_tag
        >::value,
        typename static_vector<T, N>::iterator
    >
    insert(const_iterator pos, InputIt first, InputIt last);
    template <typename ForwardIt>
    std::enable_if_t<
        std::is_convertible<
            typename std::iterator_traits<ForwardIt>::iterator_category,
            std::forward_iterator_tag
        >::value,
        typename static_vector<T, N>::iterator
    >
    insert(const_iterator pos, ForwardIt first, ForwardIt last);
    iterator insert(const_iterator pos, std::initializer_list<value_type> il);
    void push_back(value_type const& value);
    void push_back(value_type&& value);
    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args);
    template <typename... Args>
    void emplace_back(Args&&... args);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void pop_back();
    void clear() noexcept;

    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    value_type* data() noexcept;
    value_type const* data() const noexcept;

    bool empty() const noexcept;
    bool full() const noexcept;
    size_type size() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr size_type capacity() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

 private:  // Private Method(s)
    template <typename... Args>
    iterator emplace_with_count(const_iterator pos, size_type count, Args&&... args);
    void reserve(const_iterator pos, size_type count);

 private:  // Private Property(ies)
    size_type size_;
    std::aligned_storage_t<sizeof(T), alignof(T)> data_[N];
};

/************************************************
 * Implementation: class static_vector<T, N>
 ************************************************/

template <typename T, std::size_t N>
inline static_vector<T, N>::static_vector()
  : size_(0), data_() {
    // do nothing
}

template <typename T, std::size_t N>
static_vector<T, N>::static_vector(size_type count)
  : size_(count), data_() {
    assert(count <= max_size());

    auto first = data();
    auto last = first + size();
    auto current = first;
    try {
        for (; current != last; ++current) {
            ::new(current) value_type;
        }
    } catch (...) {
        for (; first != current; ++first) {
            first->~value_type();
        }
        throw;
    }
}

template <typename T, std::size_t N>
inline static_vector<T, N>::static_vector(size_type count, value_type const& value)
  : size_(count), data_() {
    assert(count <= max_size());

    auto first = data();
    auto last = first + size();
    std::uninitialized_fill(first, last, value);
}

template <typename T, std::size_t N>
template <typename InputIt>
inline static_vector<T, N>::static_vector(InputIt first, InputIt last)
  : static_vector() {
    auto data_first = data();
    auto data_last = std::uninitialized_copy(first, last, data_first);
    size_ = data_last - data_first;
    assert(size() <= max_size());
}

template <typename T, std::size_t N>
inline static_vector<T, N>::static_vector(std::initializer_list<value_type> il)
  : static_vector(il.begin(), il.end()) {
    // do nothing
}

template <typename T, std::size_t N>
inline static_vector<T, N>::static_vector(static_vector const& other)
  : static_vector(other.data(), other.data() + other.size()) {
    // do nothing
}

template <typename T, std::size_t N>
inline static_vector<T, N>::static_vector(static_vector&& other)
  : static_vector(std::make_move_iterator(other.data()),
                  std::make_move_iterator(other.data() + other.size())) {
    other.clear();
}

template <typename T, std::size_t N>
inline static_vector<T, N>::~static_vector() {
    clear();
}

template <typename T, std::size_t N>
inline static_vector<T, N>& static_vector<T, N>::operator=(std::initializer_list<value_type> il) {
    assign(il);
    return *this;
}

template <typename T, std::size_t N>
inline static_vector<T, N>& static_vector<T, N>::operator=(static_vector const& other) {
    assign(other.data(), other.data() + other.size());
    return *this;
}

template <typename T, std::size_t N>
inline static_vector<T, N>& static_vector<T, N>::operator=(static_vector&& other) {
    assign(std::make_move_iterator(other.data()),
           std::make_move_iterator(other.data() + other.size()));
    other.clear();
    return *this;
}

template <typename T, std::size_t N>
inline void static_vector<T, N>::assign(size_type count, value_type const& value) {
    assert(count <= max_size());

    clear();

    auto first = data();
    auto last = first + count;
    size_ = count;
    std::uninitialized_fill(first, last, value);
}

template <typename T, std::size_t N>
inline void static_vector<T, N>::assign(std::initializer_list<value_type> il) {
    assign(il.begin(), il.end());
}

template <typename T, std::size_t N>
template <typename InputIt>
inline void static_vector<T, N>::assign(InputIt first, InputIt last) {
    clear();

    auto ptr = std::uninitialized_copy(first, last, data());
    size_ = ptr - data();
    assert(size() <= max_size());
}

template <typename T, std::size_t N>
void static_vector<T, N>::swap(static_vector& other) {
    decltype(data()) short_first, short_last, long_first, long_mid, long_last;
    if (size() < other.size()) {
        short_first = data();
        short_last = short_first + size();
        long_first = other.data();
        long_mid = long_first + size();
        long_last = long_first + other.size();
    } else {
        short_first = other.data();
        short_last = short_first + other.size();
        long_first = data();
        long_mid = long_first + other.size();
        long_last = long_first + size();
    }

    std::swap_ranges(short_first, short_last, long_first);
    for (; long_mid != long_last; ++long_mid, ++short_last) {
        ::new(short_last) value_type(std::move(*long_mid));
        long_mid->~value_type();
    }

    std::swap(size_, other.size_);
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::iterator
static_vector<T, N>::insert(const_iterator pos, value_type const& value) {
    return emplace(pos, value);
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::iterator
static_vector<T, N>::insert(const_iterator pos, value_type&& value) {
    return emplace(pos, std::move(value));
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::iterator
static_vector<T, N>::insert(const_iterator pos, size_type count, value_type const& value) {
    return emplace_with_count(pos, count, value);
}

template <typename T, std::size_t N>
template <typename InputIt>
std::enable_if_t<
    std::is_same<
        typename std::iterator_traits<InputIt>::iterator_category,
        std::input_iterator_tag
    >::value,
    typename static_vector<T, N>::iterator
>
static_vector<T, N>::insert(const_iterator pos, InputIt first, InputIt last) {
    auto offset = pos - cbegin();
    for (; first != last; ++first, ++pos) {
        pos = emplace(pos, *first);
    }

    return begin() + offset;
}

template <typename T, std::size_t N>
template <typename ForwardIt>
std::enable_if_t<
    std::is_convertible<
        typename std::iterator_traits<ForwardIt>::iterator_category,
        std::forward_iterator_tag
    >::value,
    typename static_vector<T, N>::iterator
>
static_vector<T, N>::insert(const_iterator pos, ForwardIt first, ForwardIt last) {
    auto count = std::distance(first, last);
    assert(size() + count <= max_size());

    auto offset = pos - cbegin();
    auto ptr = data() + offset;
    if (count == 0) {
        return iterator(ptr);
    }

    reserve(pos, count);

    auto d_first = ptr;
    while (first != last) {
        ::new(d_first) value_type(*first);
        ++d_first;
        ++first;
    }

    size_ += count;

    return iterator(ptr);
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::iterator
static_vector<T, N>::insert(const_iterator pos, std::initializer_list<value_type> il) {
    return insert(pos, il.begin(), il.end());
}

template <typename T, std::size_t N>
inline void static_vector<T, N>::push_back(value_type const& value) {
    emplace_back(value);
}

template <typename T, std::size_t N>
inline void static_vector<T, N>::push_back(value_type&& value) {
    emplace_back(std::move(value));
}

template <typename T, std::size_t N>
template <typename... Args>
inline typename static_vector<T, N>::iterator
static_vector<T, N>::emplace(const_iterator pos, Args&&... args) {
    return emplace_with_count(pos, 1, std::forward<Args>(args)...);
}

template <typename T, std::size_t N>
template <typename... Args>
inline void static_vector<T, N>::emplace_back(Args&&... args) {
    assert(!full());
    ::new(data() + size()) value_type(std::forward<Args>(args)...);
    ++size_;
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::iterator static_vector<T, N>::erase(const_iterator pos) {
    return erase(pos, pos + 1);
}

template <typename T, std::size_t N>
typename static_vector<T, N>::iterator
static_vector<T, N>::erase(const_iterator first, const_iterator last) {
    assert(first >= cbegin());
    assert(last >= first);
    assert(last <= cend());

    auto offset = first - cbegin();
    auto count = last - first;
    if (count == 0) {
        return iterator(data() + offset);
    }

    auto d_ptr = data() + offset;
    auto s_ptr = d_ptr + count;
    auto last_ptr = data() + size();
    while (s_ptr != last_ptr) {
        d_ptr->~value_type();
        ::new(d_ptr) value_type(std::move(*s_ptr++));
        ++d_ptr;
    }

    while (d_ptr != last_ptr) {
        d_ptr->~value_type();
        ++d_ptr;
    }

    size_ -= count;

    return iterator(data() + offset);
}

template <typename T, std::size_t N>
inline void static_vector<T, N>::pop_back() {
    assert(!empty());
    at(size() - 1).~value_type();
    --size_;
}

template <typename T, std::size_t N>
inline void static_vector<T, N>::clear() noexcept {
    for (size_type pos = 0; pos < size(); ++pos) {
        at(pos).~value_type();
    }

    size_ = 0;
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::reference
static_vector<T, N>::operator[](size_type pos) {
    assert(pos < size());
    return *(data() + pos);
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::const_reference
static_vector<T, N>::operator[](size_type pos) const {
    assert(pos < size());
    return *(data() + pos);
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::reference static_vector<T, N>::at(size_type pos) {
    return const_cast<reference>(
        static_cast<static_vector const*>(this)->at(pos));
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::const_reference static_vector<T, N>::at(size_type pos) const {
    if (pos >= size()) {
        throw std::out_of_range("index out of bounds");
    }

    return operator[](pos);
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::reference static_vector<T, N>::front() {
    assert(!empty());
    return *begin();
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::const_reference static_vector<T, N>::front() const {
    assert(!empty());
    return *begin();
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::reference static_vector<T, N>::back() {
    assert(!empty());
    return *rbegin();
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::const_reference static_vector<T, N>::back() const {
    assert(!empty());
    return *rbegin();
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::value_type* static_vector<T, N>::data() noexcept {
    return reinterpret_cast<value_type*>(data_);
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::value_type const* static_vector<T, N>::data() const noexcept {
    return reinterpret_cast<value_type const*>(data_);
}

template <typename T, std::size_t N>
inline bool static_vector<T, N>::empty() const noexcept {
    return size() == 0;
}

template <typename T, std::size_t N>
inline bool static_vector<T, N>::full() const noexcept {
    return size() >= max_size();
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::size_type static_vector<T, N>::size() const noexcept {
    return size_;
}

template <typename T, std::size_t N>
inline constexpr typename static_vector<T, N>::size_type
static_vector<T, N>::max_size() const noexcept {
    return N;
}

template <typename T, std::size_t N>
inline constexpr typename static_vector<T, N>::size_type
static_vector<T, N>::capacity() const noexcept {
    return max_size();
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::iterator
static_vector<T, N>::begin() noexcept {
    return iterator(data());
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::const_iterator
static_vector<T, N>::begin() const noexcept {
    return cbegin();
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::const_iterator
static_vector<T, N>::cbegin() const noexcept {
    return const_iterator(data());
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::iterator
static_vector<T, N>::end() noexcept {
    return iterator(data() + size());
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::const_iterator
static_vector<T, N>::end() const noexcept {
    return cend();
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::const_iterator
static_vector<T, N>::cend() const noexcept {
    return const_iterator(data() + size());
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::reverse_iterator
static_vector<T, N>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::const_reverse_iterator
static_vector<T, N>::rbegin() const noexcept {
    return crbegin();
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::const_reverse_iterator
static_vector<T, N>::crbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::reverse_iterator
static_vector<T, N>::rend() noexcept {
    return reverse_iterator(begin());
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::const_reverse_iterator
static_vector<T, N>::rend() const noexcept {
    return crend();
}

template <typename T, std::size_t N>
inline typename static_vector<T, N>::const_reverse_iterator
static_vector<T, N>::crend() const noexcept {
    return const_reverse_iterator(begin());
}

template <typename T, std::size_t N>
template <typename... Args>
typename static_vector<T, N>::iterator
static_vector<T, N>::emplace_with_count(const_iterator pos, size_type count, Args&&... args) {
    auto offset = pos - cbegin();
    auto ptr = data() + offset;
    if (count == 0) {
        return iterator(ptr);
    }

    reserve(pos, count);
    auto d_last = ptr + count;
    while (ptr != d_last) {
        ::new(--d_last) value_type(std::forward<Args>(args)...);
    }

    size_ += count;

    return iterator(ptr);
}

template <typename T, std::size_t N>
void static_vector<T, N>::reserve(const_iterator pos, size_type count) {
    assert(pos >= cbegin());
    assert(pos <= cend());
    assert(size() + count <= max_size());

    auto offset = pos - cbegin();
    auto ptr = data() + offset;
    auto last = data() + size();
    auto d_last = last + count;
    while (ptr != last) {
        ::new(--d_last) value_type(std::move(*(--last)));
        last->~value_type();
    }
}

/************************************************
 * Implementation: comparison operators of static_vector<T, N>
 ************************************************/

template <typename T, std::size_t N>
inline bool operator==(static_vector<T, N> const& x, static_vector<T, N> const& y) {
    return x.size() == y.size() && std::equal(x.data(), x.data() + x.size(), y.data());
}

template <typename T, std::size_t N>
inline bool operator!=(static_vector<T, N> const& x, static_vector<T, N> const& y) {
    return !(x == y);
}

template <typename T, std::size_t N>
inline bool operator>(static_vector<T, N> const& x, static_vector<T, N> const& y) {
    return y < x;
}

template <typename T, std::size_t N>
inline bool operator<(static_vector<T, N> const& x, static_vector<T, N> const& y) {
    return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <typename T, std::size_t N>
inline bool operator>=(static_vector<T, N> const& x, static_vector<T, N> const& y) {
    return !(x < y);
}

template <typename T, std::size_t N>
inline bool operator<=(static_vector<T, N> const& x, static_vector<T, N> const& y) {
    return !(x > y);
}

}  // namespace internal

}  // namespace bptree

/************************************************
 * Implementation: std::swap(static_vector<T, N>&, static_vector<T, N>&)
 ************************************************/

namespace std {

template <typename T, std::size_t N>
void swap(bptree::internal::static_vector<T, N>& v1,
          bptree::internal::static_vector<T, N>& v2) {
    v1.swap(v2);
}

}  // namespace std

#endif  // BPTREE_INTERNAL_STATIC_VECTOR_HPP_
