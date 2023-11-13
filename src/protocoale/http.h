#include <string>
#include "./payload.h"

class HTTPRequestPayload : public L2Payload {
    public:
        enum Method : uint16_t {
            GET     = 1,
            POST    = 2,
            PUT     = 3,
            DELETE  = 4,
            PATCH   = 5
        };

    private: 
        Method method;
        std::string url, host, userAgent, contentType, body;

    protected:
        void print(std::ostream&) const override;

    public:
        HTTPRequestPayload(Method, std::string, std::string, 
        std::string, std::string, std::string);

        HTTPRequestPayload(const HTTPRequestPayload&);

        [[nodiscard]] Method getMethod() const;

        [[nodiscard]] const std::string& getUrl() const;

        [[nodiscard]] const std::string& getUserAgent() const;

        [[nodiscard]] const std::string& getContentType() const;

        [[nodiscard]] const std::string& getBody() const;
};