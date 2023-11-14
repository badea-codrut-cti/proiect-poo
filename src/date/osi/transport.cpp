#include "./transport.h"
#include <iostream>

// "Copilu' filmele au 24 FPS din cauza la Coca Cola"

TransportLayer::TransportLayer(NetworkLayer& layer, uint16_t _sourcePort, uint16_t _destinationPort): 
NetworkLayer(layer), sourcePort(_sourcePort), destinationPort(_destinationPort) {

}

TransportLayer::TransportLayer(const TransportLayer& layer): 
NetworkLayer(layer), sourcePort(layer.sourcePort), destinationPort(layer.destinationPort) {

}

uint16_t TransportLayer::getSourcePort() const {
    return sourcePort;
}

uint16_t TransportLayer::getDestinationPort() const {
    return destinationPort;
}

DataLinkLayer* TransportLayer::clone() const {
    return new TransportLayer(*this);
}

std::ostream& operator<<(std::ostream& os, const TransportLayer& other) {
	os << (DataLinkLayer&) other;
    os << other.getIPSource() << ":" << (unsigned) other.sourcePort << "\n";
    os << other.getIPDestination() << ":" << (unsigned) other.destinationPort << "\n";
	return os;
}