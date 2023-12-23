#include "./network.h"
#include <iostream>

// "Copilu, ce e ala Datagram? Nu exista, dom student."

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