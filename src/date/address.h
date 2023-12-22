#include <array>
#include <stdexcept>

#pragma once

#ifndef IP_H
#define IP_H

template <size_t N>
class Address {
    protected:
        std::array<uint8_t, N> octets;

    public:
        Address(const Address<N>&);
        explicit Address(const std::array<uint8_t, N>&);

        [[nodiscard]] std::array<uint8_t, N> getOctets() const;

        Address<N>& operator=(const Address<N>&);
        bool operator==(const Address<N>&) const;
        bool operator<(const Address<N>&) const;
        bool operator>(const Address<N>&) const;
        bool operator>=(const Address<N>&) const;
        bool operator<=(const Address<N>&) const;
        bool operator!=(const Address<N>&) const;

        Address<N>& operator+=(int64_t);

        Address<N>& operator-=(int64_t);

        Address<N> operator+(int64_t) const;

        Address<N> operator-(int64_t) const;
};

template <size_t N>
Address<N>::Address(const Address<N>& other): octets(other.octets) {


}

template <size_t N>
Address<N>::Address(const std::array<uint8_t, N>& other): octets(other) {

}


template <size_t N>
std::array<uint8_t, N> Address<N>::getOctets() const {
    return octets;
}

template <size_t N>
Address<N>& Address<N>::operator=(const Address<N>& other) {
    octets = other.octets;
    return *this;
}

template <size_t N>
bool Address<N>::operator==(const Address<N>& other) const {
    return octets == other.octets;
}


template <size_t N>
bool Address<N>::operator<(const Address<N>& other) const {
    for (size_t i = 0; i < N - 1; i++)
        if (octets[i] > other.octets[i])
            return false;
    
    return octets[N - 1] < other.octets[N - 1];
}

template <size_t N>
bool Address<N>::operator>(const Address<N>& other) const {
    for (size_t i = 0; i < N - 1; i++)
        if (octets[i] < other.octets[i])
            return false;
    
    return octets[N - 1] > other.octets[N - 1];
}

template <size_t N>
bool Address<N>::operator>=(const Address<N>& other) const {
    return !(*this < other);
}

template <size_t N>
bool Address<N>::operator<=(const Address<N>& other) const {
    return !(*this > other);
}

template <size_t N>
bool Address<N>::operator!=(const Address<N>& other) const {
    return !(*this == other);
}

//TODO: Fix this garbage
template <size_t N>
Address<N>& Address<N>::operator+=(int64_t value) {
    bool isNegative = value < 0;

    if (isNegative) 
        value = -value;

    for (int i = N - 1; i >= 0; i--) {
        if (value == 0)
            break;

        uint8_t val = value & 0xFF;

        if (isNegative) {
            if (val > octets[i]) {
                if (i == 0) 
                    throw std::underflow_error("Address underflow");

                octets[i] += 0xFF - val + 1;
                value >>= 8;
                if (i > 0)
                    octets[i-1]--;
            } else {
                octets[i] -= val;
                value >>= 8;
            }

        } else {
            if (val > 0xFF - octets[i]) {
                if (i == 0)
                    throw std::overflow_error("Address overflow");

                octets[i] += val - 0xFF;
                value = (value >> 8) + 1;
            } else {
                octets[i] += val;
                value >>= 8;
            }
        }
    }

    return *this;
}

template <size_t N>
Address<N>& Address<N>::operator-=(int64_t value) {
    *this += -value;

    return *this;
}

template <size_t N>
Address<N> Address<N>::operator+(int64_t value) const {
    Address<N> copy(*this);

    copy += value;

    return copy;
}

template <size_t N>
Address<N> Address<N>::operator-(int64_t value) const {
    Address<N> copy(*this);

    copy += -value;

    return copy;
}
#endif