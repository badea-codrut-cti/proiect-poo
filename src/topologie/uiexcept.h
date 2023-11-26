#include <exception>
#include <string>
#include <utility>

#include "../device/device.h"

class UIException : public std::exception {
    public:
        explicit UIException(std::string message) : msg(std::move(message)) {}
        [[nodiscard]] const char* what() const noexcept override { return msg.c_str(); }

    private:
        std::string msg;
};

class UIParameterException : public UIException {
    public:
        explicit UIParameterException(const std::string& key) : UIException("Parameter " + key + " has invalid value.") {}
};

class UIStateException : public UIException {
    public:
        explicit UIStateException(const Device&);
        explicit UIStateException(const EthernetInterface&);
};