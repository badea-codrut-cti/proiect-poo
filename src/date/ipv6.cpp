#include "ipv6.h"
#include <sstream>

std::array<uint8_t, IPV6_SIZE> IPv6Address::stringToOctets(const std::string& str) {
    std::istringstream iss(str);
    std::string token;
    std::array<uint8_t, IPV6_SIZE> newOctets = {0};
    int count = 0;
    int doubleColonIndex = -1;

    while (getline(iss, token, ':')) {
        if (token.empty()) {
            if (doubleColonIndex != -1) {
                throw std::invalid_argument("Invalid IP address format. More than one double colon.");
            }
            doubleColonIndex = count;
            continue;
        }

        if (count >= IPV6_SIZE) {
            throw std::invalid_argument("Invalid IP address format. More than 16 octets.");
        }
        
        try {
            int value = std::stoi(token, nullptr, 16);
            if (value > 0xFFFF) {
                throw std::invalid_argument("Invalid IP address format. Octet value more than 0xFFFF.");
            }
            newOctets[count++] = value >> 8; 
            newOctets[count++] = value & 0xFF;
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Invalid IP address format.");
        }
    }

    if (doubleColonIndex != -1) {
        int shift = IPV6_SIZE - count;
        for (int i = count - 1; i >= doubleColonIndex; --i) {
            newOctets[i + shift] = newOctets[i];
            newOctets[i] = 0;
        }
        count = IPV6_SIZE;
    }

    if (count < IPV6_SIZE) {
        throw std::invalid_argument("Invalid IP address format. Less than 16 octets.");
    }

    return newOctets;
}

std::string IPv6Address::toString() const {
    std::ostringstream oss;
    size_t maxZerosStart = 0, maxZerosLen = 0;
    size_t curZerosStart = 0, curZerosLen = 0;
    for (size_t i = 0; i < IPV6_SIZE; i += 2) {
        uint16_t part = (octets[i] << 8) | octets[i+1];
        if (part == 0) {
            if (curZerosLen == 0) {
                curZerosStart = i;
            }
            curZerosLen += 2;
            if (curZerosLen > maxZerosLen) {
                maxZerosStart = curZerosStart;
                maxZerosLen = curZerosLen;
            }
        } else {
            curZerosLen = 0;
        }
    }

    for (size_t i = 0; i < IPV6_SIZE; i += 2) {
        if (i == maxZerosStart && maxZerosLen > 0) {
            oss << "::";
            i += maxZerosLen - 2;
            continue;
        }
        uint16_t part = (octets[i] << 8) | octets[i+1];
        oss << std::hex << part;
        if (i < IPV6_SIZE-2 && !(i == maxZerosStart - 2 && maxZerosLen > 0)) {
            oss << ":";
        }
    }

    std::string result = oss.str();
    if (result.back() == ':') {
        result.pop_back();
    }

    return result;
}

IPv6Address::IPv6Address() : 
Address<IPV6_SIZE>(std::array<uint8_t, IPV6_SIZE>({0xFE, 0x80 })) 
{}

IPv6Address::IPv6Address(const IPv6Address& other):
Address<IPV6_SIZE>(other.octets) {}

IPv6Address::IPv6Address(const std::array<uint8_t, IPV6_SIZE>& octets) :
Address<IPV6_SIZE>(octets) {}

IPv6Address::IPv6Address(const std::string& str): 
Address<IPV6_SIZE>(stringToOctets(str)) {}

std::ostream& operator<<(std::ostream& os, const IPv6Address& addr) {
    os << addr.toString();
    return os;
}

std::string SubnetAddressV6::toString() const {
    std::ostringstream sstr;
    sstr << IPv6Address::toString();
    sstr << "/" << subnetMask;

    return sstr.str();
}

std::ostream& operator<<(std::ostream& os, const SubnetAddressV6& addr) {
    os << addr.toString();
    return os;
}