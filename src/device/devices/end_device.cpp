#include "./end_device.h"
#include "../../date/osi/network.h"
#include "../../date/osi/osiexcept.h"

EndDevice::EndDevice(): Device() {}

EndDevice::EndDevice(const EndDevice& other):
Device(other), defaultGateway(other.defaultGateway) {

}

bool EndDevice::setIpAddress(const SubnetAddress& add) {
    return adapter[0].setIpAddress(add);
}

bool EndDevice::connect(EthernetInterface* _int) {
    return adapter[0].connect(_int);
} 

bool EndDevice::setDefaultGateway(const IPv4Address& gatewayAddress) {
    defaultGateway = gatewayAddress;
    return true;
}

bool EndDevice::sendData(L2Payload& data, const IPv4Address& target) {
    if(adapter[0].getAddress().isLoopbackAddress())
        return false;

    if (adapter[0].getAddress().getNetworkAddress() == target)
        throw InvalidPacketException(target);

    bool hitRouter = !getAddress().isInSameSubnet(target);

    if (hitRouter) 
        sendARPRequest(defaultGateway, false);
    else 
        sendARPRequest(target, false);

    DataLinkLayer l2(adapter[0].getMacAddress(), 
    hitRouter ? getArpEntryOrBroadcast(defaultGateway) : getArpEntryOrBroadcast(target),
     data, DataLinkLayer::IPV4);
    NetworkLayer l3(l2, getAddress(), target);

    return adapter[0].sendData(l3);
}

bool EndDevice::sendPing(const IPv4Address& dest) {
    ICMPPayload pl(ICMPPayload::ECHO_REQUEST, 0);

    bool hitRouter = !getAddress().isInSameSubnet(dest);

    if (hitRouter) 
        sendARPRequest(defaultGateway, false);
    else 
        sendARPRequest(dest, false);

    DataLinkLayer l2(adapter[0].getMacAddress(),
    hitRouter ? getArpEntryOrBroadcast(defaultGateway) : getArpEntryOrBroadcast(dest), 
    pl, DataLinkLayer::IPV4);

    NetworkLayer l3(l2, getAddress(), dest, DEFAULT_TTL, NetworkLayer::ICMP);
    return adapter[0].sendData(l3);
}

SubnetAddress EndDevice::getAddress() const {
    return adapter[0].getAddress();
}

IPv4Address EndDevice::getDefaultGateway() const {
    return defaultGateway;
}

EndDevice::operator EthernetInterface&() {
    return adapter[0];
}

Device* EndDevice::clone() const {
    return new EndDevice(*this);
}