//
// Created by thomas on 10/10/17.
//

#ifndef UTILITIES_MISC_HPP
#define UTILITIES_MISC_HPP

#include <functional>


template<typename T>
T sum(std::size_t i, const std::size_t& n, const std::function<T(std::size_t)>& expr)
{
    if(i == n)
        return expr(i);
    else
        return expr(i) + sum(i+1, n, expr);
}

template<typename T>
T clamp(T x, T min, T max)
{
    if(x < max)
        return std::max(x, min);
    else
        return max;
}

#endif //UTILITIES_MISC_HPP
