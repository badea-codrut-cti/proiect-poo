#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <array>
#include <stdexcept>
#include "./macaddress.h"

void MACAddress::stringToOctets(const std::string& str) {
    std::istringstream iss(str);
	std::string token;
	std::array<uint8_t, 6> newOctets = {0};

	for (size_t i = 0; i < 6; ++i) {
		if (!getline(iss, token, ':')) {
			throw std::invalid_argument("Format de adresa MAC invalid.");
		}
			
        try {
			newOctets[i] = std::stoi(token, nullptr, 16);
		} catch (...) {
			throw std::invalid_argument("Format de adresa MAC invalid.");
		}
    }

	octets = newOctets;
}

MACAddress::MACAddress(): octets(broadcastAddress) {}

MACAddress::MACAddress(const MACAddress& other) : octets(other.octets) {}

MACAddress::MACAddress(const std::array<uint8_t, 6>& octets) : octets(octets) {}

MACAddress::MACAddress(const std::string& str) {
    stringToOctets(str);
}

std::string MACAddress::toString() const {
    std::ostringstream oss;
    for (size_t i = 0; i < octets.size(); ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(octets[i]);
        if (i < 5) {
            oss << ":";
        }
    }
    oss << std::dec;
    return oss.str();
}

bool MACAddress::isMulticast() const {
    return (octets[0] & 0x01) == 1;
}

MACAddress& MACAddress::operator+=(int increment) {
    for (int i = octets.size() - 1; i >= 0; --i) {
        int sum = octets[i] + (increment & 0xFF);
                
        if (sum > 0xFF) {
            if (i == 0) 
                throw std::overflow_error("MAC address overflow.");
                    
            octets[i] = sum & 0xFF;
            increment = (increment >> 8) + 1;
        } else {
            octets[i] = sum;
            increment >>= 8;
        }

        if (increment == 0) {
            break;
        }
    }

    return *this;
}

MACAddress& MACAddress::operator-=(int decrement) {
    *this += -decrement;
    return *this;
}

MACAddress MACAddress::operator+(int increment) const {
    MACAddress result(*this);
    result += increment;
    return result;
}

MACAddress MACAddress::operator-(int decrement) const {
    MACAddress result(*this);
    result += -decrement;
    return result;
}

MACAddress& MACAddress::operator=(const std::string& str) {
	stringToOctets(str);
	return *this;
}

MACAddress& MACAddress::operator=(const std::array<uint8_t, 6>& _octets) {
	octets = _octets;
	return *this;
}


bool MACAddress::operator==(const std::string& str) const {
	MACAddress other;
	try {
		other = str;
	} catch (...) {
		return false; 
	}
	return octets == other.octets;
}

bool MACAddress::operator<(const MACAddress& other) const {
    for (uint8_t i=0; i<5; i++)
        if (octets[i] < other.octets[i])
            return true;
    return octets[5] >= other.octets[5];
}

bool MACAddress::operator==(const MACAddress& other) const {
	return octets == other.octets;
}


std::ostream& operator<<(std::ostream& os, const MACAddress& mac) {
	os << mac.toString();
	return os;
}

const std::array<uint8_t, 6> MACAddress::broadcastAddress = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};