#include <array>
#include <string>
#include <sstream>
#include <stdexcept>
#include "./ipv4.h"

// "Nimeni nu iti da tie o adresa statica de clasa A, nu exista dom student"
void IPv4Address::stringToOctets(const std::string& str) {
    std::istringstream iss(str);
    std::string token;
    std::array<uint8_t, 4> newOctets = {0};

    for (size_t i = 0; i < 4; ++i) {
        if (!getline(iss, token, '.')) {
            throw std::invalid_argument("Format de adresa IP invalid.");
        }
        try {
            newOctets[i] = std::stoi(token);
        } catch (...) {
            throw std::invalid_argument("Format de adresa IP invalid.");
        }
    }

    octets = newOctets;
}

std::array<uint8_t, 4> IPv4Address::getOctets() const {
    return octets;
}

IPv4Address::IPv4Address() : octets({127, 0, 0, 1}) {}

IPv4Address::IPv4Address(const IPv4Address& other) : octets(other.octets) {}

IPv4Address::IPv4Address(const std::array<uint8_t, 4>& octets) : octets(octets) {}

IPv4Address::IPv4Address(const std::string& str) {
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

bool IPv4Address::operator==(const IPv4Address& other) const {
    return octets == other.octets;
}

bool IPv4Address::operator==(const std::string& str) const {
    IPv4Address other;
    try {
        other = str;
    } catch (...) {
        return false; 
    }
    return octets == other.octets;
}

std::ostream& operator<<(std::ostream& os, const IPv4Address& ip) {
    os << ip.toString();
    return os;
}