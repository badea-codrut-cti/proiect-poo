#include "workspace.h"
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
    return str;
}

Workspace Workspace::workspace{};

