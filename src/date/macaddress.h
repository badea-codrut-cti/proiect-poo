#include <string>
#include <array>

#ifndef MACADDRESS_H
#define MACADDRESS_H

const uint8_t MACADDRESS_SIZE = 6;

class MACAddress {
    private:
        std::array<uint8_t, MACADDRESS_SIZE> octets; 

        void stringToOctets(const std::string& str);

    public:
        static const std::array<uint8_t, MACADDRESS_SIZE> broadcastAddress;

        MACAddress();

        MACAddress(const MACAddress&);

	    explicit MACAddress(const std::array<uint8_t, MACADDRESS_SIZE>&);

        explicit MACAddress(const std::string&);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] bool isMulticast() const;

        [[nodiscard]] const std::array<uint8_t, MACADDRESS_SIZE>& getOctets() const;

        MACAddress& operator+=(long);

        MACAddress& operator-=(long);

        MACAddress operator+(long) const;

        MACAddress operator-(long) const;

	    MACAddress& operator=(const std::string&);

        MACAddress& operator=(const std::array<uint8_t, MACADDRESS_SIZE>&);

        MACAddress& operator=(const MACAddress&);

        bool operator<(const MACAddress&) const;

	    bool operator==(const MACAddress&) const;

	    bool operator==(const std::string&) const;

	    friend std::ostream& operator<<(std::ostream&, const MACAddress&);
};

#endif

