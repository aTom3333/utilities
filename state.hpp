//
// Created by thomas on 30/11/17.
//

#ifndef UTILITIES_STATE_HPP
#define UTILITIES_STATE_HPP

#include <iostream>
#include <iomanip>


struct ostream_state {};

std::ostream& operator<<(std::ostream& out, const ostream_state& s)
{
    auto flags = out.flags();
    out << std::boolalpha;
    out << "boolalapha : " << static_cast<bool>(flags & std::ios::boolalpha) << std::endl
        << "showbase : " << static_cast<bool>(flags & std::ios::showbase) << std::endl
        << "showpoint : " << static_cast<bool>(flags & std::ios::showpoint) << std::endl
        << "showpos : " << static_cast<bool>(flags & std::ios::showpos) << std::endl
        << "skipws : " << static_cast<bool>(flags & std::ios::skipws) << std::endl
        << "unitbuf : " << static_cast<bool>(flags & std::ios::unitbuf) << std::endl
        << "uppercase : " << static_cast<bool>(flags & std::ios::uppercase) << std::endl
        << "basefield : " << (flags & std::ios::dec ? "dec" : (flags & std::ios::hex ? "hex" : "oct")) << std::endl
        << "float format : " << (flags & std::ios::fixed ? "fixed" : "scientific") << std::endl
        << "adjustment : " << (flags & std::ios::internal ? "internal" : (flags & std::ios::left ? "left" : "right")) << std::endl;

    out.flags(flags);

    return out;
}

ostream_state state;

#endif //UTILITIES_STATE_HPP
