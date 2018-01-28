//
// Created by thomas on 10/10/17.
//

#ifndef UTILITIES_DCT_HPP
#define UTILITIES_DCT_HPP

#include "array2.hpp"


namespace ut
{
    array2<double> DCT(const array2_view<std::uint8_t>& pixel);

    array2<std::uint8_t> DCT_inv(const array2_view<double>& DCT);

    array2<double> compute_DCT(const array2_view<std::uint8_t>& image, unsigned int N);

    array2<std::uint8_t> compute_DCT_inv(const array2_view<double>& DCT_image, unsigned int N);
};

#endif //UTILITIES_DCT_HPP
