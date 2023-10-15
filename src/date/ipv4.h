#include <array>
#include <string>

#ifndef IPV4_H
#define IPV4_H

class IPv4Address {
    protected:
        std::array<uint8_t, 4> octets;
        void stringToOctets(const std::string&);

    public:
        IPv4Address();
        IPv4Address(const IPv4Address&);
        IPv4Address(const std::array<uint8_t, 4>&);
        IPv4Address(const std::string&);

        std::string toString() const;
        std::array<uint8_t, 4> getOctets() const;

        IPv4Address& operator=(const std::string&);
        bool operator==(const IPv4Address&) const;
        bool operator==(const std::string&) const;

        friend std::ostream& operator<<(std::ostream&, const IPv4Address&);
};

#endif