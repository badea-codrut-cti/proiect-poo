#include <cstdint>
#include <stdexcept>
#include "../date/ipv4.h"
#include "../date/macaddress.h"
#include "./payload.h"

class ARPPayload : public L2Payload {
    public:
        enum HardwareType : uint16_t {
            ETHERNET    = 1,
            TOKENRING   = 6,
            FRAMERELAY  = 15,
            INFINIBAND  = 32,
            FIBRE       = 56
        };

        enum ProtocolType : uint16_t {
            IPV4      = 0x0800,
            IPV6      = 0x86DD,
            IPX       = 0x8137,
            APPLETALK = 0x809B,
            DECNET    = 0x6003
        };

        enum Operation : uint16_t {
            REQUEST = 1,
            REPLY = 2
        };

    private: 
        HardwareType hardwareType;
        ProtocolType protocolType;
        Operation operation;
        uint8_t hwAddLength, protoAddLength;
        uint8_t *sourceHardwareAddress, *destinationHardwareAddress,
        *sourceProtocolAddress, *destinationProtocolAddress;

    public:
        template <std::size_t N, std::size_t M>
        ARPPayload(Operation _operation, 
        const std::array<uint8_t, N>& _srcHwAddress,
        const std::array<uint8_t, N>& _destHwAddress,
        const std::array<uint8_t, M>& _srcProtoAddress,
        const std::array<uint8_t, M>& _destProtoAddress, 
        HardwareType _hwType = ETHERNET,
        ProtocolType _protoType = IPV4
        );

        ARPPayload(const ARPPayload&);

        ARPPayload& operator=(const ARPPayload&);

        ~ARPPayload() override;

        [[nodiscard]] Operation getOperation() const;

        [[nodiscard]] const uint8_t* getSourceHardwareAddress() const;

        [[nodiscard]] const uint8_t* getDestinationHardwareAddress() const;

        [[nodiscard]] const uint8_t* getSourceProtocolAddress() const;

        [[nodiscard]] const uint8_t* getDestinationProtocolAddress() const;

        [[nodiscard]] HardwareType getHardwareType() const;

        [[nodiscard]] ProtocolType getProtocolType() const;
};  

class ARPParser {
    public:
        static ARPPayload createARPRequest(const MACAddress&, const IPv4Address&, const IPv4Address&);
        static ARPPayload createARPReply(const MACAddress&, const MACAddress&, const IPv4Address&, const IPv4Address&);
        static std::tuple<MACAddress, MACAddress, IPv4Address, IPv4Address> parseARPPayload(const ARPPayload&);
};