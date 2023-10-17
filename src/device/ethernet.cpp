#include "./ethernet.h"
#include "./device.h"
#include <memory>
#include <stdexcept>
#include <iostream>

// "Nu esti conectat la eternet, ai restanta puisor"
bool EthernetInterface::receiveData(DataLinkLayer& _data) {
    return device.receiveData(_data, *this);
}

EthernetInterface::EthernetInterface(Device& _device):
device(_device), macAddress(publicMACCounter += 1) {
}

EthernetInterface::~EthernetInterface() {
    if (link)
        link->disconnect();
}   

bool EthernetInterface::connect(EthernetInterface* _link) {
    link = _link;
    link->link = this;
    return true;
}

bool EthernetInterface::setIpAddress(const SubnetAddress& add) {
    if (add.isMulticastAddress())
        throw std::invalid_argument("Interface was assigned a multicast address.");
    if (add == add.getBroadcastAddress())
        throw std::invalid_argument("Interface was assigned the broadcast address.");
    if (add == add.getNetworkAddress())
        throw std::invalid_argument("Interface was assigned the network address.");

    address = add;
    return true;
}

bool EthernetInterface::setMacAddress(const MACAddress& _macAddress) {
    if (_macAddress.isMulticast())
        throw std::invalid_argument("Interface was assigned a multicast MAC address.");

    macAddress = _macAddress;
    return true;
}

bool EthernetInterface::setSpeed(unsigned long _speed) {
    if (_speed < REGULAR_ETHERNET)
        throw std::invalid_argument("Speed cannot be lower than regular ethernet.");

    speed = _speed;
    return true;
}

bool EthernetInterface::setDuplex(Duplex _duplex) {
    duplex = _duplex;
    return true;
}

bool EthernetInterface::turnOn() {
    return isOn = true;
}

bool EthernetInterface::turnOff() {
    return isOn = false;
}

bool EthernetInterface::sendData(DataLinkLayer& data) {
    if (link == nullptr || !isOn)
        return false; 

    if (link->duplex != duplex && duplex != AUTO)
        return false;

    return link->receiveData(data);
}

bool EthernetInterface::disconnect() {
    if (!link)
        return true;

    link->link = nullptr;
    link = nullptr;
    return true;
}

unsigned long EthernetInterface::getSpeed() const {
    return speed;
}

MACAddress EthernetInterface::getMacAddress() const {
    return macAddress;
}

bool EthernetInterface::getState() const {
    return isOn;
}

std::ostream& operator<<(std::ostream& os, const EthernetInterface& ip) {
    os << std::string("mac-address ") << ip.macAddress << std::string("\r\n");
    os << std::string("ip address ") << ip.address << std::string("\r\n!\r\n");
    os << std::string("speed ") << std::to_string(ip.speed) << std::string("\r\n");
    os << std::string("bandwidth ") << std::to_string(ip.bandwidth) << std::string("\r\n");

    return os;
}

MACAddress EthernetInterface::publicMACCounter("00:00:00:00:00:00");
