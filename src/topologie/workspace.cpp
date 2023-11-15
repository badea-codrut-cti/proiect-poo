/*

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
    for (auto& device : data) {
        std::cout << device["label"];
    }
    return str;
}

Workspace& Workspace::getWorkspace() {
    return workspace;
}

Workspace Workspace::workspace{};

*/