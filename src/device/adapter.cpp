#include "./adapter.h"
#include <ostream>
#include <stdexcept>
#include <iostream>

NetworkAdapter::NetworkAdapter(Device& device, uint8_t intCount, bool passive): 
numInterfaces(intCount) {
    interfaces = new EthernetInterface*[intCount];
    for (uint8_t i=0;i < numInterfaces; i++)
        interfaces[i] = new EthernetInterface(device, GIGABIT_ETHERNET, passive);
}

NetworkAdapter::NetworkAdapter(Device& device, uint8_t intCount, std::function<EthernetInterface*(Device&, uint8_t)> func):
numInterfaces(intCount) {
    interfaces = new EthernetInterface*[intCount];
    for (uint8_t i=0;i < numInterfaces; i++)
        interfaces[i] = func(device, i);
}

bool NetworkAdapter::copy(Device& dev, const NetworkAdapter& other) {
    if (this == &other) {
        // Self assignment would return in making everything a nullptr
        // I have no idea when we will need to swap the card owner to another device
        for (uint8_t i = 0; i < numInterfaces; i++) {
            EthernetInterface* interface = interfaces[i]->copy(dev);
            delete interfaces[i];
            interfaces[i] = interface;
        }
        return true;
    }

    for (uint8_t i = 0; i < numInterfaces; i++)
        delete interfaces[i];

    delete[] interfaces;

    numInterfaces = other.numInterfaces;
    interfaces = new EthernetInterface*[numInterfaces];

    for (uint8_t i = 0; i < numInterfaces; i++) {
        interfaces[i] = other.interfaces[i]->copy(dev);
    }

    return true;
}

NetworkAdapter::~NetworkAdapter() {
    for (uint8_t i=0;i < numInterfaces; i++)
        delete interfaces[i];
    
    delete[] interfaces;
}

uint8_t NetworkAdapter::interfaceCount() const {
    return numInterfaces;
}

uint8_t NetworkAdapter::getIntefaceIndex(const MACAddress& mac) const {
    for (uint8_t i = 0; i < numInterfaces; i++)
        if (interfaces[i]->getMacAddress() == mac)
            return i;
    throw std::out_of_range("Index out of range");
}

uint8_t NetworkAdapter::getIntefaceIndex(const IPv4Address& other) const {
    for (uint8_t i = 0; i < numInterfaces; i++)
        if (interfaces[i]->getAddress() == other)
            return i;
    throw std::out_of_range("Index out of range");
}

uint8_t NetworkAdapter::findInSubnet(const IPv4Address& other) const {
    for (uint8_t i = 0; i < numInterfaces; i++)
        if (interfaces[i]->getAddress().isInSameSubnet(other))
            return i;
    throw std::out_of_range("Index out of range");
}

bool NetworkAdapter::hasInterface(const MACAddress& other) const {
    for (uint8_t i = 0; i < numInterfaces; i++)
        if (interfaces[i]->getMacAddress() == other)
            return true;
    return false;
}

bool NetworkAdapter::hasInterface(const IPv4Address& other) const {
    for (uint8_t i = 0; i < numInterfaces; i++)
        if (interfaces[i]->getAddress() == other)
            return true;
    return false;
}

bool NetworkAdapter::hasInterfaceInSubnet(const IPv4Address& other) const {
    for (uint8_t i = 0; i < numInterfaces; i++)
        if (interfaces[i]->getAddress().isInSameSubnet(other))
            return true;

    return false;
}

EthernetInterface& NetworkAdapter::operator[](uint8_t index) const {
    if (index >= numInterfaces) {
        throw std::out_of_range("Index out of range");
    }
    return *interfaces[index];
}

std::ostream& operator<<(std::ostream& os, const NetworkAdapter& adapter) {
    uint8_t lastChange = 0;
    for (uint8_t i=0; i<adapter.interfaceCount();i++) {
        os << "interface ";
        if (adapter[i].getMaxSpeed() >= 1000)
            os << "Gigabit";
        else if (adapter[i].getMaxSpeed() >= 100)
            os << "Fast";

        if (i > 0 && adapter[i].getMaxSpeed() != adapter[i-1].getMaxSpeed())
            lastChange = i;
        
        os << "Ethernet 0/" << (int)(i-lastChange) << "\r\n";
        os << adapter[i];
    }
    return os;
}