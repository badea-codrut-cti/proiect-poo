#include "./device.h"
#include "./adapter.h"
#include "../date/osi/network.h"
#include "../date/osi/osiexcept.h"
#include <stdexcept>
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
        if (!adapter[i].getIPv4Address().isInSameSubnet(target))
            continue;

        if (getArpEntryOrBroadcast(target) != MACAddress{MACAddress::broadcastAddress} && !forced) 
            return true;
    
        ARPIPv4 pl(ARPData::REQUEST, adapter[i].getMacAddress(), 
        MACAddress("00:00:00:00:00:01"), adapter[i].getIPv4Address(), target);
        DataLinkLayer l2(adapter[i].getMacAddress(), MACAddress{MACAddress::broadcastAddress}, pl, DataLinkLayer::ARP);
        NetworkLayerV4 l3(l2, adapter[i].getIPv4Address(), target);
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

bool Device::interfaceCallback([[maybe_unused]]const DataLinkLayer& _data, [[maybe_unused]]uint8_t index) {
    return true;
}

bool Device::checkPingRequest(const DataLinkLayer& data, const MACAddress& mac) {
    try {
        auto& packet = dynamic_cast<const NetworkLayerV4&>(data);
        if (packet.getL3Protocol() != NetworkLayerV4::ICMP)
            return false;

        if (!adapter.hasInterface(packet.getIPDestination()))
            return false;

        try {
            auto payload = dynamic_cast<const ICMPPayload*>(data.getPayload());

            if (payload->getType() != ICMPPayload::ECHO_REQUEST)
                return false;

            return handlePingRequest(packet, mac);
        } catch(const std::bad_cast&) {
            throw InvalidPayloadException(NetworkLayerV4::ICMP);
        }
    } catch(const std::bad_cast&) {
        // Why would we be passed l2 here?
        return false;
    }
}

bool Device::handlePingRequest(const NetworkLayerV4& packet, const MACAddress& mac) {
    ICMPPayload pl(ICMPPayload::ECHO_REPLY, 0);
    DataLinkLayer l2(packet.getMACDestination(), packet.getMACSource(), pl, DataLinkLayer::IPV4);
    NetworkLayerV4 l3(l2, packet.getIPDestination(), packet.getIPSource(), DEFAULT_TTL, NetworkLayerV4::ICMP);
    adapter[adapter.getIntefaceIndex(mac)].sendData(l3);
    return true;
}

bool Device::handleARPRequest(const DataLinkLayer& data, const MACAddress& mac) {
    try {
        auto payload = dynamic_cast<const ARPIPv4*>(data.getPayload());

        bool isItForMe = adapter.hasInterface(payload->getDestinationIPv4Address());

        if (payload->getOperation() == ARPData::REPLY) {
            arpCache.insert(std::make_pair(
                payload->getSourceIPv4Address(), 
                payload->getSourceMacAddress()
            ));

            if (isItForMe)
            return isItForMe;
        } else if (isItForMe) {
            MACAddress destMAC = adapter[adapter.getIntefaceIndex(payload->getDestinationIPv4Address())].getMacAddress();
            ARPIPv4 l2pl(ARPData::REPLY, destMAC, payload->getSourceMacAddress(), 
            payload->getDestinationIPv4Address(), payload->getSourceIPv4Address());
            DataLinkLayer l2(destMAC, payload->getSourceMacAddress(), l2pl, DataLinkLayer::ARP);
            NetworkLayerV4 l3(l2, payload->getDestinationIPv4Address(), payload->getSourceIPv4Address());
            adapter[adapter.getIntefaceIndex(mac)].sendData(l3);
            return true;
        }

    } catch(const std::bad_cast&) {
        throw InvalidPayloadException(DataLinkLayer::ARP);
    }    
    return false;
}

bool Device::receiveData(const DataLinkLayer& data, EthernetInterface& interface) {
    if (!isOn)
        return false;

    if (!adapter.hasInterface(interface.getMacAddress()))
        throw std::invalid_argument("Interface does not belong in the network adapter.");

    if (data.getL2Type() == DataLinkLayer::ARP && handleARPRequest(data, interface.getMacAddress())) { 
        return true;
    }

    if (data.getL2Type() == DataLinkLayer::IPV4 && checkPingRequest(data, interface.getMacAddress())) {
        return true;
    }

    for (const auto& listener : listeners) {
        if (listener(data, interface.getMacAddress()))
            return true;
    }

    int fIndex = adapter.getIntefaceIndex(interface.getMacAddress());
    
    return interfaceCallback(data, fIndex);
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
adapter(*this, 1, false),
hostname(other.hostname) {
    adapter.copy(*this, other.adapter);
}

Device* Device::clone() const {
    return new Device(*this);
}

unsigned long long Device::registerFuncListener(const std::function<bool(const DataLinkLayer&, const MACAddress&)> &func) {
    listeners.push_back(func);
    return listeners.size()-1;
}

std::ostream& operator<<(std::ostream& os, const Device& device) {
    os << "Status: " << (device.getState() ? "on" : "off") << "\r\n";
    os << "Hostname: " << device.getHostname() << "\r\n";
    return os;
}