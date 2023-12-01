#include <exception>
#include <string>
#include "datalink.h"
#include "network.h"
#include "../subnetaddress.h"

class InvalidFrameException : public std::exception {
    protected: 
    std::string msg;

    public:
        explicit InvalidFrameException(std::string message) : msg(std::move(message)) {}
        [[nodiscard]] const char* what() const noexcept override { return msg.c_str(); }
};

class InvalidPayloadException : public InvalidFrameException {
        enum ILayerType : uint8_t {
            DataLink,
            Network
        };

        DataLinkLayer::L2TypeField l2Type{};
        NetworkLayer::IPProtocolType l3Type{};

        ILayerType msgType{};
    public:
        explicit InvalidPayloadException(DataLinkLayer::L2TypeField);
        explicit InvalidPayloadException(NetworkLayer::IPProtocolType);
};

//TODO: Fix naming scheme, MACAddress is part of the "frame", not the "packet"
class InvalidPacketException : public InvalidFrameException {
    public:
        explicit InvalidPacketException(const SubnetAddress&);
        explicit InvalidPacketException(const IPv4Address&);
        explicit InvalidPacketException(const MACAddress&);
};