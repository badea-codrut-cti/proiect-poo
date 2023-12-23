#include <array>
#include <string>

#pragma once

#ifndef IPV4_H
#define IPV4_H

#include "subnetaddress.h"

const uint8_t IPV4_SIZE = 4;

class IPv4Address : public Address<IPV4_SIZE> {
    protected:
        static std::array<uint8_t, IPV4_SIZE> stringToOctets(const std::string&);

    public:
        IPv4Address();
        IPv4Address(const IPv4Address&);
        explicit IPv4Address(const std::array<uint8_t, IPV4_SIZE>&);
        explicit IPv4Address(const std::string&);

        IPv4Address& operator=(const IPv4Address&);

        [[nodiscard]] std::string toString() const;
};

class SubnetAddressV4 : public SubnetAddress<IPV4_SIZE, IPv4Address> {
    public:
        static uint8_t getClassSubnetMask(const IPv4Address&);
        static uint8_t dotMaskToCIDR(const IPv4Address&);

        SubnetAddressV4();
        explicit SubnetAddressV4(const IPv4Address&);
        SubnetAddressV4(const IPv4Address&, uint8_t);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] bool isLoopbackAddress() const;
        [[nodiscard]] bool isMulticastAddress() const;

        [[nodiscard]] IPv4Address getMaskDotNotation() const;
};

std::ostream& operator<<(std::ostream&, const IPv4Address&);
std::ostream& operator<<(std::ostream&, const SubnetAddressV4&);
#endif