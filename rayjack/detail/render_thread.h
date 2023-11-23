#pragma once

#include "utils.h"
#include <stdint.h>
#include <vector>

class RenderThread {

public:
    struct Work {
        uint16_t scanline = 0;
    };

    struct Result {
        uint16_t scanline = 0;
        std::vector<color> data = {}; 
    }
}