#include <iostream>
#include <ostream>

#ifndef PAYLOAD_H
#define PAYLOAD_H

class L2Payload {
    protected:
        virtual void print(std::ostream&) const = 0;
    public:
        L2Payload() = default;
        L2Payload(const L2Payload&) = default;
        virtual ~L2Payload() = default;

        [[nodiscard]] virtual L2Payload* clone() const = 0;

        friend std::ostream& operator<<(std::ostream& o, const L2Payload& pl) {
            pl.print(o);
            return o;
        }
};

#endif