#pragma once

#include <stdbool.h>

int getIntNormal(const char *name, int *result, bool useDefault, int defaultValue, int min, int max);
int getIntEnum(const char *name, int *result, const char *const *elements, int count);

long long getLongNormal(const char *name, long long *result);