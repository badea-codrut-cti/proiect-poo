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

    for (size_t i = 0; i < IPV4_SIZE; ++i) {
        if (!getline(iss, token, '.')) {
            throw std::invalid_argument("Invalid IP address format.");
        }
        try {
            newOctets[i] = std::stoi(token);
        } catch ([[maybe_unused]] const std::invalid_argument& e) {
            throw std::invalid_argument("Invalid IP address format.");
        }
    }

    octets = newOctets;
}

std::array<uint8_t, IPV4_SIZE> IPv4Address::getOctets() const {
    return octets;
}

IPv4Address::IPv4Address() : octets({127, 0, 0, 1}) {}

IPv4Address::IPv4Address(const IPv4Address& other) = default;

IPv4Address::IPv4Address(const std::array<uint8_t, 4>& octets) : octets(octets) {}

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