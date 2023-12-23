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
device(_device),
macAddress((publicMACCounter += 1).getOctets()), 
linkLocalAddress(macAddress),
burnAddress(macAddress), 
unnumbered(isUnnumbered), 
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

bool EthernetInterface::setIpAddress(const SubnetAddressV4& add) {
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

    addressV4 = add;
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

SubnetAddressV4 EthernetInterface::getIPv4Address() const {
    return addressV4;
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
    ret->defaultGatewayV4 = defaultGatewayV4;
    ret->defaultGatewayV6 = defaultGatewayV6;
    return ret;
}

bool EthernetInterface::setDefaultGateway(const IPv4Address& addr) {
    defaultGatewayV4 = addr;
    return true;
}

bool EthernetInterface::setDefaultGateway(const IPv6Address& addr) {
    defaultGatewayV6 = addr;
    return true;
}

bool EthernetInterface::addGlobalUnicastAddress(const SubnetAddressV6& addr) {
    globalUnicastAddresses.push_back(addr);
    return true;
}

bool EthernetInterface::removeGlobalUnicastAddress(const SubnetAddressV6& addr) {
    globalUnicastAddresses.erase(std::remove(
        globalUnicastAddresses.begin(),
        globalUnicastAddresses.end(),
        addr
    ), globalUnicastAddresses.end());

    return true;
}

std::vector<SubnetAddressV6> EthernetInterface::getGlobalUnicastAddresses() const {
    return globalUnicastAddresses;
}

IPv6Address EthernetInterface::getLinkLocalAddress() const {
    return linkLocalAddress;
}

IPv6Address EthernetInterface::getIPv6DefaultGateway() const {
    return defaultGatewayV6;
}

IPv4Address EthernetInterface::getIPv4DefaultGateway() const {
    return defaultGatewayV4;
}

std::ostream& operator<<(std::ostream& os, const EthernetInterface& ip) {
    os << "mac-address " << ip.macAddress << "\r\n";
    if (ip.addressV4 != IPv4Address("127.0.0.1"))
        os << "ip address " << ((IPv4Address)ip.addressV4) << " " << ip.addressV4.getMaskDotNotation() << "\r\n";
    else
        os << "no ip address\r\n";

    for (auto gua : ip.globalUnicastAddresses) {
        os << "ipv6 address " << gua << "\r\n";
    }
    os << "ipv6 address " << ip.linkLocalAddress << " link-local\r\n";
    os << "ip default-gateway " << ip.defaultGatewayV4 << "\r\n";
    os << "ipv6 default-gateway " << ip.defaultGatewayV6 << "\r\n";
    os << "speed " << std::to_string(ip.speed) << "\r\n";
    os << "bandwidth "<< std::to_string(ip.bandwidth) << "\r\n";
    os << (ip.isOn ? "no shutdown" : "shutdown") << "\r\n";
    return os;
}

MACAddress EthernetInterface::publicMACCounter("00:00:00:00:00:00");
