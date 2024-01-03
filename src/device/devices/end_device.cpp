#include "./end_device.h"
#include "../../date/osi/network.h"
#include "../../date/osi/osiexcept.h"

EndDevice::EndDevice(): Device() {}

EndDevice::EndDevice(const EndDevice& other):
Device(other) {

}

bool EndDevice::setIPv4Address(const SubnetAddressV4& add) {
    return adapter[0].setIpAddress(add);
}

bool EndDevice::setDefaultGateway(const IPv4Address& addr) {
    return adapter[0].setDefaultGateway(addr);
}

bool EndDevice::setDefaultGateway(const IPv6Address& addr) {
    return adapter[0].setDefaultGateway(addr);
}

bool EndDevice::connect(EthernetInterface* _int) {
    return adapter[0].connect(_int);
} 

bool EndDevice::sendData(L2Payload& data, const IPv4Address& target) {
    if(getIPv4Address().isLoopbackAddress())
        return false;

    if (getIPv4Address().getNetworkAddress() == target)
        throw InvalidPacketException(target);

    bool hitRouter = !getIPv4Address().isInSameSubnet(target);

    IPv4Address defaultGatewayV4 = adapter[0].getIPv4DefaultGateway();

    if (hitRouter) 
        sendARPRequest(defaultGatewayV4, false);
    else 
        sendARPRequest(target, false);

    DataLinkLayer l2(adapter[0].getMacAddress(), 
    hitRouter ? getArpEntryOrBroadcast(defaultGatewayV4) : getArpEntryOrBroadcast(target),
     data, DataLinkLayer::IPV4);
    NetworkLayerV4 l3(l2, getIPv4Address(), target);

    return adapter[0].sendData(l3);
}

SubnetAddressV4 EndDevice::getIPv4Address() const {
    return adapter[0].getIPv4Address();
}

IPv4Address EndDevice::getIPv4DefaultGateway() const {
    return adapter[0].getIPv4DefaultGateway();
}

EndDevice::operator EthernetInterface&() {
    return adapter[0];
}

Device* EndDevice::clone() const {
    return new EndDevice(*this);
}