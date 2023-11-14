#include "./datalink.h"
#include "../../protocoale/arp.h"
#include <iostream>

DataLinkLayer::DataLinkLayer(const MACAddress& sourceMac, const MACAddress& destinationMac,
 L2Payload& l2payload, L2TypeField l2frametype = IPV4): 
source(sourceMac), destination(destinationMac), payload(l2payload.clone()), l2type(l2frametype) {

}

DataLinkLayer::DataLinkLayer(const DataLinkLayer& layer): 
source(layer.source), destination(layer.destination), payload(layer.payload->clone()), l2type(layer.l2type) {
}

DataLinkLayer::~DataLinkLayer() {
    delete payload;
}

MACAddress DataLinkLayer::getMACSource() const {
    return source;
}

MACAddress DataLinkLayer::getMACDestination() const {
    return destination;
}

const L2Payload* DataLinkLayer::getPayload() const {
    return payload;
}

DataLinkLayer::L2TypeField DataLinkLayer::getL2Type() const {
    return l2type;
}

DataLinkLayer* DataLinkLayer::clone() const {
    return new DataLinkLayer(*this);
}

std::ostream& operator<<(std::ostream& os, const DataLinkLayer& other) {
	os << "Source MAC: " << other.source << "\r\n";
    os << "Destination MAC: " << other.destination << "\r\n";
	return os;
}