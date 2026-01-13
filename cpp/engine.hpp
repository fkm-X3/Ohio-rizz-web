#pragma once
#include <cstdint>

extern "C" {
    void render_page(const char* html, uint32_t* buffer, int width, int height);
}
