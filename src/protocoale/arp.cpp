#include "./arp.h"
#include <iomanip>
#include <ostream>

template <std::size_t N>
void pipeHex(std::ostream& o, std::array<uint8_t, N> arr) {
    for (size_t i = 0; i < N; ++i) {
        o << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(arr[i]);
        if (i < N-1) {
            o << " ";
        }
    }
}

template <std::size_t N, std::size_t M>
void ARPPayload<N, M>::print(std::ostream& o) const {
    o << "ARP Payload\n";
    o << "Hardware Address Length: " << N << "\n";
    o << "Protocol Address Length: " << M << "\n";
    o << "Source Hardware Address: ";
    pipeHex(o, sourceHardwareAddress);
    o << "\nDestination Hardware Address: ";
    pipeHex(o, destinationHardwareAddress);
    o << "\nSource Protocol Address: ";
    pipeHex(o, sourceProtocolAddress);
    o << "\nDestination Protocol Address: ";
    pipeHex(o, destinationProtocolAddress);
}

template <std::size_t N, std::size_t M>
ARPPayload<N, M>::ARPPayload(ARPData::Operation operation, 
    const std::array<uint8_t, N>& srcHwAddress,
    const std::array<uint8_t, N>& destHwAddress,
    const std::array<uint8_t, M>& srcProtoAddress,
    const std::array<uint8_t, M>& destProtoAddress, 
    ARPData::HardwareType hwType,
    ARPData::ProtocolType protoType): 
    hardwareType(hwType), protocolType(protoType), operation(operation),
    sourceHardwareAddress(srcHwAddress), destinationHardwareAddress(destHwAddress),
    sourceProtocolAddress(srcProtoAddress), destinationProtocolAddress(destProtoAddress) {

}

template <std::size_t N, std::size_t M>
ARPPayload<N, M>::ARPPayload(const ARPPayload<N, M>& other) :
    hardwareType(other.hardwareType), protocolType(other.protocolType), 
    operation(other.operation),
    sourceHardwareAddress(other.sourceHardwareAddress),
    destinationHardwareAddress(other.destinationHardwareAddress),
    sourceProtocolAddress(other.sourceProtocolAddress),
    destinationProtocolAddress(other.destinationProtocolAddress) {
    

}

template <std::size_t N, std::size_t M>
ARPPayload<N, M>& ARPPayload<N, M>::operator=(const ARPPayload<N, M>& other) {
    operation = other.operation;
    hardwareType = other.hardwareType;
    protocolType = other.protocolType;

    sourceHardwareAddress = other.sourceHardwareAddress;
    destinationHardwareAddress = other.destinationHardwareAddress;

    sourceProtocolAddress = other.sourceProtocolAddress;
    destinationProtocolAddress = other.destinationProtocolAddress;
    return *this;
}

template <std::size_t N, std::size_t M>
ARPPayload<N, M>::~ARPPayload() {

}

template <std::size_t N, std::size_t M>
ARPData::Operation ARPPayload<N, M>::getOperation() const {
    return operation;
}

template <std::size_t N, std::size_t M>
std::array<uint8_t, N> ARPPayload<N, M>::getSourceHardwareAddress() const {
    return sourceHardwareAddress;
}

template <std::size_t N, std::size_t M>
std::array<uint8_t, N> ARPPayload<N, M>::getDestinationHardwareAddress() const {
    return destinationHardwareAddress;
}

template <std::size_t N, std::size_t M>
std::array<uint8_t, M> ARPPayload<N, M>::getSourceProtocolAddress() const {
    return sourceProtocolAddress;
}

template <std::size_t N, std::size_t M>
std::array<uint8_t, M> ARPPayload<N, M>::getDestinationProtocolAddress() const {
    return destinationProtocolAddress;
}

template <std::size_t N, std::size_t M>
ARPData::HardwareType ARPPayload<N, M>::getHardwareType() const {
    return hardwareType;
}

template <std::size_t N, std::size_t M>
ARPData::ProtocolType ARPPayload<N, M>::getProtocolType() const {
    return protocolType;
}

template <std::size_t N, std::size_t M>
L2Payload* ARPPayload<N, M>::clone() const {
    return new ARPPayload(*this);
}

ARPIPv4::ARPIPv4(ARPData::Operation op, const MACAddress& srcMac, const MACAddress& destMac,
const IPv4Address& srcIpv4, const IPv4Address& destIpv4): 
ARPPayload<MACADDRESS_SIZE, IPV4_SIZE>(op,
 srcMac.getOctets(), destMac.getOctets(),
srcIpv4.getOctets(), destIpv4.getOctets(), 
ARPData::ETHERNET, ARPData::IPV4) {

}

MACAddress ARPIPv4::getSourceMacAddress() const {
    return MACAddress(getSourceHardwareAddress());
}

MACAddress ARPIPv4::getDestinationMacAddress() const {
    return MACAddress(getDestinationHardwareAddress());
}

IPv4Address ARPIPv4::getSourceIPv4Address() const {
    return IPv4Address(getSourceProtocolAddress());
}

IPv4Address ARPIPv4::getDestinationIPv4Address() const {
    return IPv4Address(getDestinationProtocolAddress());
}

ARPData::Operation ARPIPv4::getOperation() const {
    return ARPPayload<MACADDRESS_SIZE, IPV4_SIZE>::getOperation();
}

L2Payload* ARPIPv4::clone() const {
    return new ARPIPv4(*this);
}

ARPIPv4::~ARPIPv4() {
    
}