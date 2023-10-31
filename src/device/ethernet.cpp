#include "./ethernet.h"
#include "./device.h"
#include "../protocoale/arp.h"
#include "../date/osi/network.h"
#include <memory>
#include <stdexcept>
#include <iostream>

// "Nu esti conectat la eternet, ai restanta puisor"
bool EthernetInterface::receiveData(DataLinkLayer& data) {
    if (!isOn)
        return false;

    return device.receiveData(data, *this);
}

EthernetInterface::EthernetInterface(Device& _device, bool isUnnumbered):
device(_device), macAddress(publicMACCounter += 1), unnumbered(isUnnumbered) {
}

EthernetInterface::~EthernetInterface() {
    if (link != nullptr)
        link->disconnect();
}   

bool EthernetInterface::connect(EthernetInterface* other) {
    if (link != nullptr)
        disconnect();

    link = other;
    link->link = this;
    return true;
}

bool EthernetInterface::setIpAddress(const SubnetAddress& add) {
    if (!device.getState())
        return false;

    if (add.isMulticastAddress())
        throw std::invalid_argument("Interface was assigned a multicast address.");
    if (add == add.getBroadcastAddress())
        throw std::invalid_argument("Interface was assigned the broadcast address.");
    if (add == add.getNetworkAddress())
        throw std::invalid_argument("Interface was assigned the network address.");

    if (unnumbered)
        throw std::invalid_argument("Unnumbered interface cannot be assigned an IP address.");

    address = add;
    return true;
}

bool EthernetInterface::setMacAddress(const MACAddress& newMacAddress) {
    if (!device.getState())
        return false;
    
    if (newMacAddress.isMulticast())
        throw std::invalid_argument("Interface was assigned a multicast MAC address.");

    macAddress = newMacAddress;
    return true;
}

bool EthernetInterface::setSpeed(unsigned long newSpeed) {
    if (newSpeed < REGULAR_ETHERNET)
        throw std::invalid_argument("Speed cannot be lower than regular ethernet.");

    speed = newSpeed;
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
