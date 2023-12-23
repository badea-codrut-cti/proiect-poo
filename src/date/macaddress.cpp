#include <iomanip>
#include <sstream>
#include <string>
#include <array>
#include <stdexcept>
#include "./macaddress.h"

std::array<uint8_t, MACADDRESS_SIZE> MACAddress::stringToOctets(const std::string& str) {
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

	return newOctets;
}

MACAddress::MACAddress(): Address<MACADDRESS_SIZE>(broadcastAddress) {}

MACAddress::MACAddress(const MACAddress& other) : Address<MACADDRESS_SIZE>(other) {}

MACAddress::MACAddress(const std::array<uint8_t, MACADDRESS_SIZE>& octets) : 
Address<MACADDRESS_SIZE>(octets) {}

MACAddress::MACAddress(const std::string& str): 
Address<MACADDRESS_SIZE>(stringToOctets(str)) {

}

std::string MACAddress::toString() const {
    std::ostringstream oss;
    for (size_t i = 0; i < octets.size(); ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(octets[i]);
        if (i < MACADDRESS_SIZE-1) {
            oss << ":";
        }
    }
    oss << std::dec;
    return oss.str();
}

bool MACAddress::isMulticast() const {
    return (octets[0] & 0x01) == 1;
}

MACAddress& MACAddress::operator=(const MACAddress& other) {
    octets = other.octets;

    return *this;
}

std::ostream& operator<<(std::ostream& os, const MACAddress& mac) {
    os << mac.toString();
    return os;
}


const std::array<uint8_t, MACADDRESS_SIZE> MACAddress::broadcastAddress = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};