#include <iostream>
#include "src/date/subnetaddress.h"

using namespace std;

int main() {
    SubnetAddress add({"192.168.1.1"}, 24);
    cout << add;
}