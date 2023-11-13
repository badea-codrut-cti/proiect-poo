#include "./device.h"
#include "./adapter.h"
#include "../date/osi/network.h"
#include "../protocoale/arp.h"
#include <stdexcept>
#include <utility>
#include <iostream>

Device::Device(uint8_t interfaceCount, bool unnumbered, std::string hostname): 
adapter(*this, interfaceCount, unnumbered), hostname(std::move(hostname)) {

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

bool Device::sendARPRequest(const IPv4Address& target, bool forced) {
    for (uint8_t i = 0; i < adapter.interfaceCount(); i++) {
        if (!adapter[i].getAddress().isInSameSubnet(target))
            continue;

        if (getArpEntryOrBroadcast(target) != MACAddress{MACAddress::broadcastAddress} && !forced) 
            return true;
        
        ARPPayload pl = ARPParser::createARPRequest(adapter[i].getMacAddress(), 
        adapter[i].getAddress(), target);
        DataLinkLayer l2(adapter[i].getMacAddress(), MACAddress{MACAddress::broadcastAddress}, pl, DataLinkLayer::ARP);
        NetworkLayer l3(l2, adapter[i].getAddress(), target);
        return adapter[i].sendData(l3);
    }

    //throw std::invalid_argument("Cannot find interface with IP in the same subnet as ARP destination.");
    return false;
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

        if (payload.getHardwareType() != ARPPayload::ETHERNET || payload.getProtocolType() != ARPPayload::IPV4)
            return false;

        auto [hwSrc, hwDest, protoSrc, protoDest] = ARPParser::parseARPPayload(payload);

        bool isItForMe = adapter.hasInterface(protoDest);

        if (payload.getOperation() == ARPPayload::REPLY) {
            arpCache.insert(std::make_pair(protoSrc, hwSrc));
            if (isItForMe)
            return isItForMe;
        } else if (isItForMe) {
            MACAddress destMAC = adapter[adapter.getIntefaceIndex(IPv4Address{protoDest})].getMacAddress();
            ARPPayload l2pl = ARPParser::createARPReply(destMAC, hwSrc, protoDest, protoSrc);
            DataLinkLayer l2(destMAC, hwSrc, l2pl, DataLinkLayer::ARP);
            NetworkLayer l3(l2, protoDest, protoSrc);
            adapter[fIndex].sendData(l3);
            return true;
        }

    } catch(const std::bad_cast&) {
        throw std::invalid_argument("ARP Request does not contain valid payload.");
    }    
    return false;
}

bool Device::receiveData(DataLinkLayer& _data, EthernetInterface& _interface) {
    if (!isOn)
        return false;

    if (!adapter.hasInterface(_interface.getMacAddress()))
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

Device::Device(const Device& other):
isOn(other.isOn), 
adapter(*this, other.adapter.interfaceCount(), other.adapter[0].isUnnumbered()),
hostname(other.hostname) {
    for (uint8_t i = 0; i < adapter.interfaceCount(); i++) {
        adapter[i].setSpeed(other.adapter[i].getSpeed());
        if (!other.adapter[i].getState())
            adapter[i].turnOff();
    }
}

Device* Device::clone() {
    return new Device(*this);
}

std::ostream& operator<<(std::ostream& os, const Device& device) {
    os << "Status: " << (device.getState() ? "on" : "off") << "\r\n";
    os << "Hostname: " << device.getHostname() << "\r\n";
    return os;
}