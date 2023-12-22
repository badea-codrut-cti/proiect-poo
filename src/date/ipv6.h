#include <array>
#include <string>
#include "macaddress.h"

#ifndef IPV6_H
#define IPV6_H

const uint8_t IPV6_SIZE = 16;

class IPv6Address {
    protected:
        std::array<uint8_t, IPV6_SIZE> octets;
        void stringToOctets(const std::string&);

    public:
        IPv6Address();
        IPv6Address(const IPv6Address&);
        explicit IPv6Address(const std::array<uint8_t, IPV6_SIZE>&);
        explicit IPv6Address(const std::string&);
        explicit IPv6Address(const MACAddress&);

        [[nodiscard]] std::string toString() const;
        [[nodiscard]] std::array<uint8_t, IPV6_SIZE> getOctets() const;

        IPv6Address& operator=(const std::string&);
        IPv6Address& operator=(const IPv6Address&);
        bool operator==(const IPv6Address&) const;
        bool operator==(const std::string&) const;
        bool operator<(const IPv6Address&) const;
        bool operator>=(const IPv6Address&) const;
        
        friend std::ostream& operator<<(std::ostream&, const IPv6Address&);
};

#endif