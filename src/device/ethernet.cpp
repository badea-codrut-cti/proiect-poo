#include "./ethernet.h"
#include "./device.h"
#include "../protocoale/arp.h"
#include "../date/osi/network.h"
#include <memory>
#include <stdexcept>
#include <iostream>

// "Nu esti conectat la eternet, ai restanta puisor"
bool EthernetInterface::receiveData(DataLinkLayer& _data) {
    if (!isOn)
        return false;

    return device.receiveData(_data, *this);
}

EthernetInterface::EthernetInterface(Device& _device):
device(_device), macAddress(publicMACCounter += 1) {
}

EthernetInterface::~EthernetInterface() {
    if (link != nullptr)
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

bool EthernetInterface::sendARPRequest(const IPv4Address& _add) {
    if (!address.isInSameSubnet(_add))
        throw std::invalid_argument("Cannot send an ARP request to a different subnet.");

    ARPPayload pl = ARPPayload::createARPRequest(macAddress, address, _add);
    DataLinkLayer l2(macAddress, MACAddress::broadcastAddress, pl, DataLinkLayer::ARP);
    NetworkLayer l3(l2, address, _add);
    return sendData(l3);
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

SubnetAddress EthernetInterface::getAddress() const {
    return address;
}

std::ostream& operator<<(std::ostream& os, const EthernetInterface& ip) {
    os << "mac-address " << ip.macAddress << "\r\n";
    os << "ip address " << ((IPv4Address)ip.address) << " " << ip.address.getMaskDotNotation() << "\r\n";
    os << "speed " << std::to_string(ip.speed) << "\r\n";
    os << "bandwidth "<< std::to_string(ip.bandwidth) << "\r\n";
    os << (ip.isOn ? "no shutdown" : "shutdown") << "\r\n";
    return os;
}

MACAddress EthernetInterface::publicMACCounter("00:00:00:00:00:00");
