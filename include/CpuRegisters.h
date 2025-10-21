#pragma once

#include <cstdint>
#include <map>
#include <string>

namespace cpu_data {
    std::map<std::string, uint8_t> registers = {{"R0", 0x00}, {"A", 0x00}, {"R1", 0x01}, {"B", 0x01}, {"R2", 0x02}, {"C", 0x02}, {"R3", 0x03}, {"D", 0x03}};
}