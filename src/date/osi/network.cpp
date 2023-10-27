#include "./network.h"
#include <string>

// "Copilu, ce e ala Datagram? Nu exista, dom student."

NetworkLayer::NetworkLayer(DataLinkLayer& layer, const IPv4Address& _source, const IPv4Address& _destination, 
uint8_t _ttl, IPProtocolType _proto): 
DataLinkLayer(layer), source(_source), destination(_destination), TTL(_ttl), proto(_proto) {

}

NetworkLayer::NetworkLayer(const NetworkLayer& layer): 
DataLinkLayer(layer), source(layer.source), destination(layer.destination), TTL(layer.getTTL()) {

}

void NetworkLayer::age() {
    if (TTL > 0)
        TTL--;
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

NetworkLayer::IPProtocolType NetworkLayer::getL3Protocol() const {
    return proto;
}

std::ostream& operator<<(std::ostream& os, const NetworkLayer& other) {
	os << other.source << std::string("->") << other.destination <<
    std::string(" (TTL:") << std::to_string(other.TTL) << std::string(")");
	return os;
}