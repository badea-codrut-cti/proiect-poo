#include "./arp.h"
#include <iomanip>
#include <ostream>
#include <tuple>

void pipeHex(std::ostream& o, uint8_t* arr, unsigned long n) {
    for (size_t i = 0; i < n; ++i) {
        o << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(arr[i]);
        if (i < n-1) {
            o << " ";
        }
    }
}

void ARPPayload::print(std::ostream& o) const {
    o << "ARP Payload\n";
    o << "Hardware Address Length: " << hwAddLength << "\n";
    o << "Protocol Address Length: " << protoAddLength << "\n";
    o << "Source Hardware Address: ";
    pipeHex(o, sourceHardwareAddress, hwAddLength);
    o << "\nDestination Hardware Address: ";
    pipeHex(o, destinationHardwareAddress, hwAddLength);
    o << "\nSource Protocol Address: ";
    pipeHex(o, sourceProtocolAddress, protoAddLength);
    o << "\nDestination Protocol Address: ";
    pipeHex(o, destinationProtocolAddress, protoAddLength);
}

template <std::size_t N, std::size_t M>
ARPPayload::ARPPayload(Operation operation, 
    const std::array<uint8_t, N>& srcHwAddress,
    const std::array<uint8_t, N>& destHwAddress,
    const std::array<uint8_t, M>& srcProtoAddress,
    const std::array<uint8_t, M>& destProtoAddress, 
    HardwareType hwType,
    ProtocolType protoType): 
    hardwareType(hwType), protocolType(protoType), operation(operation) {

    hwAddLength = N;
    protoAddLength = M;

    sourceHardwareAddress = new uint8_t[srcHwAddress.size()];
    destinationHardwareAddress = new uint8_t[srcHwAddress.size()];

    sourceProtocolAddress = new uint8_t[srcProtoAddress.size()];
    destinationProtocolAddress = new uint8_t[srcProtoAddress.size()];

    for (unsigned long i = 0; i < srcHwAddress.size(); i++) {
        sourceHardwareAddress[i] = srcHwAddress[i];
        destinationHardwareAddress[i] = destHwAddress[i];
    }

    for(unsigned long i = 0; i < srcProtoAddress.size(); i++) {
        sourceProtocolAddress[i] = srcProtoAddress[i];
        destinationProtocolAddress[i] = destProtoAddress[i];
    }
}

ARPPayload::ARPPayload(const ARPPayload& other) :
    hardwareType(other.hardwareType), protocolType(other.protocolType), operation(other.operation),
    hwAddLength(other.hwAddLength), protoAddLength(other.protoAddLength) {
    
    sourceHardwareAddress = new uint8_t[hwAddLength];
    std::copy(other.sourceHardwareAddress, other.sourceHardwareAddress + hwAddLength, sourceHardwareAddress);

    destinationHardwareAddress = new uint8_t[hwAddLength];
    std::copy(other.destinationHardwareAddress, other.destinationHardwareAddress + hwAddLength, destinationHardwareAddress);

    sourceProtocolAddress = new uint8_t[protoAddLength];
    std::copy(other.sourceProtocolAddress, other.sourceProtocolAddress + protoAddLength, sourceProtocolAddress);

    destinationProtocolAddress = new uint8_t[protoAddLength];
    std::copy(other.destinationProtocolAddress, other.destinationProtocolAddress + protoAddLength, destinationProtocolAddress);
}

ARPPayload& ARPPayload::operator=(const ARPPayload& other) {
    if (this == &other) {
        /* 
        * Natural selection doesn't exist anymore
        * If a user can do ARPPayload a; a = a; 
        * and not suffer any consequences
        */
        return *this;
    }

    operation = other.operation;
    hardwareType = other.hardwareType;
    protocolType = other.protocolType;

    if (hwAddLength != other.hwAddLength) {
        delete[] sourceHardwareAddress;
        delete[] destinationHardwareAddress;

        hwAddLength = other.hwAddLength;
        sourceHardwareAddress = new uint8_t[hwAddLength];
        destinationHardwareAddress = new uint8_t[hwAddLength];
    }

    if (protoAddLength != other.protoAddLength) {
        delete[] sourceProtocolAddress;
        delete[] destinationProtocolAddress;

        protoAddLength = other.protoAddLength;
        sourceProtocolAddress = new uint8_t[protoAddLength];
        destinationProtocolAddress = new uint8_t[protoAddLength];
    }
    
    std::copy(other.sourceHardwareAddress, other.sourceHardwareAddress + hwAddLength, sourceHardwareAddress);
    std::copy(other.destinationHardwareAddress, other.destinationHardwareAddress + hwAddLength, destinationHardwareAddress);
    std::copy(other.sourceProtocolAddress, other.sourceProtocolAddress + protoAddLength, sourceProtocolAddress);
    std::copy(other.destinationProtocolAddress, other.destinationProtocolAddress + protoAddLength, destinationProtocolAddress);

    return *this;
}

ARPPayload::~ARPPayload() {
    delete[] sourceHardwareAddress;
    delete[] destinationHardwareAddress;
    delete[] sourceProtocolAddress;
    delete[] destinationProtocolAddress;
}

ARPPayload::Operation ARPPayload::getOperation() const {
    return operation;
}

const uint8_t* ARPPayload::getSourceHardwareAddress() const {
    return sourceHardwareAddress;
}

const uint8_t* ARPPayload::getDestinationHardwareAddress() const {
    return destinationHardwareAddress;
}

const uint8_t* ARPPayload::getSourceProtocolAddress() const {
    return sourceProtocolAddress;
}

const uint8_t* ARPPayload::getDestinationProtocolAddress() const {
    return destinationProtocolAddress;
}

ARPPayload::HardwareType ARPPayload::getHardwareType() const {
    return hardwareType;
}

ARPPayload::ProtocolType ARPPayload::getProtocolType() const {
    return protocolType;
}

ARPPayload ARPParser::createARPRequest(const MACAddress& hwSrc, const IPv4Address& hwDest, const IPv4Address& protoDest) {
    return {ARPPayload::REQUEST, 
    hwSrc.getOctets(), {0, 0, 0, 0, 0, 1}, 
    hwDest.getOctets(), protoDest.getOctets()};
}

ARPPayload ARPParser::createARPReply(const MACAddress& hwSrc, const MACAddress& hwDest, const IPv4Address& protoSrc, const IPv4Address& protoDest) {
    return {ARPPayload::REPLY, hwSrc.getOctets(),
    hwDest.getOctets(), protoSrc.getOctets(), protoDest.getOctets()};
}

std::tuple<MACAddress, MACAddress, IPv4Address, IPv4Address> ARPParser::parseARPPayload(const ARPPayload& payload) {
    std::array<uint8_t, MACADDRESS_SIZE> hwSrc{};
    std::array<uint8_t, MACADDRESS_SIZE> hwDest{};

    for (uint8_t i = 0; i < MACADDRESS_SIZE; i++) {
        hwSrc[i] = payload.getSourceHardwareAddress()[i];
        hwDest[i] = payload.getDestinationHardwareAddress()[i];
    }

    std::array<uint8_t, IPV4_SIZE> protoSrc{};
    std::array<uint8_t, IPV4_SIZE> protoDest{};

    for (uint8_t i = 0; i < IPV4_SIZE; i++) {
        protoSrc[i] = payload.getSourceProtocolAddress()[i];
        protoDest[i] = payload.getDestinationProtocolAddress()[i];
    }

    return std::make_tuple(MACAddress{hwSrc}, MACAddress{hwDest}, IPv4Address{protoSrc}, IPv4Address{protoDest});
}