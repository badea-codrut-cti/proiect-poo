#include <array>
#include <cstdint>
#include <string>
#include <sstream>
#include <stdexcept>
#include "./ipv4.h"

// "Nimeni nu iti da tie o adresa statica de clasa A, nu exista dom student"
void IPv4Address::stringToOctets(const std::string& str) {
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

    octets = newOctets;
}

std::array<uint8_t, IPV4_SIZE> IPv4Address::getOctets() const {
    return octets;
}

IPv4Address::IPv4Address() : octets({127, 0, 0, 1}) {}

IPv4Address::IPv4Address(const IPv4Address& other) = default;

IPv4Address::IPv4Address(const std::array<uint8_t, IPV4_SIZE>& octets) : octets(octets) {}

IPv4Address::IPv4Address(const std::string& str): octets() {
    stringToOctets(str);
}

std::string IPv4Address::toString() const {
    std::ostringstream oss;
    for (size_t i = 0; i < octets.size(); ++i) {
        oss << static_cast<int>(octets[i]);
        if (i < 3) {
            oss << ".";
        }
    }

    return oss.str();
}

IPv4Address& IPv4Address::operator=(const std::string& str) {
    stringToOctets(str);
    return *this;
}

IPv4Address& IPv4Address::operator=(const IPv4Address&) = default;

bool IPv4Address::operator==(const IPv4Address& other) const {
    return octets == other.octets;
}

bool IPv4Address::operator==(const std::string& str) const {
    return str == toString();
}

bool IPv4Address::operator<(const IPv4Address& other) const {
    for (uint8_t i = 0; i < IPV4_SIZE - 1; i++)
        if (octets[i] > other.octets[i])
            return false;

    return octets[IPV4_SIZE - 1] < other.octets[IPV4_SIZE - 1];
}

bool IPv4Address::operator>=(const IPv4Address& other) const {
    return !(*this < other);
}

std::ostream& operator<<(std::ostream& os, const IPv4Address& ip) {
    os << ip.toString();
    return os;
}