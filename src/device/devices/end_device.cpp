#include "./end_device.h"

bool EndDevice::setIpAddress(const SubnetAddress& add) {
    return adapter[0].setIpAddress(add);
}

bool EndDevice::connect(EthernetInterface* _int) {
    return adapter[0].connect(_int);
} 

bool EndDevice::setDefaultGateway(const IPv4Address& add) {
    defaultGateway = IPv4Address{add};
    return true;
}

bool EndDevice::sendData(DataLinkLayer& _data) {
    return adapter[0].sendData(_data);
}

SubnetAddress EndDevice::getAddress() const {
    return adapter[0].getAddress();
}

IPv4Address EndDevice::getDefaultGateway() const {
    return defaultGateway;
}