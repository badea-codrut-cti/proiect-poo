#include <exception>
#include <string>
#include "datalink.h"
#include "network.h"

class InvalidFrameException : public std::exception {
    protected: 
    std::string msg;

    public:
        explicit InvalidFrameException(std::string message) : msg(std::move(message)) {}
        [[nodiscard]] const char* what() const noexcept override { return msg.c_str(); }
};

class InvalidPayloadException : public InvalidFrameException {
        DataLinkLayer::L2TypeField l2Type{};
        NetworkLayerV4::IPv4ProtocolType l3Type{};

    public:
        explicit InvalidPayloadException(DataLinkLayer::L2TypeField);
        explicit InvalidPayloadException(NetworkLayerV4::IPv4ProtocolType);
};

//TODO: Fix naming scheme, MACAddress is part of the "frame", not the "packet"
class InvalidPacketException : public InvalidFrameException {
    public:
        explicit InvalidPacketException(const SubnetAddressV4&);
        explicit InvalidPacketException(const IPv4Address&);
        explicit InvalidPacketException(const MACAddress&);
};