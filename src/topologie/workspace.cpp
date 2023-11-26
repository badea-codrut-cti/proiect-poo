#include "workspace.h"
#include "../device/devices/router.h"
#include "../device/devices/end_device.h"
#include "../device/devices/l2switch.h"
#include "uiexcept.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Workspace::~Workspace() {
    for (Device* dev : devices)
        delete dev;
}

unsigned int Workspace::addDevice(Device* dev) {
    devices.push_back(dev);
    return devices.size()-1;
}

std::string Workspace::WDeviceAddParser(std::string str) {
    json data = json::parse(str);
    json device = data[0];
    if (device["deviceType"].empty())
        throw UIParameterException("deviceType");

    std::string deviceType = device["deviceType"].get<std::string>();
    json outDevice = json::object();
    outDevice["deviceIndex"] = devices.size();
    outDevice["ports"] = json::array();

    Device* pDevice{nullptr};

    if (deviceType == "router") {
        pDevice = new Router();
    } else if (deviceType == "pc") {
        pDevice = new EndDevice();
    } else if (deviceType == "l2switch") {
        pDevice = new L2Switch();
    } else throw UIParameterException("deviceType");

    if (pDevice == nullptr)
        throw UIException("Failed allocation of new device.");

    outDevice["deviceType"] = deviceType;

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
            interface["ip"]["address"] = static_cast<const IPv4Address&>(ether.getAddress()).toString();
            interface["ip"]["subnetMask"] = json::object();
            interface["ip"]["subnetMask"]["dotNotation"] = ether.getAddress().getMaskDotNotation().toString();
            interface["ip"]["subnetMask"]["slashNotation"] = ether.getAddress().getMaskSlashNotation();
        }
        outDevice["interfaces"][i] = interface;
    }

    json output = json::object();
    output["success"] = true;
    output["device"] = outDevice;

    addDevice(pDevice);
    return output.dump();
}

std::string Workspace::WDeviceConnectParser(std::string str) {
    json data = json::parse(str);
    
    long long devIndex1 = data[0]["deviceIndex"], devIndex2 = data[1]["deviceIndex"];
    uint8_t intIndex1 = data[0]["interfaceIndex"], intIndex2 = data[1]["interfaceIndex"];

    devices[devIndex1]->getNetworkAdapter()[intIndex1].
    connect(&devices[devIndex2]->getNetworkAdapter()[intIndex2]);
    return "{'success': true}";
}

Workspace& Workspace::getWorkspace() {
    return workspace;
}

Workspace Workspace::workspace{};