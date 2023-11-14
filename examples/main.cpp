#include "address.h"
#include "device.h"

int main() {
    test_macaddress();
    test_ipv4();
    arp();
    clone();
    return 0;
}