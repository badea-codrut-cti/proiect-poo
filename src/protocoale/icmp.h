#include "./payload.h"
#include "../date/ipv6.h"

#ifndef ICMP_H
#define ICMP_H

class ICMPPayload : public L2Payload {
    public:
        enum ICMPType : uint8_t {
            ECHO_REPLY = 0,
            DEST_UNREACHABLE = 3,
            SOURCE_QUENCH = 4,
            REDIRECT_MESSAGE = 5,
            ECHO_REQUEST = 8,
            TIME_EXCEEDED = 11,
            PARAMETER_PROBLEM = 12,
            TIMESTAMP_REQUEST = 13,
            TIMESTAMP_REPLY = 14,
            INFO_REQUEST = 15,
            INFO_REPLY = 16
        };

    private: 
        ICMPType plType;
        uint8_t plCode;
    protected:
        void print(std::ostream&) const override;

    public:
        ICMPPayload(ICMPType, uint8_t);

        ICMPPayload(const ICMPPayload&);

        ICMPPayload& operator=(const ICMPPayload&);

        [[nodiscard]] ICMPPayload* clone() const override;

        [[nodiscard]] ICMPType getType() const;

        [[nodiscard]] uint8_t getCode() const;
};  

class ICMPv6Payload : public L2Payload {
    public:
        enum ICMPv6Type : uint8_t {
            DEST_UNREACHABLE = 1,
            PACKET_TOO_BIG = 2,
            TIME_EXCEEDED = 3,
            PARAMETER_PROBLEM = 4,
            ECHO_REQUEST = 128,
            ECHO_REPLY = 129,
            REDIRECT_MESSAGE = 137
        };

    protected:
        uint8_t plType;
        uint8_t plCode;

        void print(std::ostream&) const override;
    public:
        ICMPv6Payload(uint8_t, uint8_t);

        ICMPv6Payload(const ICMPv6Payload&);

        ICMPv6Payload& operator=(const ICMPv6Payload&);

        [[nodiscard]] ICMPv6Payload* clone() const override;

        [[nodiscard]] uint8_t getType() const;

        [[nodiscard]] uint8_t getCode() const;
};

class NDPPayload : public ICMPv6Payload {
    public:
        enum NDPOperation : uint8_t {
            ROUTER_SOLICITATION = 133,
            ROUTER_ADVERTISEMENT = 134,
            NEIGHBOR_SOLICITATION = 135,
            NEIGHBOR_ADVERTISEMENT = 136,
            REDIRECT_MESSAGE = 137
        };

    private: 
        IPv6Address targetAddress;

    protected:
        void print(std::ostream&) const override;

    public:
        NDPPayload(NDPOperation, uint8_t, const IPv6Address&);

        NDPPayload(const NDPPayload&);

        NDPPayload& operator=(const NDPPayload&);

        [[nodiscard]] NDPPayload* clone() const override;

        [[nodiscard]] IPv6Address getTargetAddress() const;

};

#endif