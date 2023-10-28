#include "./end_device.h"
#include "../../date/osi/network.h"

bool EndDevice::setIpAddress(const SubnetAddress& add) {
    return adapter[0].setIpAddress(add);
}

bool EndDevice::connect(EthernetInterface* _int) {
    return adapter[0].connect(_int);
} 

bool EndDevice::setDefaultGateway(const IPv4Address& add) {
    defaultGateway = add;
    return true;
}

bool EndDevice::sendData(L2Payload& _data, const IPv4Address& add) {
    if(adapter[0].getAddress().isLoopbackAddress())
        return false;

    bool hitRouter = !adapter[0].getAddress().isInSameSubnet(add);

    if (hitRouter) 
        sendARPRequest(defaultGateway, false);
    else 
        sendARPRequest(add, false);

    DataLinkLayer l2(adapter[0].getMacAddress(), 
    hitRouter ? getArpEntryOrBroadcast(defaultGateway) : getArpEntryOrBroadcast(add),
     _data, DataLinkLayer::IPV4);
    NetworkLayer l3(l2, adapter[0].getAddress(), add);

    return adapter[0].sendData(l3);
}

SubnetAddress EndDevice::getAddress() const {
    return adapter[0].getAddress();
}

IPv4Address EndDevice::getDefaultGateway() const {
    return defaultGateway;
}