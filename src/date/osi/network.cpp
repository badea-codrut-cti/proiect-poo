#include "./network.h"
#include <iostream>

NetworkLayerV4::NetworkLayerV4(DataLinkLayer& layer, const IPv4Address& sourceIP, const IPv4Address& destIP, 
uint8_t ttl, IPv4ProtocolType l3proto): 
DataLinkLayer(layer), source(sourceIP), destination(destIP), TTL(ttl), proto(l3proto) {

}

NetworkLayerV4::NetworkLayerV4(const NetworkLayerV4& other): 
DataLinkLayer(other), source(other.source), destination(other.destination), TTL(other.getTTL()),
proto(other.proto) {

}

IPv4Address NetworkLayerV4::getIPSource() const {
    return source;
}

IPv4Address NetworkLayerV4::getIPDestination() const {
    return destination;
}

uint8_t NetworkLayerV4::getTTL() const {
    return TTL;
}

void NetworkLayerV4::age() {
    TTL--;
}

NetworkLayerV4* NetworkLayerV4::clone() const {
    return new NetworkLayerV4(*this);
}

NetworkLayerV4::IPv4ProtocolType NetworkLayerV4::getL3Protocol() const {
    return proto;
}

std::ostream& operator<<(std::ostream& os, const NetworkLayerV4& other) {
    os << (DataLinkLayer&) other;
	os << "Source IP address: " << other.source << "\n";
    os << "Destination IP address: " << other.destination << "\n";
	return os;
}

std::ostream& operator<<(std::ostream& os, const NetworkLayerV4::IPv4ProtocolType& l3type) {
    switch (l3type) {
        case NetworkLayerV4::ICMP: {
            os << "ICMP";
            break;
        }
        case NetworkLayerV4::IPV4: {
            os << "IPV4";
            break;
        }
        case NetworkLayerV4::OSPF: {
            os << "OSPF";
            break;
        }
        case NetworkLayerV4::TCP: {
            os << "TCP";
            break;
        }
        case NetworkLayerV4::UDP: {
            os << "UDP";
            break;
        }
        case NetworkLayerV4::AH: {
            os << "Authentication";
            break;
        }
        case NetworkLayerV4::ESP:
        case NetworkLayerV4::GRE: {
            os << "Encapsulation";
            break;
        }
        case NetworkLayerV4::SCTP: {
            os << "SCTP";
            break;
        }
        default: {
            os << "Layer 3";
            break;
        }
    }
    return os;
}

NetworkLayerV6::NetworkLayerV6(DataLinkLayer& l2, const IPv6Address& srcIp, 
const IPv6Address& destIp, uint8_t hLimit, IPv6ProtocolType pType): 
DataLinkLayer(l2),
source(srcIp), destination(destIp), hopLimit(hLimit), proto(pType) {

}

NetworkLayerV6::NetworkLayerV6(const NetworkLayerV6& other):
DataLinkLayer(other), source(other.source), destination(other.destination),
hopLimit(other.hopLimit), proto(other.proto) {

}

void age();

IPv6Address NetworkLayerV6::getIPSource() const {
    return source;
}

IPv6Address NetworkLayerV6::getIPDestination() const {
    return destination;
}

uint8_t NetworkLayerV6::getHopLimit() const {
    return hopLimit;
}

NetworkLayerV6* NetworkLayerV6::clone() const {
    return new NetworkLayerV6(*this);
}

NetworkLayerV6::IPv6ProtocolType 
NetworkLayerV6::getL3Protocol() const {
    return proto;
}

void NetworkLayerV6::age() {
    hopLimit--;
}

std::ostream& operator<<(std::ostream& os, const NetworkLayerV6& other) {
    os << (DataLinkLayer&) other;
	os << "Source IP address: " << other.source << "\n";
    os << "Destination IP address: " << other.destination << "\n";
	return os;
}