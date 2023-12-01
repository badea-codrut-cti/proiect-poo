#include "workspace.h"
#include "../device/devices/router.h"
#include "../device/devices/end_device.h"
#include "../device/devices/l2switch.h"
#include "./jsonify_data.h"
#include "uiexcept.h"
#include "uihook.h"

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

    json outDevice = deviceToJson(pDevice);

    outDevice["deviceIndex"] = devices.size();
    outDevice["ports"] = json::array();
    outDevice["deviceType"] = deviceType;

    json output = json::object();

    output["success"] = true;
    output["device"] = outDevice;

    addDevice(pDevice);
    return output.dump();
}

std::string Workspace::WDeviceConnectParser(std::string str) {
    json data = json::parse(str);
    
    unsigned long long devIndex1 = data[0]["deviceIndex"], devIndex2 = data[1]["deviceIndex"];
    uint8_t intIndex1 = data[0]["interfaceIndex"], intIndex2 = data[1]["interfaceIndex"];

    devices[devIndex1]->getNetworkAdapter()[intIndex1].
    connect(&devices[devIndex2]->getNetworkAdapter()[intIndex2]);
    return "{'success': true}";
}

std::string Workspace::WOpenDeviceSettings(std::string str) {
    json data = json::parse(str);

    if (data[0]["deviceIndex"].empty())
        throw UIException("Device index not specified.");

    unsigned long long devIndex = data[0]["deviceIndex"];

    if (devIndex >= devices.size())
        throw UIException("Device index out of range.");

    hookSettingsWindow(devices[devIndex]);
    //alter_window.bind("wDeviceAdd",windowDeviceAdd);
    //alter_window.bind("wDeviceConnect", windowDeviceConnect);
    return "";
}

Workspace& Workspace::getWorkspace() {
    return workspace;
}

Workspace Workspace::workspace{};