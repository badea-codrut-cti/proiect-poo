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

class NDPPayload : public L2Payload {
    public:
        enum NDPOperation : uint8_t {
            ROUTER_SOLICITATION = 133,
            ROUTER_ADVERTISEMENT = 134,
            NEIGHBOR_SOLICITATION = 135,
            NEIGHBOR_ADVERTISEMENT = 136,
            REDIRECT_MESSAGE = 137
        };

    private: 
        NDPOperation ndpOperation;
        uint8_t ndpCode;
        IPv6Address targetAddress;

    protected:
        void print(std::ostream&) const override;

    public:
        NDPPayload(NDPOperation, uint8_t, const IPv6Address&);

        NDPPayload(const NDPPayload&);

        NDPPayload& operator=(const NDPPayload&);

        [[nodiscard]] NDPPayload* clone() const override;

        [[nodiscard]] NDPOperation getOperation() const;

        [[nodiscard]] uint8_t getCode() const;

        [[nodiscard]] IPv6Address getTargetAddress() const;

};

#endif