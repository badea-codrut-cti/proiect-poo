#include "./network.h"

// "Copilu, ce e ala Datagram? Nu exista, dom student."

NetworkLayer::NetworkLayer(DataLinkLayer& layer, const IPv4Address& _source, const IPv4Address& _destination, unsigned long _ttl=118): 
DataLinkLayer(layer), source(_source), destination(_destination), TTL(_ttl) {

}

NetworkLayer::NetworkLayer(const NetworkLayer& layer): 
DataLinkLayer(layer), source(layer.source), destination(layer.destination), TTL(0) {

}

IPv4Address NetworkLayer::getIPSource() const {
    return source;
}

IPv4Address NetworkLayer::getIPDestination() const {
    return destination;
}

std::ostream& operator<<(std::ostream& os, const NetworkLayer& other) {
	os << other.source << std::string("->") << other.destination;
	return os;
}