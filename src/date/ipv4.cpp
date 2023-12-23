#include <array>
#include <cstdint>
#include <string>
#include <sstream>
#include <stdexcept>
#include "./ipv4.h"
#include "subnetaddress.h"

// "Nimeni nu iti da tie o adresa statica de clasa A, nu exista dom student"
std::array<uint8_t, IPV4_SIZE> IPv4Address::stringToOctets(const std::string& str) {
    std::istringstream iss(str);
    std::string token;
    std::array<uint8_t, IPV4_SIZE> newOctets = {0};
    int count = 0;

    while (getline(iss, token, '.')) {
        count++;
        if (count > IPV4_SIZE) {
            throw std::invalid_argument("Invalid IP address format. More than 4 octets.");
        }
        try {
            int value = std::stoi(token);
            if (value > 255) {
                throw std::invalid_argument("Invalid IP address format. Octet value more than 255.");
            }
            newOctets[count-1] = value;
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Invalid IP address format.");
        }
    }

    if (count < IPV4_SIZE) {
        throw std::invalid_argument("Invalid IP address format. Less than 4 octets.");
    }

    return newOctets;
}

uint8_t SubnetAddressV4::getClassSubnetMask(const IPv4Address& address) {
    std::array<uint8_t, IPV4_SIZE> octets = address.getOctets();
    if (!(octets[0] & 0x80)) {
        return 8;
    }

    if ((octets[0] & 0xC0) == 0x80) {
        return 16;
    }

    if ((octets[0] & 0xE0) == 0xC0) {
        return 24;
    }

    if ((octets[0] & 0xF0) == 0xE0) {
        return octets[0] == 224 ? 24 : 8;
    }

    throw std::invalid_argument("Invalid IP address for subnet mask calculation");
}

uint8_t SubnetAddressV4::dotMaskToCIDR(const IPv4Address& addr) {
    uint8_t cidr = 0;
    for(uint8_t octet : addr.getOctets()) {
        uint8_t block = 0x80;
        while (octet & block) {
            octet = octet & (~block);
            block >>= 1;
            cidr++;
        }
        if (octet)
            throw std::invalid_argument("Invalid subnet mask");
    }
    return cidr;
}

IPv4Address::IPv4Address() : 
Address<IPV4_SIZE>(
    std::array<uint8_t, IPV4_SIZE>({127, 0, 0, 1})
) {}

IPv4Address::IPv4Address(const IPv4Address& other) : Address<IPV4_SIZE>(other) {}

IPv4Address::IPv4Address(const std::array<uint8_t, IPV4_SIZE>& octets) : 
Address<IPV4_SIZE>(octets) {}

IPv4Address::IPv4Address(const std::string& str): 
Address<IPV4_SIZE>(stringToOctets(str)) {

}

IPv4Address& IPv4Address::operator=(const IPv4Address& other) {
    octets = other.octets;
    return *this;
}

std::string IPv4Address::toString() const {
    std::ostringstream oss;
    for (size_t i = 0; i < IPV4_SIZE; ++i) {
        oss << static_cast<int>(octets[i]);
        if (i < IPV4_SIZE-1) {
            oss << ".";
        } 
    }

    return oss.str();
}

SubnetAddressV4::SubnetAddressV4(): 
SubnetAddress<IPV4_SIZE, IPv4Address>(IPv4Address(), 8) {

}

SubnetAddressV4::SubnetAddressV4(const IPv4Address& addr):
SubnetAddress<IPV4_SIZE, IPv4Address>(addr, getClassSubnetMask(addr)) {

}

SubnetAddressV4::SubnetAddressV4(const IPv4Address& addr, uint8_t nr):
SubnetAddress<IPV4_SIZE, IPv4Address>(addr, nr) {

}

IPv4Address SubnetAddressV4::getMaskDotNotation() const {
    std::array<uint8_t, IPV4_SIZE> dotNotation{};
    size_t remainingBits = subnetMask, i = 0;
    while (i < IPV4_SIZE && remainingBits) {
        if (remainingBits >= 8) {
            dotNotation[i++] = 255;
            remainingBits -= 8;
            continue;
        } 

        dotNotation[i++] = 0xFFU << (8 - remainingBits);
        remainingBits = 0;
    }

    while (i < IPV4_SIZE) 
        dotNotation[i++] = 0;

    return IPv4Address{dotNotation};
}

std::string SubnetAddressV4::toString() const {
    std::ostringstream str;
    str << IPv4Address::toString();
    str << "/" << subnetMask;
    return str.str();
}

bool SubnetAddressV4::isLoopbackAddress() const {
    return octets[0] == 127;
}

bool SubnetAddressV4::isMulticastAddress() const {
    return (octets[0] >> 4) == 0xE;
}

std::ostream& operator<<(std::ostream& os, const IPv4Address& addr) {
    os << addr.toString();
    return os;
}

std::ostream& operator<<(std::ostream& os, const SubnetAddressV4& addr) {
    os << addr.toString();
    return os;
}