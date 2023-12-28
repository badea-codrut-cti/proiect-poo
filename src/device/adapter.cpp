#include "./adapter.h"
#include <ostream>
#include <stdexcept>
#include <iostream>

NetworkAdapter::NetworkAdapter(Device& device, size_t intCount, bool passive): 
numInterfaces(intCount) {
    interfaces = new EthernetInterface*[intCount];
    for (size_t i=0;i < numInterfaces; i++)
        interfaces[i] = new EthernetInterface(device, GIGABIT_ETHERNET, passive);
}

NetworkAdapter::NetworkAdapter(Device& device, size_t intCount, std::function<EthernetInterface*(Device&, size_t)> func):
numInterfaces(intCount) {
    interfaces = new EthernetInterface*[intCount];
    for (size_t i=0;i < numInterfaces; i++)
        interfaces[i] = func(device, i);
}

bool NetworkAdapter::copy(Device& dev, const NetworkAdapter& other) {
    if (this == &other) {
        // Self assignment would return in making everything a nullptr
        // I have no idea when we will need to swap the card owner to another device
        for (size_t i = 0; i < numInterfaces; i++) {
            EthernetInterface* interface = interfaces[i]->copy(dev);
            delete interfaces[i];
            interfaces[i] = interface;
        }
        return true;
    }

    for (size_t i = 0; i < numInterfaces; i++)
        delete interfaces[i];

    delete[] interfaces;

    numInterfaces = other.numInterfaces;
    interfaces = new EthernetInterface*[numInterfaces];

    for (size_t i = 0; i < numInterfaces; i++) {
        interfaces[i] = other.interfaces[i]->copy(dev);
    }

    return true;
}

NetworkAdapter::~NetworkAdapter() {
    for (size_t i=0;i < numInterfaces; i++)
        delete interfaces[i];
    
    delete[] interfaces;
}

size_t NetworkAdapter::interfaceCount() const {
    return numInterfaces;
}

size_t NetworkAdapter::getIntefaceIndex(const MACAddress& mac) const {
    for (size_t i = 0; i < numInterfaces; i++)
        if (interfaces[i]->getMacAddress() == mac)
            return i;
    throw std::out_of_range("Index out of range");
}

size_t NetworkAdapter::getIntefaceIndex(const IPv4Address& other) const {
    for (size_t i = 0; i < numInterfaces; i++)
        if (interfaces[i]->getIPv4Address() == other)
            return i;
    throw std::out_of_range("Index out of range");
}

size_t NetworkAdapter::getIntefaceIndex(const IPv6Address& other) const {
    bool isLinkLocal = other.isLinkLocalAddress();

    for (size_t i = 0; i < numInterfaces; i++) {
        if (isLinkLocal) {
            if (interfaces[i]->getLinkLocalAddress() == other)
                return i;
        } else {
            for (auto& gua : interfaces[i]->getGlobalUnicastAddresses())
                if (other == gua)
                    return i;
        }
    }
    throw std::out_of_range("Index out of range");
}

size_t NetworkAdapter::findInSubnet(const IPv4Address& other) const {
    for (size_t i = 0; i < numInterfaces; i++)
        if (interfaces[i]->getIPv4Address().isInSameSubnet(other))
            return i;
    throw std::out_of_range("Index out of range");
}

size_t NetworkAdapter::findInSubnet(const IPv6Address& other) const {
    bool isLinkLocal = other.isLinkLocalAddress();

    for (size_t i = 0; i < numInterfaces; i++) {
        if (isLinkLocal) {
            if (interfaces[i]->getLinkLocalAddress() == other)
                return i;
        } else {
            for (auto& gua : interfaces[i]->getGlobalUnicastAddresses())
                if (gua.isInSameSubnet(other))
                    return i;
        }
    }
    throw std::out_of_range("Index out of range");
}

bool NetworkAdapter::hasInterfaceInSubnet(const IPv6Address& other) const {
    bool isLinkLocal = other.isLinkLocalAddress();

    for (size_t i = 0; i < numInterfaces; i++) {
        if (isLinkLocal) {
            if (interfaces[i]->getLinkLocalAddress() == other)
                return true;
        } else {
            for (auto& gua : interfaces[i]->getGlobalUnicastAddresses())
                if (gua.isInSameSubnet(other))
                    return true;
        }
    }

    return false;
}

bool NetworkAdapter::hasInterface(const MACAddress& other) const {
    for (size_t i = 0; i < numInterfaces; i++)
        if (interfaces[i]->getMacAddress() == other)
            return true;
    return false;
}

bool NetworkAdapter::hasInterface(const IPv4Address& other) const {
    for (size_t i = 0; i < numInterfaces; i++)
        if (interfaces[i]->getIPv4Address() == other)
            return true;
    return false;
}

bool NetworkAdapter::hasInterfaceInSubnet(const IPv4Address& other) const {
    for (size_t i = 0; i < numInterfaces; i++)
        if (interfaces[i]->getIPv4Address().isInSameSubnet(other))
            return true;

    return false;
}

EthernetInterface& NetworkAdapter::operator[](size_t index) const {
    if (index >= numInterfaces) {
        throw std::out_of_range("Index out of range");
    }
    return *interfaces[index];
}

std::ostream& operator<<(std::ostream& os, const NetworkAdapter& adapter) {
    size_t lastChange = 0;
    for (size_t i=0; i<adapter.interfaceCount();i++) {
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