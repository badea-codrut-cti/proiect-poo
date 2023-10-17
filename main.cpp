#include <iostream>
#include "src/device/device.h"

using namespace std;

int main() {
    MACAddress mac("00:00:00:00:00:02");
    cout << mac << endl;
    mac -= 1; 
    cout << mac << endl;
    mac += 300; // 301
    cout << mac << endl;
    mac -= 210; // 91
    cout << mac << endl;
    mac += 100120;
    cout << mac << endl;
    mac -= 100100;
    cout << mac << endl;
    return 0;
}