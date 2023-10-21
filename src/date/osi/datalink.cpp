#include "./datalink.h"
#include "../../protocoale/arp.h"
#include <iostream>

DataLinkLayer::DataLinkLayer(const MACAddress& _source, const MACAddress& _destination, L2Payload& _payload, L2TypeField _l2type = IPV4): 
source(_source), destination(_destination), payload(_payload), l2type(_l2type) {

}

DataLinkLayer::DataLinkLayer(const DataLinkLayer& layer): 
source(layer.source), destination(layer.destination), payload(layer.payload), l2type(layer.l2type) {
}

DataLinkLayer::~DataLinkLayer() {
    
}

MACAddress DataLinkLayer::getMACSource() const {
    return source;
}

MACAddress DataLinkLayer::getMACDestination() const {
    return destination;
}

L2Payload& DataLinkLayer::getPayload() const {
    return payload;
}

DataLinkLayer::L2TypeField DataLinkLayer::getL2Type() const {
    return l2type;
}

std::ostream& operator<<(std::ostream& os, const DataLinkLayer& other) {
	os << "Source MAC: " << other.source << "\r\n";
    os << "Destination MAC: " << other.destination << "\r\n";
	return os;
}