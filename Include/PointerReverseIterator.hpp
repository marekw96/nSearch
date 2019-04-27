#pragma once

#include <iterator>

template <typename T>
class PointerReverseIterator
{
public:
    using self_type = PointerReverseIterator<T>;
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = int;

    PointerReverseIterator(T* start)
        : ptr(start) 
    {}

    self_type operator++() { auto temp = *this; --ptr; return temp; }
    self_type& operator*(int) { --ptr; return *this; }

    reference operator*() { return *ptr; }
    pointer operator->() { return ptr; }


    bool operator==(const self_type& other) { return ptr == other.ptr; }
    bool operator!=(const self_type& other) { return ptr != other.ptr; }

private:
    T* ptr;
};
