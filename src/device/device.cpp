#include "./device.h"
#include "./adapter.h"
#include "../date/osi/network.h"
#include <stdexcept>

Device::Device(uint8_t interfaceCount): adapter(*this, interfaceCount) {

}

MACAddress Device::getArpEntryOrBroadcast(const IPv4Address& ip) {
    auto it = arpCache.find(ip);
    if (it == arpCache.end())
        return MACAddress::broadcastAddress;
    return it->second;
}

Device::Device(): adapter(*this, 1) {
    
}

void Device::turnOn() {
    isOn = true;
}

void Device::turnOff() { 
    arpCache.clear();
    isOn = false;
}

bool Device::getState() const { 
    return isOn;
}

bool Device::interfaceCallback([[maybe_unused]]DataLinkLayer& _data, [[maybe_unused]]uint8_t index) {
    return true;
}

bool Device::receiveData(DataLinkLayer& _data, EthernetInterface& _interface) {
    if (!isOn)
        return false;

    try {
        auto cast = dynamic_cast<NetworkLayer&>(_data);
        arpCache.insert(std::make_pair(cast.getIPSource(), cast.getMACSource()));
    } catch(const std::bad_cast& e) {}

    try {
        int fIndex = adapter.getIntefaceIndex(_interface.getMacAddress());
        return interfaceCallback(_data, fIndex);
    } catch(const std::out_of_range& e) {
        throw std::invalid_argument("Interface does not belong in the network adapter.");
    }
}

/*NetworkAdapter Device::getNetworkAdapter() {
    return adapter;
}

std::ostream& operator<<(std::ostream& os, Device& device) {
    uint8_t count = device.adapter.interfaceCount();
    for (uint8_t i = 0; i < count; i++) {
        EthernetInterface ip = device.adapter[i];

        os << std::string("interface ");
        if (ip.getSpeed() >= 1000)
            os << std::string("Gigabit");
        else if (ip.getSpeed() >= 100)
            os << std::string("Fast");
        os << std::string("Ethernet ");

        if (i < count / 2)
            os << std::to_string(i) << std::string("/0");
        else
            os << std::to_string(i - count / 2) << std::string("/1");

        os << std::string("\r\n!\r\n");
        os << (*device.adapter)[i];
    }
    return os;
}*/