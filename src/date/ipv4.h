#include <array>
#include <string>

#ifndef IPV4_H
#define IPV4_H

const uint8_t IPV4_SIZE = 4;

class IPv4Address {
    protected:
        std::array<uint8_t, IPV4_SIZE> octets;
        void stringToOctets(const std::string&);

    public:
        IPv4Address();
        IPv4Address(const IPv4Address&);
        IPv4Address(const std::array<uint8_t, IPV4_SIZE>&);
        IPv4Address(const std::string&);

        [[nodiscard]] std::string toString() const;
        [[nodiscard]] std::array<uint8_t, IPV4_SIZE> getOctets() const;

        IPv4Address& operator=(const std::string&);
        bool operator==(const IPv4Address&) const;
        bool operator==(const std::string&) const;
        bool operator<(const IPv4Address&) const;
        bool operator>=(const IPv4Address&) const;
        
        friend std::ostream& operator<<(std::ostream&, const IPv4Address&);
};

#endif