#include "osiexcept.h"

std::string ipMessage(const SubnetAddress& addr) {
    return addr.isLoopbackAddress() ? "a loopback" : addr.isMulticastAddress() ? "a multicast" : "a reserved";
} 

InvalidPayloadException::InvalidPayloadException(DataLinkLayer::L2TypeField l2): 
InvalidFrameException("Invalid payload for frame type header."), 
l2Type(l2), msgType(DataLink) {

}

InvalidPayloadException::InvalidPayloadException(NetworkLayer::IPProtocolType l3): 
InvalidFrameException("Invalid payload for packet type header."),  
l3Type(l3), msgType(Network) {

}

const char* InvalidPayloadException::what() const noexcept {
    auto* pType = new std::string("");
    if (msgType == DataLink) {
        switch (l2Type) {
            case DataLinkLayer::ARP: {
                *pType = "ARP";
                break;
            }
            case DataLinkLayer::IPV4: {
                *pType = "IPv4";
                break;
            }        
            case DataLinkLayer::IPV6: {
                *pType = "IPv6";
                break;
            }
            case DataLinkLayer::LLDP: {
                *pType = "LLDP";
                break;
            }
            case DataLinkLayer::MPLS: {
                *pType = "MPLS";
                break;
            }
            case DataLinkLayer::DOT1Q:
            case DataLinkLayer::DOT1X:
            case DataLinkLayer::VLAN_CTAG: {
                *pType = "VLAN";
                break;
            }

            default: {
                *pType = "Layer 2";
                break;
            }
        }

        *pType += " frame header does not have a matching payload.";
        return pType->c_str();
    } else if (msgType == Network) {
        switch (l3Type) {
            case NetworkLayer::ICMP: {
                *pType = "ICMP";
                break;
            }
            case NetworkLayer::IPV4: {
                *pType = "IPV4";
                break;
            }
            case NetworkLayer::OSPF: {
                *pType = "OSPF";
                break;
            }
            case NetworkLayer::TCP: {
                *pType = "TCP";
                break;
            }
            case NetworkLayer::UDP: {
                *pType = "UDP";
                break;
            }
            case NetworkLayer::AH: {
                *pType = "Authentication";
                break;
            }
            case NetworkLayer::ESP:
            case NetworkLayer::GRE: {
                *pType = "Encapsulation";
                break;
            }
            case NetworkLayer::SCTP: {
                *pType = "SCTP";
                break;
            }
            default: {
                *pType = "Layer 3";
                break;
            }
        }
        *pType += " packet header does not have a matching payload.";
        return pType->c_str();
    }

    //How did we get here?
    delete pType;
    return "";
} 

InvalidPacketException::InvalidPacketException(const MACAddress& mac): 
InvalidFrameException(
    "Source MAC address " + mac.toString() + 
    " is a " + (
        mac.isMulticast() ? "multicast" : 
        mac == MACAddress(MACAddress::broadcastAddress) ? "broadcast" : "reserved"
    ) + " address."
) {}

InvalidPacketException::InvalidPacketException(const SubnetAddress& addr):
InvalidFrameException(
    "Source IP address " + addr.toString() + " is " +
    (
        addr == addr.getBroadcastAddress() ? "the broadcast" :
        addr == addr.getNetworkAddress() ? "the network" : ipMessage(addr)
    ) + " address."
) {}

InvalidPacketException::InvalidPacketException(const IPv4Address& addr):
InvalidFrameException(
    "Source IP address " + addr.toString() + " is " + ipMessage(SubnetAddress(addr)) + " address."
) {}

