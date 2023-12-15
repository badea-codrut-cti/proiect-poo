#include "device_factory.h"
#include "../device/devices/l2switch.h"
#include "../device/devices/end_device.h"
#include "../device/devices/router.h"
#include "../device/devices/sniffer.h"
#include "uiexcept.h"
#include <sstream>
#include <stdexcept>

DeviceFactory& DeviceFactory::setType(DeviceFactory::DeviceType deviceType) {
    this->devType = deviceType;
    return *this;
}

Device* DeviceFactory::create() {
    Device* dev = nullptr;

    switch (devType) {
        case DeviceFactory::l2Switch: {
            dev = new class L2Switch();
            break;
        }

        case DeviceFactory::endDevice: {
            dev = new class EndDevice();
            break;
        }

        case DeviceFactory::router: {
            dev = new class Router();
            break;
        }

        case DeviceFactory::trafficSniffer: {
            dev = new class TrafficSniffer();
            break;
        }

        default: {
            throw std::invalid_argument("Invalid device type.");
        }
    }

    return dev;
}

std::ostream& operator<<(std::ostream& os, DeviceFactory::DeviceType devType) {
    switch (devType) {
        case DeviceFactory::l2Switch: {
            os << "l2switch";
            break;
        }

        case DeviceFactory::endDevice: {
            os << "pc";
            break;
        }

        case DeviceFactory::router: {
            os << "router";
            break;
        }

        case DeviceFactory::trafficSniffer: {
            os << "sniffer";
            break;
        }
    }

    return os;
}

DeviceFactory::DeviceType deviceTypeFromString(const std::string& str) {
    DeviceFactory::DeviceType types[] = {DeviceFactory::l2Switch, DeviceFactory::endDevice, 
    DeviceFactory::router, DeviceFactory::trafficSniffer};

    for (auto type : types) {
        std::ostringstream os;
        os << type;
        if (os.str() == str)
            return type;
    }

    throw UIParameterException("deviceType");
}