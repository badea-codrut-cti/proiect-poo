#include "address.h"

#pragma once

#ifndef SUBNETADDR_H
#define SUBNETADDR_H

template <class T, class B>
concept CheckType = std::is_base_of_v<B, T>;

template<size_t N, typename T>
requires CheckType<T, Address<N>>
class SubnetAddress : public T {
    protected:
        size_t subnetMask;

    public:
        SubnetAddress(const T&, size_t);

        SubnetAddress(const SubnetAddress<N, T>&);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] T getNetworkAddress() const;

        [[nodiscard]] T getBroadcastAddress() const;

        [[nodiscard]] bool isInSameSubnet(const SubnetAddress<N, T>&) const;

        [[nodiscard]] bool isInSameSubnet(const T&) const;

        [[nodiscard]] size_t getSubnetMask() const;

        SubnetAddress<N, T>& operator=(const SubnetAddress<N, T>&);
};

template<size_t N, typename T>
requires CheckType<T, Address<N>>
std::ostream& operator<<(std::ostream&, const SubnetAddress<N, T>&);

template <size_t N, class T>
requires CheckType<T, Address<N>>
SubnetAddress<N, T>::SubnetAddress(const T& ip, size_t sMask) : 
T(ip), subnetMask(sMask) {
    if (subnetMask > N * 8 - 2) 
        throw std::invalid_argument("Invalid subnet mask.");
}

template <size_t N, class T>
requires CheckType<T, Address<N>>
SubnetAddress<N, T>::SubnetAddress(const SubnetAddress<N, T>& add): 
T(add), subnetMask(add.subnetMask) {

}

template <size_t N, class T>
requires CheckType<T, Address<N>>
T SubnetAddress<N, T>::getNetworkAddress() const {
    std::array<uint8_t, N> networkAddress{};
    size_t maskLen = subnetMask, i = 0;

    while (i < N && maskLen) {
        if (maskLen >= 8) {
            networkAddress[i] = this->octets[i];
            i++;
            maskLen -= 8;
            continue;
        }
        
        networkAddress[i] = this->octets[i] & (0xFF << (8 - maskLen));
        i++;
        maskLen = 0;
    }

    while (i < N) 
        networkAddress[i++] = 0;
      
    return T(networkAddress);
}

template <size_t N, class T>
requires CheckType<T, Address<N>>
T SubnetAddress<N, T>::getBroadcastAddress() const {
    std::array<uint8_t, N> broadcastAddress{};
    size_t maskLen = subnetMask, i = 0;

    while (i < N && maskLen) {
        if (maskLen >= 8) {
            broadcastAddress[i] = this->octets[i];
            maskLen -= 8;
            i++;
            continue;
        }
        
        broadcastAddress[i] = this->octets[i] | ~(0xFF << (8 - maskLen));
        i++;
        maskLen = 0;
    }

    while (i < N) 
        broadcastAddress[i++] = 0xFF;
       
    return T(broadcastAddress);
}

template <size_t N, class T>
requires CheckType<T, Address<N>>
bool SubnetAddress<N, T>::isInSameSubnet(const SubnetAddress<N, T>& other) const {
    if (subnetMask != other.subnetMask)
        return false;

    return getNetworkAddress() == other.getNetworkAddress();
}

template <size_t N, class T>
requires CheckType<T, Address<N>>
bool SubnetAddress<N, T>::isInSameSubnet(const T& other) const {
    return isInSameSubnet(SubnetAddress<N, T>(other, subnetMask));
}

template <size_t N, class T>
requires CheckType<T, Address<N>>
SubnetAddress<N, T>& SubnetAddress<N, T>::operator=(const SubnetAddress<N, T>& other) {
    this->octets = other.octets;
    subnetMask = other.subnetMask;
    return *this;
}

template <size_t N, class T>
requires CheckType<T, Address<N>>
size_t SubnetAddress<N, T>::getSubnetMask() const {
    return subnetMask;
}

template <size_t N, class T>
requires CheckType<T, Address<N>>
std::string SubnetAddress<N, T>::toString() const {
    return "";
}

template <size_t N, class T>
requires CheckType<T, Address<N>>
std::ostream& operator<<(std::ostream& os, const SubnetAddress<N, T>& add) {
    os << add.toString();
    return os;
}
#endif
