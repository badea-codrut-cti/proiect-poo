#include "workspace.h"

Workspace::~Workspace() {
    for (Device* dev : devices)
        delete dev;
}

unsigned int Workspace::addDevice(Device* dev) {
    devices.push_back(dev);
    return devices.size()-1;
}

std::string Workspace::WDeviceAddParser(std::string str) {
    
}

Workspace Workspace::workspace{};

