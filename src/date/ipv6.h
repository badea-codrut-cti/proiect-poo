#include <array>
#include <string>
#include "macaddress.h"
#include "subnetaddress.h"

#pragma once

#ifndef IPV6_H
#define IPV6_H

const uint8_t IPV6_SIZE = 16;

class IPv6Address : public Address<IPV6_SIZE> {
    protected:
        static std::array<uint8_t, IPV6_SIZE> stringToOctets(const std::string&);

    public:
        IPv6Address();
        IPv6Address(const IPv6Address&);
        explicit IPv6Address(const std::array<uint8_t, IPV6_SIZE>&);
        explicit IPv6Address(const std::string&);
        explicit IPv6Address(const MACAddress&);

        [[nodiscard]] bool isLinkLocalAddress() const;

        [[nodiscard]] bool isMulticastAddress() const;

        [[nodiscard]] bool isLoopbackAddress() const;

        IPv6Address& operator=(const IPv6Address&);

        [[nodiscard]] std::string toString() const;
};

class SubnetAddressV6 : public SubnetAddress<IPV6_SIZE, IPv6Address> {
    public:
        SubnetAddressV6();
        explicit SubnetAddressV6(const IPv6Address&);
        SubnetAddressV6(const IPv6Address&, uint8_t);

        [[nodiscard]] std::string toString() const;
};

std::ostream& operator<<(std::ostream&, const IPv6Address&);
std::ostream& operator<<(std::ostream&, const SubnetAddressV6&);
#endif