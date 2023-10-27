#include "./device.h"
#include "./adapter.h"
#include "../date/osi/network.h"
#include "../protocoale/arp.h"
#include <stdexcept>
#include <utility>
#include <iostream>

Device::Device(uint8_t interfaceCount, bool unnumbered, std::string _hostname): 
adapter(*this, interfaceCount, unnumbered), hostname(std::move(_hostname)) {

}

MACAddress Device::getArpEntryOrBroadcast(const IPv4Address& ip) {
    auto it = arpCache.find(ip);
    if (it == arpCache.end())
        return MACAddress{MACAddress::broadcastAddress};
    return it->second;
}

Device::Device(): adapter(*this, 1), hostname(DEFAULT_DEVICE_HOSTNAME) {

}

void Device::setHostname(const std::string& str) {
    hostname = str;
}

bool Device::sendARPRequest(const IPv4Address& _add) {
    for (uint8_t i = 0; i < adapter.interfaceCount(); i++) {
        if (!adapter[i].getAddress().isInSameSubnet(_add))
            continue;

        ARPPayload pl = ARPPayload::createARPRequest(adapter[i].getMacAddress(), 
        adapter[i].getAddress(), _add);
        DataLinkLayer l2(adapter[i].getMacAddress(), MACAddress{MACAddress::broadcastAddress}, pl, DataLinkLayer::ARP);
        NetworkLayer l3(l2, adapter[i].getAddress(), _add);
        return adapter[i].sendData(l3);
    }

    throw std::invalid_argument("Cannot find interface with IP in the same subnet as ARP destination.");
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

bool Device::handleARPRequest(DataLinkLayer& _data, uint8_t fIndex) {
    if (_data.getL2Type() != DataLinkLayer::ARP) 
        return false;

    try {
        auto payload = dynamic_cast<ARPPayload&>(_data.getPayload());

        std::array<uint8_t, MACADDRESS_SIZE> hwSrc{};
        std::array<uint8_t, MACADDRESS_SIZE> hwDest{};

        for (uint8_t i = 0; i < MACADDRESS_SIZE; i++) {
            hwSrc[i] = payload.getSourceHardwareAddress()[i];
            hwDest[i] = payload.getDestinationHardwareAddress()[i];
        }

        std::array<uint8_t, IPV4_SIZE> protoSrc{};
        std::array<uint8_t, IPV4_SIZE> protoDest{};

        for (uint8_t i = 0; i < IPV4_SIZE; i++) {
            protoSrc[i] = payload.getSourceProtocolAddress()[i];
            protoDest[i] = payload.getDestinationProtocolAddress()[i];
        }

        bool isItForMe = adapter.findInterface(IPv4Address{protoDest});

        if (payload.getOperation() == ARPPayload::REPLY) {
            arpCache.insert(std::make_pair(protoSrc, hwSrc));
            return isItForMe;
        } else if (isItForMe) {
            int fIndex = adapter.getIntefaceIndex(IPv4Address{protoDest});
            EthernetInterface& interface = adapter[fIndex];
            ARPPayload payload(ARPPayload::REPLY, interface.getMacAddress().getOctets(),
             hwSrc, protoDest, protoSrc);
            DataLinkLayer l2(interface.getMacAddress(), MACAddress{hwSrc}, payload, DataLinkLayer::ARP);
            NetworkLayer l3(l2, IPv4Address{protoDest}, IPv4Address{protoSrc});
            interface.sendData(l3);
            return true;
        }

    } catch([[maybe_unused]]const std::bad_cast& e) {
        throw std::invalid_argument("ARP Request does not contain valid payload");
    }    
    return false;
}

bool Device::receiveData(DataLinkLayer& _data, EthernetInterface& _interface) {
    if (!isOn)
        return false;

    if (!adapter.findInterface(_interface.getMacAddress()))
        throw std::invalid_argument("Interface does not belong in the network adapter.");
    
    int fIndex = adapter.getIntefaceIndex(_interface.getMacAddress());

    // TODO: This should be adapted to each device's interfaceCallback override
    // Some devices will have VLANs and passive interfaces (i.e switches), thus
    // using `adapter.getInterfaceIndex` is a mistake
    if (handleARPRequest(_data, fIndex))
        return true;

    return interfaceCallback(_data, fIndex);
}

const std::map<IPv4Address, MACAddress>& Device::getARPCache() const {
    return arpCache;
}

const NetworkAdapter& Device::getNetworkAdapter() const {
    return adapter;
}

const std::string& Device::getHostname() const {
    return hostname;
}

std::ostream& operator<<(std::ostream& os, const Device& device) {
    os << "Status: " << (device.getState() ? "on" : "off") << "\r\n";
    os << "Hostname: " << device.getHostname() << "\r\n";
    return os;
}