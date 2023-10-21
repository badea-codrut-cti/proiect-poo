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
        HardwareType hardwareType{ETHERNET};
        ProtocolType protocolType{IPV4};
        uint8_t hwAddLength, protoAddLength;
        Operation operation;
        uint8_t *sourceHardwareAddress, *destinationHardwareAddress,
        *sourceProtocolAddress, *destinationProtocolAddress;

    public:
        template <std::size_t N, std::size_t M>
        ARPPayload(Operation _operation, 
        const std::array<uint8_t, N>& _srcHwAddress,
        const std::array<uint8_t, N>& _destHwAddress,
        const std::array<uint8_t, M>& _srcProtoAddress,
        const std::array<uint8_t, M>& _destProtoAddress): 
        operation(_operation) {
            hwAddLength = N;
            protoAddLength = M;

            sourceHardwareAddress = new uint8_t[_srcHwAddress.size()];
            destinationHardwareAddress = new uint8_t[_srcHwAddress.size()];

            sourceProtocolAddress = new uint8_t[_srcProtoAddress.size()];
            destinationProtocolAddress = new uint8_t[_srcProtoAddress.size()];

            for (unsigned long i = 0; i < _srcHwAddress.size(); i++) {
                sourceHardwareAddress[i] = _srcHwAddress[i];
                destinationHardwareAddress[i] = _destHwAddress[i];
            }

            for(unsigned long i = 0; i < _srcProtoAddress.size(); i++) {
                sourceProtocolAddress[i] = _srcProtoAddress[i];
                destinationProtocolAddress[i] = _destProtoAddress[i];
            }
        }

        ARPPayload(const ARPPayload& other) :
        hwAddLength(other.hwAddLength), protoAddLength(other.protoAddLength),
        operation(other.operation) {
            sourceHardwareAddress = new uint8_t[hwAddLength];
            std::copy(other.sourceHardwareAddress, other.sourceHardwareAddress + hwAddLength, sourceHardwareAddress);

            destinationHardwareAddress = new uint8_t[hwAddLength];
            std::copy(other.destinationHardwareAddress, other.destinationHardwareAddress + hwAddLength, destinationHardwareAddress);

            sourceProtocolAddress = new uint8_t[protoAddLength];
            std::copy(other.sourceProtocolAddress, other.sourceProtocolAddress + protoAddLength, sourceProtocolAddress);

            destinationProtocolAddress = new uint8_t[protoAddLength];
            std::copy(other.destinationProtocolAddress, other.destinationProtocolAddress + protoAddLength, destinationProtocolAddress);
        }

        static ARPPayload createARPRequest(const MACAddress& _sourceHw, const IPv4Address& _sourceProto, const IPv4Address& _destProto) {
            return {REQUEST, _sourceHw.getOctets(), 
            {0, 0, 0, 0, 0, 1}, 
            _sourceProto.getOctets(), _destProto.getOctets()};
        }

        ~ARPPayload() override {
            delete[] sourceHardwareAddress;
            delete[] destinationHardwareAddress;
            delete[] sourceProtocolAddress;
            delete[] destinationProtocolAddress;
        }

        [[nodiscard]] Operation getOperation() const {
            return operation;
        }

        [[nodiscard]] const uint8_t* getSourceHardwareAddress() const {
            return sourceHardwareAddress;
        }

        [[nodiscard]] const uint8_t* getDestinationHardwareAddress() const {
            return destinationHardwareAddress;
        }

        [[nodiscard]] const uint8_t* getSourceProtocolAddress() const {
            return sourceProtocolAddress;
        }

        [[nodiscard]] const uint8_t* getDestinationProtocolAddress() const {
            return destinationProtocolAddress;
        }
};  