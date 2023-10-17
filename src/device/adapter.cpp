#include "./adapter.h"
#include <stdexcept>

NetworkAdapter::NetworkAdapter(Device& device, uint8_t numInterfaces): 
numInterfaces(numInterfaces), interfaces() {
    interfaces = new EthernetInterface*[numInterfaces];
    for (uint8_t i=0;i < numInterfaces; i++)
        interfaces[i] = new EthernetInterface(device);
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

EthernetInterface& NetworkAdapter::operator[](uint8_t index) {
    if (index >= numInterfaces) {
        throw std::out_of_range("Index out of range");
    }
    return *(interfaces[index]);
}