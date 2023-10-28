#include <iomanip>
#include <sstream>
#include <string>
#include <array>
#include <stdexcept>
#include "./macaddress.h"

void MACAddress::stringToOctets(const std::string& str) {
    std::istringstream iss(str);
	std::string token;
	std::array<uint8_t, MACADDRESS_SIZE> newOctets{};

	for (size_t i = 0; i < MACADDRESS_SIZE; ++i) {
		if (!getline(iss, token, ':')) {
			throw std::invalid_argument("Invalid MAC address format.");
		}
			
        try {
			newOctets[i] = std::stoi(token, nullptr, 16);
		} catch ([[maybe_unused]] const std::invalid_argument& e) {
			throw std::invalid_argument("Invalid MAC address format.");
		}
    }

	octets = newOctets;
}

MACAddress::MACAddress(): octets(broadcastAddress) {}

MACAddress::MACAddress(const MACAddress& other) = default;

MACAddress::MACAddress(const std::array<uint8_t, MACADDRESS_SIZE>& octets) : octets(octets) {}

MACAddress::MACAddress(const std::string& str): octets() {
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

MACAddress& MACAddress::operator+=(long increment) {
    bool isNegative = increment < 0;
    if (isNegative)
        increment *= -1;
    for (int i = MACADDRESS_SIZE - 1; i >= 0; --i) {
        long sum = octets[i] + (isNegative ? -1 : 1) * (increment & 0xFF);
        if (sum > 0xFF) {
            if (i == 0) 
                throw std::overflow_error("MAC address overflow.");
                    
            octets[i] = sum & 0xFF;
            increment = (increment >> 8) + 1;
        } else if (sum < 0) {
            if (i == 0 || !octets[i-1]) 
                throw std::underflow_error("MAC address underflow.");

            octets[i] = 0xFF + sum + 1;
            octets[i-1] -= 1;
            increment = increment >> 8;
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

MACAddress& MACAddress::operator-=(long decrement) {
    *this += -decrement;
    return *this;
}

MACAddress MACAddress::operator+(long increment) const {
    MACAddress result(*this);
    result += increment;
    return result;
}

MACAddress MACAddress::operator-(long decrement) const {
    MACAddress result(*this);
    result += -decrement;
    return result;
}

MACAddress& MACAddress::operator=(const std::string& str) {
	stringToOctets(str);
	return *this;
}

MACAddress& MACAddress::operator=(const std::array<uint8_t, MACADDRESS_SIZE>& _octets) {
	octets = _octets;
	return *this;
}

MACAddress& MACAddress::operator=(const MACAddress& other) = default;

bool MACAddress::operator==(const std::string& str) const {
	MACAddress other(str);
	return octets == other.octets;
}

bool MACAddress::operator<(const MACAddress& other) const {
    for (uint8_t i=0; i<MACADDRESS_SIZE-1; i++)
        if (octets[i] > other.octets[i])
            return false;
    return octets[MACADDRESS_SIZE-1] < other.octets[MACADDRESS_SIZE];
}

bool MACAddress::operator==(const MACAddress& other) const {
	return octets == other.octets;
}

bool MACAddress::operator!=(const MACAddress& other) const {
    return octets != other.octets;
}

const std::array<uint8_t, MACADDRESS_SIZE>& MACAddress::getOctets() const {
    return octets;
}


std::ostream& operator<<(std::ostream& os, const MACAddress& mac) {
	os << mac.toString();
	return os;
}

const std::array<uint8_t, MACADDRESS_SIZE> MACAddress::broadcastAddress = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};