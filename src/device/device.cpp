#include "./device.h"
#include "./adapter.h"
#include "../date/osi/network.h"
#include "../date/osi/osiexcept.h"
#include <stdexcept>
#include <iostream>
#include <typeinfo>

Device::Device(uint8_t interfaceCount, bool unnumbered, std::string hostname): 
adapter(*this, interfaceCount, unnumbered), hostname(std::move(hostname)) {

}

MACAddress Device::getArpEntryOrBroadcast(const IPv4Address& ip) const {
    auto it = arpCache.find(ip);
    if (it == arpCache.end())
        return MACAddress{MACAddress::broadcastAddress};
    return it->second;
}

MACAddress Device::getNDPEntryOrMulticast(const IPv6Address& ip) const {
    auto it = ndCache.find(ip);
    if (it == ndCache.end()) {
        std::array<uint8_t, MACADDRESS_SIZE> ret{};
        ret[0] = 0x33;
        ret[1] = 0x33;

        for (size_t j = MACADDRESS_SIZE-1; j >= 2; j--) 
            ret[j] = ip.getOctets()[j + (IPV6_SIZE - MACADDRESS_SIZE)];
        
        return MACAddress(ret);
    }
    return it->second;
}

Device::Device(): adapter(*this, 1), hostname(DEFAULT_DEVICE_HOSTNAME) {

}

void Device::setHostname(const std::string& str) {
    hostname = str;
}

bool Device::sendARPRequest(const IPv4Address& target, bool forced) {
    if (!adapter.hasInterfaceInSubnet(target))
        return false;

    if (getArpEntryOrBroadcast(target) != MACAddress{MACAddress::broadcastAddress} && !forced) 
        return true;

    size_t fIndex = adapter.findInSubnet(target);

    ARPIPv4 pl(ARPData::REQUEST, adapter[fIndex].getMacAddress(), 
    MACAddress("00:00:00:00:00:01"), adapter[fIndex].getIPv4Address(), target);
    DataLinkLayer l2(adapter[fIndex].getMacAddress(), MACAddress{MACAddress::broadcastAddress}, pl, DataLinkLayer::ARP);
    NetworkLayerV4 l3(l2, adapter[fIndex].getIPv4Address(), target);
    return adapter[fIndex].sendData(l3);
}

bool Device::sendNDPRequest(const IPv6Address& target, bool forced) {
    if (!adapter.hasInterfaceInSubnet(target))
        return false;

    auto it = ndCache.find(target);
    if (it != ndCache.end() && !forced)
        return true;

    std::array<uint8_t, MACADDRESS_SIZE> ret{};
    ret[0] = 0x33;
    ret[1] = 0x33;

    for (size_t j = MACADDRESS_SIZE-1; j >= 2; j--) 
        ret[j] = target.getOctets()[j + (IPV6_SIZE - MACADDRESS_SIZE)];
        
    size_t fIndex = adapter.findInSubnet(target);

    for (auto& gua : adapter[fIndex].getGlobalUnicastAddresses()) {
        if (!gua.isInSameSubnet(target))
            continue;

        NDPPayload pl(NDPPayload::NEIGHBOR_SOLICITATION, 0, target);
        DataLinkLayer l2(adapter[fIndex].getMacAddress(), MACAddress(ret), pl, DataLinkLayer::IPV6);
        NetworkLayerV6 l3(l2, gua, target);
        return adapter[fIndex].sendData(l3);
    }

    return false;
}

