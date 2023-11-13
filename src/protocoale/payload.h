#include <iostream>
#include <ostream>

#ifndef PAYLOAD_H
#define PAYLOAD_H

class L2Payload {
    protected:
        virtual void print(std::ostream&) const {

        }
    public:
        virtual ~L2Payload() = default;
        friend std::ostream& operator<<(std::ostream& o, const L2Payload& pl) {
            pl.print(o);
            return o;
        }
};

#endif