//
// Created by thomas on 10/10/17.
//

#ifndef UTILITIES_CHOOSE_HPP
#define UTILITIES_CHOOSE_HPP

namespace ut
{
    template<bool b, typename isTrue, typename isFalse>
    struct choose;

    template<typename isTrue, typename isFalse>
    struct choose<true, isTrue, isFalse> { using type = isTrue; };

    template<typename isTrue, typename isFalse>
    struct choose<false, isTrue, isFalse> { using type = isFalse; };
};

#endif //UTILITIES_CHOOSE_HPP
