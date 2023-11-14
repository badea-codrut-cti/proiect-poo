#include "address.h"
#include "device.h"
#include "./topologies/labs.h"

int main() {
    test_macaddress();
    test_ipv4();
    arp();
    clone();
    labs();
    return 0;
}