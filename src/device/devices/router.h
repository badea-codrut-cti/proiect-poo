#include "../device.h"

// "Ce v-am zis eu ca tot ce e gresit se intoarce in zero?"

const uint8_t ROUTER_INT_COUNT = 4;

const std::string DEFAULT_ROUTER_HOSTNAME = "Router";

class Router: public Device {
    private:
        std::map<SubnetAddress, IPv4Address> routes{};

        int findRoute(const IPv4Address&);

    public:
        Router();

        bool interfaceCallback(DataLinkLayer&, uint8_t) override;

        bool addStaticRoute(const SubnetAddress&, const IPv4Address&);

        bool removeStaticRoute(const SubnetAddress&, const IPv4Address&);
};