#include "address.h"
#include "device.h"
#include "interface.h"
#include "./topologies/labs.h"

int main() {
    test_macaddress();
    test_ipv4();   
    test_device();
    arp();
    clone();
    labs();
    ethernet_interface();
    return 0;
}