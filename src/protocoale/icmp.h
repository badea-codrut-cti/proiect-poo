#include "./payload.h"

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

        [[nodiscard]] L2Payload* clone() const override;

        [[nodiscard]] ICMPType getType() const;

        [[nodiscard]] uint8_t getCode() const;
};  