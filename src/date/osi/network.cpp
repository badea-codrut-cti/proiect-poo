#include "./network.h"
#include <string>
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