#pragma once

typedef enum {
    v1_16_1 = 0,
    v1_18 = 1,
    VERSION_COUNT = 2
} Version;

extern const char *VERSION_NAMES[VERSION_COUNT];