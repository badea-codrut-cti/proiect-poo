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

void NDPPayload::print(std::ostream& o) const {
    o << "NDP Payload\n";
    o << "Operation: " << (int) ndpOperation << "\n";
    o << "Target address: " << targetAddress << "\n";
    o << "Code: " << (int) ndpCode << "\n";
}

NDPPayload::NDPPayload(NDPOperation operation, uint8_t code, const IPv6Address& target):
ndpOperation(operation), ndpCode(code), targetAddress(target) {

}

NDPPayload::NDPPayload(const NDPPayload& other):
ndpOperation(other.ndpOperation), ndpCode(other.ndpCode), 
targetAddress(other.targetAddress) {

}

NDPPayload& NDPPayload::operator=(const NDPPayload& other) {
    ndpOperation = other.ndpOperation;
    ndpCode = other.ndpCode;
    targetAddress = other.targetAddress;

    return *this;
}

NDPPayload* NDPPayload::clone() const {
    return new NDPPayload(*this);
}

NDPPayload::NDPOperation NDPPayload::getOperation() const {
    return ndpOperation;
}

uint8_t NDPPayload::getCode() const {
    return ndpCode;
}

IPv6Address NDPPayload::getTargetAddress() const {
    return targetAddress;
}
