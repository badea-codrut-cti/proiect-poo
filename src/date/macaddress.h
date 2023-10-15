#include <string>
#include <array>

#ifndef MACADDRESS_H
#define MACADDRESS_H

class MACAddress {
    private:
        std::array<uint8_t, 6> octets; 

        void stringToOctets(const std::string& str);

    public:
        static const std::array<uint8_t, 6> broadcastAddress;

        MACAddress();

        MACAddress(const MACAddress&);

	    MACAddress(const std::array<uint8_t, 6>&);

        MACAddress(const std::string&);

        std::string toString() const;

        bool isMulticast() const;

        MACAddress& operator+=(int);

        MACAddress& operator-=(int);

        MACAddress operator+(int) const;

        MACAddress operator-(int) const;

	    MACAddress& operator=(const std::string&);

        MACAddress& operator=(const std::array<uint8_t, 6>&);

        bool operator<(const MACAddress&) const;

	    bool operator==(const MACAddress&) const;

	    bool operator==(const std::string&) const;

	    friend std::ostream& operator<<(std::ostream&, const MACAddress&);
};

#endif
