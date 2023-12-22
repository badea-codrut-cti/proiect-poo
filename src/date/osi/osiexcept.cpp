#include "osiexcept.h"
#include <sstream>

std::string ipMessage(const SubnetAddressV4& addr) {
    return addr.isLoopbackAddress() ? "a loopback" : 
    addr.isMulticastAddress() ? "a multicast" : "a reserved";
} 

InvalidPayloadException::InvalidPayloadException(DataLinkLayer::L2TypeField l2): 
InvalidFrameException("Invalid payload for frame type header."), 
l2Type(l2) {
    std::ostringstream oss;
    oss << l2Type;
    msg = oss.str() + " frame header does not have a matching payload.";
}

InvalidPayloadException::InvalidPayloadException(NetworkLayer::IPProtocolType l3): 
InvalidFrameException("Invalid payload for packet type header."),  
l3Type(l3) {
    std::ostringstream oss;
    oss << l3Type;
    msg = oss.str() + " packet header does not have a matching payload.";
}

InvalidPacketException::InvalidPacketException(const MACAddress& mac): 
InvalidFrameException(
    "Source MAC address " + mac.toString() + 
    " is a " + (
        mac.isMulticast() ? "multicast" : 
        mac == MACAddress(MACAddress::broadcastAddress) ? "broadcast" : "reserved"
    ) + " address."
) {}

InvalidPacketException::InvalidPacketException(const SubnetAddressV4& addr):
InvalidFrameException(
    "Source IP address " + addr.toString() + " is " +
    (
        addr == addr.getBroadcastAddress() ? "the broadcast" :
        addr == addr.getNetworkAddress() ? "the network" : ipMessage(addr)
    ) + " address."
) {}

InvalidPacketException::InvalidPacketException(const IPv4Address& addr):
InvalidFrameException(
    "Source IP address " + addr.toString() + " is " + ipMessage(SubnetAddressV4(addr)) + " address."
) {}

