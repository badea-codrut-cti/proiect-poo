#include <nlohmann/json.hpp>
#include <sstream>
#include "../device/device.h"
#include "../date/osi/network.h"

using json = nlohmann::json;

json deviceToJson(const Device* pDevice) {
    json outDevice = json::object();

    outDevice["isOn"] = pDevice->getState();
    outDevice["hostname"] = pDevice->getHostname();

    outDevice["interfaces"] = json::array();
    for (uint8_t i = 0; i < pDevice->getNetworkAdapter().interfaceCount(); i++) {
        const EthernetInterface& ether = pDevice->getNetworkAdapter()[i];
        json interface = json::object();
        interface["isOn"] = ether.getState();
        interface["maxSpeed"] = ether.getMaxSpeed();
        interface["speed"] = ether.getSpeed();
        interface["isUnnumbered"] = ether.isUnnumbered();
        interface["macAddress"] = ether.getMacAddress().toString();
        interface["hardwareAddress"] = ether.getBurntInAddress().toString();
        if (!ether.isUnnumbered()) {
            interface["ip"] = json::object();
            interface["ip"]["address"] = static_cast<const IPv4Address&>(ether.getIPv4Address()).toString();
            interface["ip"]["subnetMask"] = json::object();
            interface["ip"]["subnetMask"]["dotNotation"] = ether.getIPv4Address().getMaskDotNotation().toString();
            interface["ip"]["subnetMask"]["slashNotation"] = ether.getIPv4Address().getSubnetMask();
        }
        outDevice["interfaces"][i] = interface;
    }

    return outDevice;
}

json frameToJson(const DataLinkLayer& data) {
    json ret = json::object();

    std::ostringstream l2t{};
    l2t << data.getL2Type();
    ret["l2Type"] = l2t.str();

    ret["macSource"] = data.getMACSource().toString();
    ret["macDestination"] = data.getMACDestination().toString();

    std::ostringstream plt{};
    plt << data.getPayload();
    ret["payload"] = plt.str();

    if (data.getL2Type() != DataLinkLayer::IPV4 && data.getL2Type() != DataLinkLayer::IPV6)
        return ret;

    try {
        auto& packet = dynamic_cast<const NetworkLayerV4&>(data);
        ret["ipSource"] = packet.getIPSource().toString();
        ret["ipDestination"] = packet.getIPDestination().toString();
        ret["TTL"] = packet.getTTL();
        
        std::ostringstream l3p{};
        l3p << packet.getL3Protocol();
        ret["l3Protocol"] = l3p.str();

        ret["data"] = json::object();
        switch (packet.getL3Protocol()) {
            case NetworkLayerV4::ICMP: {
                try {
                    auto icmp = dynamic_cast<const ICMPPayload*>(packet.getPayload());
                    ret["data"]["type"] = icmp->getType();
                    ret["data"]["code"] = icmp->getCode();
                } catch(const std::bad_cast&) {
                    return ret;
                }
                break;
            }

            default: {
                //TODO: account for HTTP packets as well
                return ret;
            }
        }
        return ret;
    } catch (const std::bad_cast&) {
        return ret;
    }
}