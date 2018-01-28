//
// Created by thomas on 10/10/17.
//

#ifndef UTILITIES_RANGE_HPP
#define UTILITIES_RANGE_HPP

#include <utility>


namespace ut
{
    template<typename T>
    struct range
    {
        range(T beg, T end) : b{std::move(beg)}, e{std::move(end)} {}
        range(T beg, T end, T step) : b{std::move(beg)}, e{std::move(end)}, s{std::move(step)},
                                      use_default_step{false} {}

        struct iterator
        {
            iterator(const range& ra, T v) : r{ra}, value{std::move(v)} {}
            iterator& operator++()
            {
                (r.use_default_step ? (r.b < r.e ? ++value : --value) : value += r.s);
                return *this;
            }
            iterator& operator++(int)
            {
                iterator temp = *this;
                (r.use_default_step ? (r.b < r.e ? ++value : --value) : value += r.s);
                return temp;
            }

            bool operator!=(const iterator& other)
            {
                return (r.e < r.b) ? !(value < other.value) : !(other.value < value);
            }

            T& operator*() { return value; }

            const range& r;
            T value;
        };

        iterator begin() const noexcept { return iterator(*this, b); }
        iterator end() const noexcept { return iterator(*this, e); }

        T b, e, s;
        bool use_default_step = true;
    };
};

#endif //UTILITIES_RANGE_HPP
