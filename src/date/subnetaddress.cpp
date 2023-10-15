#include "./subnetaddress.h"
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>

// "Nu lasam packet tracer sa aleaga /24. Daca lasam programul sa faca alegeri pentru noi atunci gresim."
uint8_t SubnetAddress::getClassSubnetMask(const IPv4Address& add) {
    std::array<uint8_t, 4> octets = add.getOctets();
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

    return 4; 
}

SubnetAddress::SubnetAddress(const IPv4Address& ip, uint8_t subnetMask) : IPv4Address(ip) {
    if (subnetMask > 30) 
        throw std::invalid_argument("Subnet mask cannot be greater than 30");

    this->subnetMask = subnetMask;
}

SubnetAddress::SubnetAddress(const SubnetAddress& add): IPv4Address(add.getOctets()) {
    subnetMask = add.subnetMask;
}

SubnetAddress::SubnetAddress() : IPv4Address() {
    subnetMask = 30;
}

SubnetAddress::SubnetAddress(const IPv4Address& ip): IPv4Address(ip) {
    subnetMask = getClassSubnetMask(ip);
}

IPv4Address SubnetAddress::getNetworkAddress() const {
    uint32_t mask = (0xFFFFFFFF << (32 - subnetMask)) & 0xFFFFFFFF;

    std::array<uint8_t, 4> networkAddress{};
    for (int i = 0; i < 4; i++) 
        networkAddress[i] = this->octets[i] & (mask >> (24 - i * 8));
            
    return {networkAddress};
}

IPv4Address SubnetAddress::getBroadcastAddress() const {
    uint32_t mask = (0xFFFFFFFF << (32 - subnetMask)) & 0xFFFFFFFF;

    std::array<uint8_t, 4> broadcastAddress{};
    for (int i = 0; i < 4; i++) 
        broadcastAddress[i] = this->octets[i] | (~mask >> (24 - i * 8));
            
    return {broadcastAddress};
}

bool SubnetAddress::isMulticastAddress() const {
    return (octets[0] & 0xF0) == 0xE0;
}

bool SubnetAddress::isInSameSubnet(SubnetAddress other) const {
    if (subnetMask != other.subnetMask)
        return false;

    uint8_t subnet = subnetMask, i = 0;

    while(subnet) {
        uint8_t minBits = subnet > 8 ? 8 : subnet;
        uint8_t mask = 0xFF << (8 - minBits);
        if ((octets[i] & mask) != (other.octets[i] & mask))
            return false;
        i++;
        subnet -= minBits;
    }
    
    return true;
}

std::string SubnetAddress::toString() const {
    return IPv4Address::toString() + "/" + std::to_string(subnetMask);
}

std::ostream& operator<<(std::ostream& os, const SubnetAddress& add) {
    os << add.toString();
    return os;
}