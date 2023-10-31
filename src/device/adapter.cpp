#include "./adapter.h"
#include <ostream>
#include <stdexcept>
#include <iostream>

NetworkAdapter::NetworkAdapter(Device& device, uint8_t _numInterfaces, bool passive): 
numInterfaces(_numInterfaces) {
    interfaces = new EthernetInterface*[_numInterfaces];
    for (uint8_t i=0;i < numInterfaces; i++)
        interfaces[i] = new EthernetInterface(device, passive);
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
    for (uint8_t i=0; i<adapter.interfaceCount();i++) {
        os << "interface ";
        if (adapter[i].getSpeed() >= 1000)
            os << "Gigabit";
        else if (adapter[i].getSpeed() >= 100)
            os << "Fast";
        
        os << "Ethernet 0/" << (int)i << "\r\n";
        os << adapter[i];
    }
    return os;
}