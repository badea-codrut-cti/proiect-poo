#include "./icmp.h"
#include <ostream>

ICMPPayload::ICMPPayload(ICMPType plType, uint8_t plCode):
plType(plType), plCode(plCode) {

}

ICMPPayload::ICMPPayload(const ICMPPayload& other): 
plType(other.plType), plCode(other.plCode) {

}

ICMPPayload& ICMPPayload::operator=(const ICMPPayload& other) {
    plType = other.plType;
    plCode = other.plCode;

    return *this;
}

ICMPPayload* ICMPPayload::clone() const {
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

ICMPv6Payload::ICMPv6Payload(uint8_t type, uint8_t code):
plType(type), plCode(code) {

}

ICMPv6Payload::ICMPv6Payload(const ICMPv6Payload& other):
plType(other.plType), plCode(other.plCode) {

}

ICMPv6Payload& ICMPv6Payload::operator=(const ICMPv6Payload& other) {
    plType = other.plType;
    plCode = other.plCode;

    return *this;
}

ICMPv6Payload* ICMPv6Payload::clone() const {
    return new ICMPv6Payload(*this);
}

uint8_t ICMPv6Payload::getType() const {
    return plType;
}

uint8_t ICMPv6Payload::getCode() const {
    return plCode;
}

void ICMPv6Payload::print(std::ostream& o) const {
    o << "ICMPv6 Payload\n";
    o << "Operation: " << (int) plType << "\n";
    o << "Code: " << (int) plCode << "\n";
}

void NDPPayload::print(std::ostream& o) const {
    o << "NDP Payload\n";
    o << "Operation: " << (int) plType << "\n";
    o << "Code: " << (int) plCode << "\n";
     o << "Target address: " << targetAddress << "\n";
}

NDPPayload::NDPPayload(NDPOperation operation, uint8_t code, const IPv6Address& target):
ICMPv6Payload(operation, code), targetAddress(target) {

}

NDPPayload::NDPPayload(const NDPPayload& other):
ICMPv6Payload(other), targetAddress(other.targetAddress) {

}

NDPPayload& NDPPayload::operator=(const NDPPayload& other) {
    ICMPv6Payload::operator=(other);
    targetAddress = other.targetAddress;

    return *this;
}

NDPPayload* NDPPayload::clone() const {
    return new NDPPayload(*this);
}

IPv6Address NDPPayload::getTargetAddress() const {
    return targetAddress;
}
