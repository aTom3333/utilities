//
// Created by thomas on 10/10/17.
//

#ifndef UTILITIES_PTR_ITERATOR_HPP
#define UTILITIES_PTR_ITERATOR_HPP

#include <iterator>
#include "choose.hpp"

namespace ut
{
    template<typename T, bool is_const = false>
    struct pointer_iterator_base
    {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using pointer = typename choose<is_const, const T *, T *>::type;
        using reference = typename choose<is_const, const T &, T &>::type;
        using difference_type = std::ptrdiff_t;

        pointer_iterator_base() = default;

        explicit pointer_iterator_base(pointer p) noexcept : p_{p} {}

        pointer_iterator_base(const pointer_iterator_base<T, false> &it) noexcept : p_{it.p_} {}

        // operators
        bool operator==(const pointer_iterator_base<T, true> other) const noexcept { return p_ == other.p_; }

        bool operator!=(const pointer_iterator_base<T, true> other) const noexcept { return p_ != other.p_; }

        reference operator*() const noexcept { return *p_; }

        pointer operator->() const noexcept { return p_; }

        pointer_iterator_base &operator++() noexcept
        {
            ++p_;
            return *this;
        }

        pointer_iterator_base operator++(int) noexcept
        {
            auto temp(*this);
            ++p_;
            return temp;
        }

        pointer_iterator_base &operator--() noexcept
        {
            --p_;
            return *this;
        }

        pointer_iterator_base operator--(int) noexcept
        {
            auto temp(*this);
            --p_;
            return temp;
        }

        friend pointer_iterator_base
        operator+(const pointer_iterator_base &a, long long b) { return pointer_iterator_base(a.p_ + b); }

        friend pointer_iterator_base
        operator+(long long b, const pointer_iterator_base &a) noexcept { return pointer_iterator_base(a.p_ + b); }

        pointer_iterator_base operator-(long long rhs) const noexcept { return pointer_iterator_base(p_ - rhs); }

        difference_type operator-(const pointer_iterator_base &rhs) const noexcept { return p_ - rhs.p_; }

        bool operator<(const pointer_iterator_base &rhs) const noexcept { return p_ < rhs.p_; }

        bool operator>(const pointer_iterator_base &rhs) const noexcept { return p_ > rhs.p_; }

        bool operator<=(const pointer_iterator_base &rhs) const noexcept { return p_ <= rhs.p_; }

        bool operator>=(const pointer_iterator_base &rhs) const noexcept { return p_ >= rhs.p_; }

        pointer_iterator_base &operator+=(long long n) noexcept
        {
            p_ += n;
            return *this;
        }

        pointer_iterator_base &operator-=(long long n) noexcept
        {
            p_ -= n;
            return *this;
        }

        reference operator[](std::size_t i) const noexcept { return p_[i]; }


        pointer p_ = nullptr;
    };
};

#endif //UTILITIES_PTR_ITERATOR_HPP
