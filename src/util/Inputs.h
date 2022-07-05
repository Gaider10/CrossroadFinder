#pragma once

#include <stdbool.h>
#include <stdint.h>

bool getI32Number(const char *name, int32_t *out, bool hasDefault, int32_t defaultValue, int32_t min, int32_t max);
bool getI64Number(const char *name, int64_t *out, bool hasDefault, int64_t defaultValue, int64_t min, int64_t max);

bool getIntEnum(const char *name, int *out, const char *const *elements, int elementCount);