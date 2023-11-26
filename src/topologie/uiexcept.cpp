#include "uiexcept.h"

UIStateException::UIStateException(const Device& dev) : 
UIException(std::string("Device is turned ") + 
std::string(dev.getState() ? "on" : "off") + ".") {

}

UIStateException::UIStateException(const EthernetInterface& interface) :
UIException(std::string("Interface is turned ") + 
std::string(interface.getState() ? "on" : "off") + ".") {

}

