//
// Created by thomas on 29/11/17.
//

#ifndef UTILITIES_BIG_INT_HPP
#define UTILITIES_BIG_INT_HPP

#include <iostream>
#include <iomanip>


template<int N>
constexpr size_t ceil_div(const size_t& n)
{
    return (n % N == 0 ? n/N : n/N + 1);
}


template<size_t N>
class UInt;


template<size_t N>
class UInt
{
    template<size_t M>
    friend class UInt;
    template<size_t M>
    friend constexpr UInt<M> operator~(UInt<M> n) noexcept;
    template<size_t M>
    friend std::ostream& operator<<(std::ostream& out, const UInt<M>& n) noexcept;
    template<size_t L, size_t M>
    friend constexpr auto operator*(const UInt<L>& a, const UInt<M>& b) noexcept;

    public:
        // Constructors
        constexpr UInt() noexcept;
        constexpr UInt(const uint64_t& ull) noexcept;
        constexpr UInt(const UInt& a) noexcept;
        template<size_t M>
        constexpr UInt(const UInt<M>& a) noexcept;

        // Assignment operators
        template<size_t M>
        constexpr UInt& operator=(const UInt<M>& a) noexcept;

        // Bit manipulation-assignement operator
        template<size_t M>
        constexpr UInt& operator&=(const UInt<M>& a) noexcept;
        constexpr UInt& operator&=(const uint64_t& a) noexcept;
        template<size_t M>
        constexpr UInt& operator|=(const UInt<M>& a) noexcept;
        constexpr UInt& operator|=(const uint64_t& a) noexcept;
        template<size_t M>
        constexpr UInt& operator^=(const UInt<M>& a) noexcept;
        constexpr UInt& operator^=(const uint64_t& a) noexcept;
        constexpr UInt& operator<<=(size_t n) noexcept;
        constexpr UInt& operator>>=(size_t n) noexcept;

        // Arithmetic-assignement operators
        template<size_t M>
        constexpr UInt& operator+=(const UInt<M>& a) noexcept;
        constexpr UInt& operator+=(uint64_t a) noexcept;
        template<size_t M>
        constexpr UInt& operator-=(const UInt<M>& a) noexcept;
        constexpr UInt& operator-=(uint64_t a) noexcept;
        template<size_t M>
        constexpr UInt& operator*=(const UInt<M>& a) noexcept;
        constexpr UInt& operator*=(uint64_t a) noexcept;
        template<size_t M>
        constexpr UInt& operator/=(const UInt<M>& a) noexcept;
        template<size_t M>
        constexpr UInt& operator%=(const UInt<M>& a) noexcept;


    private:

        uint32_t data[ceil_div<32>(N)] = {};

        void write_hex(std::ostream& out) const noexcept;
        constexpr void truncate() noexcept;
};

// Constructors
template<size_t N>
constexpr UInt<N>::UInt() noexcept
{
    static_assert(N>0, "number of bits must be greater than 0");
}

template<size_t N>
constexpr UInt<N>::UInt(const uint64_t& ull) noexcept : UInt ()
{
    data[0] = ull & 0xFFFFFFFF;
    if(N > 32) data[1] = ull >> 32;
    truncate();
}

template<size_t N>
constexpr UInt<N>::UInt(const UInt& a) noexcept : UInt()
{
    std::copy(a.data, a.data+ceil_div<32>(N), data);
    truncate();
}

template<size_t N>
template<size_t M>
constexpr UInt<N>::UInt(const UInt<M>& a) noexcept : UInt()
{
    std::copy(a.data, a.data+ceil_div<32>(std::min(N, M)), data);
    truncate();
}


// Assignment operators
template<size_t N>
template<size_t M>
constexpr UInt<N>& UInt<N>::operator=(const UInt<M>& a) noexcept
{
    for(size_t i = 0; i < ceil_div<32>(std::min(N, M)); ++i)
        data[i] = a.data[i];
    for(size_t i = ceil_div<32>(std::min(N, M)); i < ceil_div<32>(N); ++i)
        data[i] = 0;
    truncate();

    return *this;
}


// ~ operator
template<size_t N>
constexpr UInt<N> operator~(UInt<N> n) noexcept
{
    for(auto& elem : n.data)
        elem = ~elem;
    n.truncate();
    return n;
}


// &, &= operators
template<size_t N, size_t M>
constexpr auto operator&(const UInt<N>& a, const UInt<M>& b) noexcept
{
    UInt<std::max(N, M)> c = a;
    return c &= b;
};

template<size_t N>
constexpr auto operator&(const UInt<N>& a, const uint64_t& b) noexcept
{
    return a & UInt<64>(b);
};

template<size_t N>
constexpr auto operator&(const uint64_t& a, const UInt<N>& b) noexcept
{
    return UInt<64>(a) & b;
};