bool Device::sendICMPRequest(const IPv4Address& dest) {
    ICMPPayload pl(ICMPPayload::ECHO_REQUEST, 0);

    bool hitRouter = !adapter[0].getIPv4Address().isInSameSubnet(dest);
    IPv4Address defaultGatewayV4 = adapter[0].getIPv4DefaultGateway();

    if (hitRouter) 
        sendARPRequest(defaultGatewayV4, false);
    else 
        sendARPRequest(dest, false);

    DataLinkLayer l2(adapter[0].getMacAddress(),
    hitRouter ? getArpEntryOrBroadcast(defaultGatewayV4) : getArpEntryOrBroadcast(dest), 
    pl, DataLinkLayer::IPV4);

    NetworkLayerV4 l3(l2, adapter[0].getIPv4Address(), dest, DEFAULT_TTL, NetworkLayerV4::ICMP);
    return adapter[0].sendData(l3);
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
    if (data.getL2Type() != DataLinkLayer::IPV4)
        return false;

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
    if (data.getL2Type() != DataLinkLayer::ARP)
        return false;
    
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
        } else if (isItForMe && payload->getDestinationMacAddress() == MACAddress("00:00:00:00:00:01")) {
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

bool Device::handleNDPRequest(const DataLinkLayer& data, const MACAddress&) {
    if (data.getL2Type() != DataLinkLayer::IPV6)
        return false;

    uint8_t fOct = data.getMACDestination().getOctets()[0],
        sOct = data.getMACDestination().getOctets()[1];

    if (fOct == 0x33 && sOct == 0x33) {
        // Hopefully a neighbor solicitation

        for (size_t i = 0; i < adapter.interfaceCount(); i++) {
            for (auto& addr : adapter[i].getGlobalUnicastAddresses()) {
                bool ok = true;
                for (size_t j = MACADDRESS_SIZE-1; j >= 2 && ok; j--) 
                    if (addr.getOctets()[j + (IPV6_SIZE - MACADDRESS_SIZE)] != 
                        data.getMACDestination().getOctets()[j])
                        ok = false;
                    
                if (!ok)
                    continue;

                // It most likely is

                try {
                    auto ndpPayload = dynamic_cast<const NDPPayload*>(data.getPayload());
                    
                    if (ndpPayload->getType() != NDPPayload::NEIGHBOR_SOLICITATION) 
                        return false;
                    
                    auto l3data = dynamic_cast<const NetworkLayerV6&>(data);

                    NDPPayload response(NDPPayload::NEIGHBOR_ADVERTISEMENT, 0, addr);
                    DataLinkLayer l2(adapter[i].getMacAddress(), l3data.getMACSource(),
                     response, DataLinkLayer::IPV6);
                    NetworkLayerV6 l3(l2, addr, l3data.getIPSource(), DEFAULT_HOP_LIMIT, NetworkLayerV6::ICMPv6);
                    adapter[i].sendData(l3);

                    return true;
                } catch(const std::bad_cast&) {
                    return false;
                }
            }
        }
    }

    try {
        auto ndpPayload = dynamic_cast<const NDPPayload*>(data.getPayload());

        if (ndpPayload->getType() == NDPPayload::NEIGHBOR_ADVERTISEMENT) {
            ndCache.insert(
                std::make_pair(
                    ndpPayload->getTargetAddress(), data.getMACSource()
                )
            );
            return true;
        }

    } catch (const std::bad_cast&) {
        return false;
    }

    return false;
}

bool Device::receiveData(const DataLinkLayer& data, EthernetInterface& interface) {
    if (!isOn)
        return false;

    if (!adapter.hasInterface(interface.getMacAddress()))
        throw std::invalid_argument("Interface does not belong in the network adapter.");

    if (handleARPRequest(data, interface.getMacAddress())) 
        return true;

    if (checkPingRequest(data, interface.getMacAddress())) 
        return true;

    if (handleNDPRequest(data, interface.getMacAddress()))
        return true;

    for (const auto& listener : listeners) {
        if (listener(data, interface.getMacAddress()))
            return true;
    }

    size_t fIndex = adapter.getIntefaceIndex(interface.getMacAddress());
    
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

size_t Device::registerFuncListener(const std::function<bool(const DataLinkLayer&, const MACAddress&)> &func) {
    listeners.push_back(func);
    return listeners.size()-1;
}

bool Device::removeFuncListener(size_t index) {
    if (index >= listeners.size())
        throw std::out_of_range("Index out of listeners range.");

    listeners.erase(listeners.begin() + index);
    return true;
}

std::ostream& operator<<(std::ostream& os, const Device& device) {
    os << "Status: " << (device.getState() ? "on" : "off") << "\r\n";
    os << "Hostname: " << device.getHostname() << "\r\n";
    return os;
}