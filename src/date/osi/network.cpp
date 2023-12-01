#include "./network.h"
#include <iostream>

// "Copilu, ce e ala Datagram? Nu exista, dom student."

NetworkLayer::NetworkLayer(DataLinkLayer& layer, const IPv4Address& sourceIP, const IPv4Address& destIP, 
uint8_t ttl, IPProtocolType l3proto): 
DataLinkLayer(layer), source(sourceIP), destination(destIP), TTL(ttl), proto(l3proto) {

}

NetworkLayer::NetworkLayer(const NetworkLayer& other): 
DataLinkLayer(other), source(other.source), destination(other.destination), TTL(other.getTTL()),
proto(other.proto) {

}

IPv4Address NetworkLayer::getIPSource() const {
    return source;
}

IPv4Address NetworkLayer::getIPDestination() const {
    return destination;
}

uint8_t NetworkLayer::getTTL() const {
    return TTL;
}

void NetworkLayer::age() {
    TTL--;
}

NetworkLayer* NetworkLayer::clone() const {
    return new NetworkLayer(*this);
}

NetworkLayer::IPProtocolType NetworkLayer::getL3Protocol() const {
    return proto;
}

std::ostream& operator<<(std::ostream& os, const NetworkLayer& other) {
    os << (DataLinkLayer&) other;
	os << "Source IP address: " << other.source << "\n";
    os << "Destination IP address: " << other.destination << "\n";
	return os;
}

std::ostream& operator<<(std::ostream& os, const NetworkLayer::IPProtocolType& l3type) {
    switch (l3type) {
        case NetworkLayer::ICMP: {
            os << "ICMP";
            break;
        }
        case NetworkLayer::IPV4: {
            os << "IPV4";
            break;
        }
        case NetworkLayer::OSPF: {
            os << "OSPF";
            break;
        }
        case NetworkLayer::TCP: {
            os << "TCP";
            break;
        }
        case NetworkLayer::UDP: {
            os << "UDP";
            break;
        }
        case NetworkLayer::AH: {
            os << "Authentication";
            break;
        }
        case NetworkLayer::ESP:
        case NetworkLayer::GRE: {
            os << "Encapsulation";
            break;
        }
        case NetworkLayer::SCTP: {
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