template<size_t N>
template<size_t M>
constexpr UInt<N>& UInt<N>::operator&=(const UInt<M>& a) noexcept
{
    for(size_t i = 0; i < ceil_div<32>(std::min(N, M)); ++i)
        data[i] &= a.data[i];
    for(size_t i = ceil_div<32>(std::min(N, M)); i < ceil_div<32>(N); ++i)
        data[i] = 0;
    truncate();

    return *this;
}

template<size_t N>
constexpr UInt<N>& UInt<N>::operator&=(const uint64_t& a) noexcept
{
    data[0] &= a;
    if(ceil_div<32>(N) > 1)
    {
        data[1] &= (a >> 32);
        for(size_t i = 2; i < ceil_div<32>(N); ++i)
            data[i] = 0;
    }
    truncate();

    return *this;
}


// |, |= operators
template<size_t N, size_t M>
constexpr auto operator|(const UInt<N>& a, const UInt<M>& b) noexcept
{
    UInt<std::max(N, M)> c = a;
    return c |= b;
};

template<size_t N>
constexpr auto operator|(const UInt<N>& a, const uint64_t& b) noexcept
{
    return a | UInt<64>(b);
};

template<size_t N>
constexpr auto operator|(const uint64_t& a, const UInt<N>& b) noexcept
{
    return UInt<64>(a) | b;
};

template<size_t N>
template<size_t M>
constexpr UInt<N>& UInt<N>::operator|=(const UInt<M>& a) noexcept
{
    for(size_t i = 0; i < ceil_div<32>(std::min(N, M)); ++i)
        data[i] |= a.data[i];
    truncate();

    return *this;
}

template<size_t N>
constexpr UInt<N>& UInt<N>::operator|=(const uint64_t& a) noexcept
{
    data[0] |= a;
    if(ceil_div<32>(N) > 1)
        data[1] |= (a >> 32);
    truncate();

    return *this;
}


// ^, ^= operators
template<size_t N, size_t M>
constexpr auto operator^(const UInt<N>& a, const UInt<M>& b) noexcept
{
    UInt<std::max(N, M)> c = a;
    return c ^= b;
};

template<size_t N>
constexpr auto operator^(const UInt<N>& a, const uint64_t& b) noexcept
{
    return a ^ UInt<64>(b);
};

template<size_t N>
constexpr auto operator^(const uint64_t& a, const UInt<N>& b) noexcept
{
    return UInt<64>(a) ^ b;
};

template<size_t N>
template<size_t M>
constexpr UInt<N>& UInt<N>::operator^=(const UInt<M>& a) noexcept
{
    for(size_t i = 0; i < ceil_div<32>(std::min(N, M)); ++i)
        data[i] ^= a.data[i];
    truncate();

    return *this;
}

template<size_t N>
constexpr UInt<N>& UInt<N>::operator^=(const uint64_t& a) noexcept
{
    data[0] ^= a;
    if(ceil_div<32>(N) > 1)
        data[1] ^= (a >> 32);
    truncate();

    return *this;
}


// <<, <<= operators
template<size_t N>
constexpr UInt<N> operator<<(const UInt<N>& a, uint64_t n) noexcept
{
    UInt<N> c = a;
    return c <<= n;
}

template<size_t N>
constexpr UInt<N>& UInt<N>::operator<<=(size_t n) noexcept
{
    size_t m = n / 32;
    size_t l = n % 32;

    for(size_t i = ceil_div<32>(N); i > 0; --i)
    {
        uint32_t temp = (i > m+1 ? data[i-m-2] >> (32-l) : 0);
        data[i-1] = ((i > m ? data[i-m-1] : 0) << l) + temp;
    }
    truncate();

    return *this;
}


// >>, >>= operators
template<size_t N>
constexpr UInt<N> operator>>(const UInt<N>& a, uint64_t n) noexcept
{
    UInt<N> c = a;
    return c >>= n;
}

template<size_t N>
constexpr UInt<N>& UInt<N>::operator>>=(size_t n) noexcept
{
    size_t m = n / 32;
    size_t l = n % 32;

    for(size_t i = 0; i < ceil_div<32>(N); ++i)
    {
        uint32_t temp = (i+m+1 < ceil_div<32>(N) ? data[i+m+1] << (32-l) : 0);
        data[i] = ((i+m > ceil_div<32>(N) ? data[i-m] : 0) >> l) + temp;
    }
    truncate();

    return *this;
}


// +, += operators
template<size_t N, size_t M>
constexpr auto operator+(const UInt<N>& a, const UInt<M>& b) noexcept
{
    UInt<std::max(N, M)> c = a;
    return c += b;
}

template<size_t N>
constexpr auto operator+(const UInt<N>& a, const uint64_t& b) noexcept
{
    return a + UInt<64>(b);
}

