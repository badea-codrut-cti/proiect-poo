#include "workspace.h"
#include "../device/devices/router.h"
#include "../device/devices/end_device.h"
#include "../device/devices/l2switch.h"
#include "./jsonify_data.h"
#include "uiexcept.h"
#include "device_factory.h"
#include <cstdint>
#include <stdexcept>

using json = nlohmann::json;

Workspace::~Workspace() {
    for (auto dev : devices)
        delete dev;
}

uint64_t Workspace::addDevice(Device* dev) {
    devices.push_back(dev);
    return devices.size();
}

json Workspace::WDeviceAddParser(json data) {
    json device = data[0];

    if (device["deviceType"].empty())
        throw UIParameterException("deviceType");

    std::string deviceType = device["deviceType"].get<std::string>();

    DeviceFactory factory;

    Device* pDevice = factory.setType(deviceTypeFromString(deviceType)).create();

    if (pDevice == nullptr)
        throw UIException("Failed allocation of new device.");

    json outDevice = deviceToJson(pDevice);

    outDevice["deviceIndex"] = devices.size();
    outDevice["ports"] = json::array();
    outDevice["deviceType"] = deviceType;

    json output = json::object();

    output["success"] = true;
    output["device"] = outDevice;

    addDevice(pDevice);
    return output;
}

bool Workspace::WDeviceConnectParser(json data) {
    unsigned long long devIndex1 = data[0]["deviceIndex"], devIndex2 = data[1]["deviceIndex"];
    uint8_t intIndex1 = data[0]["interfaceIndex"], intIndex2 = data[1]["interfaceIndex"];

    devices[devIndex1]->getNetworkAdapter()[intIndex1].
    connect(&devices[devIndex2]->getNetworkAdapter()[intIndex2]);
    return true;
}

uint64_t Workspace::WOpenDeviceSettings(json data) {
    if (data[0]["deviceIndex"].empty())
        throw UIException("Device index not specified.");

    unsigned long long devIndex = data[0]["deviceIndex"];

    if (devices.size() <= devIndex)
        throw UIException("Device index out of range.");

    return devIndex;
}

bool Workspace::WToggleDeviceState(json data) {
    unsigned long long devIndex = data[0];

    if (devices.size() <= devIndex)
        throw UIParameterException("deviceIndex");
    
    if (devices[devIndex]->getState())
        devices[devIndex]->turnOff();
    else 
        devices[devIndex]->turnOn();

    return true;
}

bool Workspace::changeDeviceSettings(uint64_t index, json data) {
    if (devices.size() <= index)
        throw UIException("Index out of range in hook function.");

    Device* dev = devices[index];

    if (dev == nullptr)
        throw UIException("Device was freed prior to edit.");

    if (!dev->getState())
        throw UIStateException(*dev);

    std::string editMode = data["editMode"];
    if (editMode == "device") {
        if (!data["hostname"].empty()) {
            dev->setHostname(data["hostname"]);
        }

        return true;
    } else if (editMode == "interface") {
        unsigned long long interfaceIndex = data["interfaceIndex"];

        if (dev->getNetworkAdapter().interfaceCount() <= interfaceIndex)
            throw UIParameterException("interfaceIndex");

        EthernetInterface& interf = dev->getNetworkAdapter()[interfaceIndex];

        if (!data["ip"].empty()) {
            if (interf.isUnnumbered())
                throw UIException("Cannot assign IP address to unnumbered interface");

            if (!data["ip"]["address"].empty()) {
                IPv4Address newIp = IPv4Address(
                    data["ip"]["address"].get<std::string>()
                );
                
                if (!data["subnetMask"].empty()) {
                   if (!data["subnetMask"]["slashNotation"].empty()) {
                        try {
                            interf.setIpAddress(
                                SubnetAddress(
                                    newIp,
                                    data["subnetMask"]["slashNotation"]
                                )
                            );
                        } catch(const std::invalid_argument&) {
                            throw UIParameterException("subnetMask");
                        }
                   }
                   else if (!data["subnetMask"]["dotNotation"].empty()) {
                        try {
                            std::string dotNot = data["subnetMask"]["dotNotation"];
                            interf.setIpAddress(
                                SubnetAddress(
                                    newIp,
                                    SubnetAddress::dotMaskToCIDR(IPv4Address(dotNot))
                                )
                            );
                        } catch(const std::invalid_argument&) {
                            throw UIParameterException("subnetMask");
                        }
                   }
                } else {
                    try {
                        interf.setIpAddress(SubnetAddress(newIp));
                    } catch(const std::invalid_argument&) {
                        throw UIParameterException("ipAddress");
                    }
                }

            }
        }

        if (!data["isOn"].empty()) {
            if (data["isOn"])
                interf.turnOn();
            else
                interf.turnOff();
        }

        if (!data["macAddress"].empty()) {
            try {
                interf.setMacAddress(
                    MACAddress(data["macAddress"].get<std::string>())
                );
            } catch(const std::invalid_argument&) {
                throw UIParameterException("macAddress");
            }
        }

        if (!data["speed"].empty()) {
            try {
                interf.setSpeed(data["speed"]);
            } catch(const std::invalid_argument&) {
                throw UIParameterException("speed");
            }
        }

        return true;
    } else throw UIParameterException("editMode");
}

Workspace& Workspace::getWorkspace() {
    return workspace;
}

Device* Workspace::getDevice(uint64_t devId) {
    if (devId > devices.size())
        throw UIParameterException("devId");

    return devices[devId];
}

Workspace Workspace::workspace{};