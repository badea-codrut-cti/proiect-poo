#include <cstring>
#include <iostream>
#include "src/device/device.h"
#include "src/device/devices/l2switch.h"
#include "src/date/subnetaddress.h"
#include "src/date/osi/network.h"
#include "src/protocoale/arp.h"
#include "src/protocoale/payload.h"

using namespace std;

int main() {
    Device brasov, bon;
    L2Switch sw_brasov, sw_bon;
    brasov.getNetworkAdapter()[0].connect(&sw_brasov.getNetworkAdapter()[0]);
    sw_brasov.getNetworkAdapter()[1].connect(&sw_bon.getNetworkAdapter()[0]);
    sw_bon.getNetworkAdapter()[1].connect(&bon.getNetworkAdapter()[0]);
    brasov.getNetworkAdapter()[0].setIpAddress({{"192.168.1.2"}, 24});
    cout << brasov.getNetworkAdapter()[0];
    bon.getNetworkAdapter()[0].setIpAddress({{"192.168.1.3"}, 22});

    brasov.getNetworkAdapter()[0].sendARPRequest({"192.168.1.3"});
    for (auto& arp : brasov.getARPCache()) {
        cout << arp.first << " " << arp.second << endl;
    }
    cout << "====" << endl;
    bon.getNetworkAdapter()[0].sendARPRequest({"192.168.1.2"});
    for (auto& arp : bon.getARPCache()) {
        cout << arp.first << " " << arp.second << endl;
    }
    cout << "====" << endl;
    L2Payload pl;
    DataLinkLayer frame(brasov.getNetworkAdapter()[0].getMacAddress(),
    bon.getNetworkAdapter()[0].getMacAddress(), pl, DataLinkLayer::IPV4);
    brasov.getNetworkAdapter()[0].sendData(frame);
    return 0;
}