template<size_t N>
constexpr auto operator+(const uint64_t& a, const UInt<N>& b) noexcept
{
    return UInt<64>(a) + b;
}

template<size_t N>
template<size_t M>
constexpr UInt<N>& UInt<N>::operator+=(const UInt<M>& a) noexcept
{
    uint64_t carry = 0;
    for(size_t i = 0; i < ceil_div<32>(std::min(N, M)); ++i)
    {
        carry += data[i] + a.data[i];
        data[i] = carry;
        carry >>= 32;
    }
    truncate();

    return *this;
}

template<size_t N>
constexpr UInt<N>& UInt<N>::operator+=(uint64_t a) noexcept
{
    uint64_t carry = 0;
    for(size_t i = 0; i < std::min(ceil_div<32>(N), static_cast<size_t>(2)); ++i)
    {
        carry += data[i] + (a & 0xFFFFFFFF);
        data[i] = carry;
        carry >>= 32;
        a >>= 32;
    }
    truncate();

    return *this;
}


// -, -= operators
template<size_t N, size_t M>
constexpr auto operator-(const UInt<N>& a, const UInt<M>& b) noexcept
{
    UInt<std::max(N, M)> c = a;
    return c -= b;
}

template<size_t N>
constexpr auto operator-(const UInt<N>& a, const uint64_t& b) noexcept
{
    return a - UInt<64>(b);
}

template<size_t N>
constexpr auto operator-(const uint64_t& a, const UInt<N>& b) noexcept
{
    return UInt<64>(a) - b;
}

template<size_t N>
template<size_t M>
constexpr UInt<N>& UInt<N>::operator-=(const UInt<M>& a) noexcept
{
    uint64_t carry = 0;
    for(size_t i = 0; i < ceil_div<32>(std::min(N, M)); ++i)
    {
        carry = data[i] - a.data[i] - carry;
        data[i] = carry;
        carry >>= 31;
    }
    truncate();

    return *this;
}

template<size_t N>
constexpr UInt<N>& UInt<N>::operator-=(uint64_t a) noexcept
{
    uint64_t carry = 0;
    for(size_t i = 0; i < std::min(ceil_div<32>(N), static_cast<size_t>(2)); ++i)
    {
        carry = data[i] - (a & 0xFFFFFFFF) - carry;
        data[i] = carry;
        carry >>= 31;
        a >>= 32;
    }
    truncate();

    return *this;
}


// *, *= operators
template<size_t N, size_t M>
constexpr auto operator*(const UInt<N>& a, const UInt<M>& b) noexcept
{
    UInt<std::max(N, M)> total = 0;

    for(size_t i = 0; i < ceil_div<32>(N); ++i)
        for(size_t j = 0; j < ceil_div<32>(M); ++j)
            total += (static_cast<uint64_t>(a.data[i]) * static_cast<uint64_t>(b.data[j])) << (32*i + 32*j);

    return total;
}

template<size_t N>
constexpr auto operator*(const UInt<N>& a, uint64_t b) noexcept
{
    return a * UInt<64>(b);
}

template<size_t N>
constexpr auto operator*(uint64_t a, const UInt<N>& b) noexcept
{
    return UInt<64>(a) * b;
}

template<size_t N>
template<size_t M>
constexpr UInt<N>& UInt<N>::operator*=(const UInt<M>& a) noexcept
{
    *this = *this * a;
    truncate();

    return *this;
}

template<size_t N>
constexpr UInt<N>& UInt<N>::operator*=(uint64_t a) noexcept
{
    return *this *= UInt<64>(a);
}


template<size_t N>
std::ostream& operator<<(std::ostream& out, const UInt<N>& n) noexcept
{
    auto flags = out.flags();
    if(flags & std::ios::hex)
        n.write_hex(out);

    return out;

    out << n.data[ceil_div<32>(N)-1];
    out.unsetf(std::ios_base::showbase | std::ios_base::showpos);
    for(int i = ceil_div<32>(N)-2; i >= 0; --i)
        out << n.data[i];
    out.flags(flags);

    return out;
}

template<size_t N>
void UInt<N>::write_hex(std::ostream& out) const noexcept
{
    auto flags = out.flags();
    size_t i = ceil_div<32>(N);

    bool zero = true;
    while(zero && i-- > 0)
    {
        if(data[i] != 0)
            zero = false;
    }

    if(zero)
    {
        out << 0;
        return;
    }

    out << data[i];
    out.unsetf(std::ios_base::showbase | std::ios_base::showpos);

    while(i-- > 0)
    {
        out << std::setw(8) << std::setfill('0') << std::right << data[i];
    }

    out.flags(flags);
}


template<size_t N>
inline constexpr void UInt<N>::truncate() noexcept
{
    if constexpr(N % 32 != 0)
        data[ceil_div<32>(N) - 1] %= (1 << (N % 32));
}




#endif //UTILITIES_BIG_INT_HPP
