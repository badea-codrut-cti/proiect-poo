#include "workspace.h"
#include "../device/devices/router.h"
#include "../device/devices/end_device.h"
#include "../device/devices/l2switch.h"
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
        return "{'success': false, 'error': 'Missing deviceType field'}";

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
    }

    if (pDevice == nullptr)
        return "{'success': false, 'error': 'Invalid device type'}";

    outDevice["deviceType"] = deviceType;

    outDevice["isOn"] = pDevice->getState();
    outDevice["hostname"] = pDevice->getHostname();

    outDevice["interfaces"] = json::array();
    for (uint8_t i = 0; i < pDevice->getNetworkAdapter().interfaceCount(); i++) {
        EthernetInterface& ether = pDevice->getNetworkAdapter()[i];
        json interface = json::object();
        interface["isOn"] = ether.getState();
        interface["maxSpeed"] = ether.getMaxSpeed();
        interface["speed"] = ether.getSpeed();
        interface["isUnnumbered"] = ether.isUnnumbered();
        interface["macAddress"] = ether.getMacAddress().toString();
        if (!ether.isUnnumbered()) {
            interface["ip"] = json::object();
            interface["ip"]["address"] = static_cast<const IPv4Address&>(ether.getAddress()).toString();
            interface["ip"]["subnetMask"] = ether.getAddress().getMaskDotNotation().toString();
        }
        outDevice["interfaces"][i] = interface;
    }

    json output = json::object();
    output["success"] = true;
    output["device"] = outDevice;

    devices.push_back(pDevice);
    return output.dump();
}

Workspace& Workspace::getWorkspace() {
    return workspace;
}

Workspace Workspace::workspace{};