#include "./icmp.h"
#include <ostream>

ICMPPayload::ICMPPayload(ICMPType plType, uint8_t plCode):
plType(plType), plCode(plCode) {

}

ICMPPayload::ICMPPayload(const ICMPPayload& other): 
plType(other.plType), plCode(other.plCode) {

}

L2Payload* ICMPPayload::clone() const {
    return new ICMPPayload(*this);
}

ICMPPayload::ICMPType ICMPPayload::getType() const {
    return plType;
}

uint8_t ICMPPayload::getCode() const {
    return plCode;
}

void ICMPPayload::print(std::ostream& o) const {
    o << "ICMP Payload\n";
    o << "Type: " << (int) plType << "\n";
    o << "Code: " << (int) plCode << "\n";
}