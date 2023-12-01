#include <nlohmann/json.hpp>
#include "../device/device.h"

nlohmann::json deviceToJson(const Device*);

nlohmann::json frameToJson(const DataLinkLayer& data);