#pragma once

typedef enum {
    v1_7,
    v1_13,
    v1_16_1,
    v1_18,
    VERSION_COUNT
} Version;

extern const char *const VERSION_NAMES[VERSION_COUNT];