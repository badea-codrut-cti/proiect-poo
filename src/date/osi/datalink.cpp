#include "./datalink.h"

DataLinkLayer::DataLinkLayer(const MACAddress& _source, const MACAddress& _destination): 
source(_source), destination(_destination) {

}

DataLinkLayer::DataLinkLayer(const DataLinkLayer& layer): source(layer.source), destination(layer.destination) {

}

MACAddress DataLinkLayer::getMACSource() const {
    return source;
}

MACAddress DataLinkLayer::getMACDestination() const {
    return destination;
}

std::ostream& operator<<(std::ostream& os, const DataLinkLayer& other) {
	os << other.source << std::string("->") << other.destination;
	return os;
}