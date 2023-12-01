#include "./ethernet.h"
#include "./device.h"
#include "../date/osi/network.h"
#include <stdexcept>
#include <iostream>

// "Nu esti conectat la eternet, ai restanta puisor"
bool EthernetInterface::receiveData(const DataLinkLayer& data) {
    if (!isOn)
        return false;

    return device.receiveData(data, *this);
}

EthernetInterface::EthernetInterface(Device& _device, unsigned long long maxSpeed, bool isUnnumbered):
device(_device), macAddress(publicMACCounter += 1), burnAddress(macAddress), unnumbered(isUnnumbered), 
speed(maxSpeed), maxSpeed(maxSpeed), bandwidth(maxSpeed) {
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

    if (newSpeed > maxSpeed)
        throw std::invalid_argument("Speed cannot be higher than max speed.");

    speed = newSpeed;
    return true;
}

bool EthernetInterface::turnOn() {
    return isOn = true;
}

bool EthernetInterface::turnOff() {
    return isOn = false;
}

bool EthernetInterface::sendData(const DataLinkLayer& data) {
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

unsigned long long EthernetInterface::getMaxSpeed() const {
    return maxSpeed;
}

unsigned long long EthernetInterface::getSpeed() const {
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

bool EthernetInterface::isUnnumbered() const {
    return unnumbered;
}

MACAddress EthernetInterface::getBurntInAddress() const {
    return burnAddress;
}

EthernetInterface* EthernetInterface::copy(Device& dev) const {
    auto ret = new EthernetInterface(dev, maxSpeed, unnumbered);
    ret->speed = speed;
    ret->bandwidth = bandwidth;
    ret->isOn = isOn;
    return ret;
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
