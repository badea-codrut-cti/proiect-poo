#include <string>
#include <array>

#pragma once

#ifndef MACADDRESS_H
#define MACADDRESS_H

#include "address.h"

const uint8_t MACADDRESS_SIZE = 6;

class MACAddress : public Address<MACADDRESS_SIZE> {
    private:
        static std::array<uint8_t, MACADDRESS_SIZE> stringToOctets(const std::string& str);

    public:
        static const std::array<uint8_t, MACADDRESS_SIZE> broadcastAddress;

        MACAddress();

        MACAddress(const MACAddress&);

	    explicit MACAddress(const std::array<uint8_t, MACADDRESS_SIZE>&);

        explicit MACAddress(const std::string&);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] bool isMulticast() const;
};

std::ostream& operator<<(std::ostream&, const MACAddress&);
#endif

