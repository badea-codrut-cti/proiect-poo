#include <cstdint>
#include <ostream>
#include "../date/ipv4.h"
#include "../date/macaddress.h"
#include "./payload.h"

#ifndef ARPPAYLOAD_H
#define ARPPAYLOAD_H

namespace ARPData {
    enum Operation : uint16_t {
        REQUEST = 1,
        REPLY = 2
    };

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
};

template <std::size_t N, std::size_t M>
class ARPPayload : public L2Payload {
    public:

    private: 
        ARPData::HardwareType hardwareType;
        ARPData::ProtocolType protocolType;
        ARPData::Operation operation;
        std::array<uint8_t, N> sourceHardwareAddress, destinationHardwareAddress;
        std::array<uint8_t, M> sourceProtocolAddress, destinationProtocolAddress;

    protected:
        void print(std::ostream&) const override;

    public:
        
        ARPPayload(ARPData::Operation, 
        const std::array<uint8_t, N>&,
        const std::array<uint8_t, N>&,
        const std::array<uint8_t, M>&,
        const std::array<uint8_t, M>&, 
        ARPData::HardwareType,
        ARPData::ProtocolType
        );

        ARPPayload(const ARPPayload<N, M>&);

        ARPPayload& operator=(const ARPPayload<N, M>&);

        ~ARPPayload() override;

        [[nodiscard]] L2Payload* clone() const override;

        [[nodiscard]] ARPData::Operation getOperation() const;

        [[nodiscard]] std::array<uint8_t, N> getSourceHardwareAddress() const;

        [[nodiscard]] std::array<uint8_t, N> getDestinationHardwareAddress() const;

        [[nodiscard]] std::array<uint8_t, M>  getSourceProtocolAddress() const;

        [[nodiscard]] std::array<uint8_t, M>  getDestinationProtocolAddress() const;

        [[nodiscard]] ARPData::HardwareType getHardwareType() const;

        [[nodiscard]] ARPData::ProtocolType getProtocolType() const;
};  

class ARPIpv4 : public ARPPayload<MACADDRESS_SIZE, IPV4_SIZE> {
    public:
        ARPIpv4(ARPData::Operation, const MACAddress&, const MACAddress&, 
        const IPv4Address&, const IPv4Address&);

        ~ARPIpv4();

        [[nodiscard]] MACAddress getSourceMacAddress() const;

        [[nodiscard]] MACAddress getDestinationMacAddress() const;

        [[nodiscard]] IPv4Address getSourceIPv4Address() const;

        [[nodiscard]] IPv4Address getDestinationIPv4Address() const;

        [[nodiscard]] ARPData::Operation getOperation() const;

        [[nodiscard]] L2Payload* clone() const override;
};

#endif