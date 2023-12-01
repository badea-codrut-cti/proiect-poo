#include "./datalink.h"
#include "../../protocoale/arp.h"
#include "osiexcept.h"
#include <iostream>

DataLinkLayer::DataLinkLayer(const MACAddress& sourceMac, const MACAddress& destinationMac,
 L2Payload& l2payload, L2TypeField l2frameType = IPV4): 
source(sourceMac), destination(destinationMac), payload(l2payload.clone()), l2type(l2frameType) {
    if (sourceMac.isMulticast() || sourceMac == MACAddress(MACAddress::broadcastAddress))
        throw InvalidPacketException(sourceMac);
}

DataLinkLayer::DataLinkLayer(const DataLinkLayer& layer): 
source(layer.source), destination(layer.destination), payload(layer.payload->clone()), l2type(layer.l2type) {
}

bool DataLinkLayer::setMACSource(const MACAddress& mac) {
    if (mac.isMulticast() || mac == MACAddress(MACAddress::broadcastAddress))
        throw InvalidPacketException(mac);
    source = mac;
    return true;
}

bool DataLinkLayer::setMACDestination(const MACAddress& mac) {
    destination = mac;
    return true;
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

std::ostream& operator<<(std::ostream& os, const DataLinkLayer::L2TypeField& l2type) {
    switch (l2type) {
        case DataLinkLayer::ARP: {
            os << "ARP";
            break;
        }
        case DataLinkLayer::IPV4: {
            os << "IPv4";
            break;
        }        
        case DataLinkLayer::IPV6: {
            os << "IPv6";
            break;
        }
        case DataLinkLayer::LLDP: {
            os << "LLDP";
            break;
        }
        case DataLinkLayer::MPLS: {
            os << "MPLS";
            break;
        }
        case DataLinkLayer::DOT1Q:
        case DataLinkLayer::DOT1X:
        case DataLinkLayer::VLAN_CTAG: {
            os << "VLAN";
            break;
        }

        default: {
            os << "Layer 2";
            break;
        }
    }
    return os;
}