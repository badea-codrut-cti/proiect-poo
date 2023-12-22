#include "address.h"
#include "device.h"
#include "interface.h"
#include "./topologies/labs.h"
#include "../src/date/macaddress.h"
#include <iostream>

int main() {
    test_macaddress();
    test_ipv4();   
    test_ipv6();
    test_device();
    arp();
    clone();
    labs();
    ethernet_interface();
    return 0;
}