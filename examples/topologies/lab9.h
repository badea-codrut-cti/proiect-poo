#include "../../src/device/devices/router.h"
#include "../../src/device/devices/l2switch.h"
#include "../../src/device/devices/end_device.h"

void lab9() {
    Router r_liban, r_liberia;
    L2Switch sw_liban, sw_liberia;
    EndDevice liban, liberia;

    liban.getNetworkAdapter()[0]
    .addGlobalUnicastAddress(SubnetAddressV6(IPv6Address("2001:ABCD:ACAD::2"), 64));

    liban.getNetworkAdapter()[0].connect(&sw_liban.getNetworkAdapter()[0]);
    sw_liban.getNetworkAdapter()[1].connect(&r_liban.getNetworkAdapter()[0]);

    r_liban.getNetworkAdapter()[0]
    .addGlobalUnicastAddress(SubnetAddressV6(IPv6Address("2001:ABCD:ACAD::1"), 64));

    r_liban.getNetworkAdapter()[1]
    .addGlobalUnicastAddress(SubnetAddressV6(IPv6Address("2001:AABB:ACAD::1"), 64));

    r_liberia.getNetworkAdapter()[1]
    .addGlobalUnicastAddress(SubnetAddressV6(IPv6Address("2001:AABB:ACAD::2"), 64));

    r_liberia.getNetworkAdapter()[0]
    .addGlobalUnicastAddress(SubnetAddressV6(IPv6Address("2001:AACC:ACAD::1"), 64));

    r_liban.getNetworkAdapter()[1].connect(&r_liberia.getNetworkAdapter()[1]);

    liberia.getNetworkAdapter()[0]
    .addGlobalUnicastAddress(SubnetAddressV6(IPv6Address("2001:AACC:ACAD::2"), 64));

    liberia.getNetworkAdapter()[0].connect(&sw_liberia.getNetworkAdapter()[0]);

    sw_liberia.getNetworkAdapter()[0].connect(&r_liberia.getNetworkAdapter()[0]);
}