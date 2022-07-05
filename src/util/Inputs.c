#include "Inputs.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>

bool getI64(const char *name, int64_t *out, bool hasDefault, int64_t defaultValue, int64_t min, int64_t max) {
    char input[64];
    char *ptr;

    if (fgets(input, 64, stdin) == NULL) {
        printf("Error was encountered from fgets\n\n");
        return 0;
    }

    if (input[0] == '\n') {
        if (hasDefault) {
            *out = defaultValue;
            return 1;
        } else {
            printf("Entered value is not valid\n");
            return 0;
        }
    }

    errno = 0;
    int64_t value = (int64_t)strtoll(input, &ptr, 10);
    if (*ptr != '\n' || errno == ERANGE) {
        printf("Entered value is not valid\n");
        return 0;
    }

    if (value < min) {
        printf("Entered value can't be smaller than %" PRIi64 "\n", min);
        return 0;
    }

    if (value > max) {
        printf("Entered value can't be bigger than %" PRIi64 "\n", max);
        return 0;
    }

    *out = value;
    return 1;
}

bool getI32Number(const char *name, int32_t *out, bool hasDefault, int32_t defaultValue, int32_t min, int32_t max) {
    if (hasDefault) {
        printf("Enter %s (leave empty for %" PRIi32 "):\n", name, defaultValue);
    } else {
        printf("Enter %s:\n", name);
    }

    int64_t value;
    bool success = getI64(name, &value, hasDefault, (int64_t)defaultValue, (int64_t)min, (int64_t)max);

    if (success) {
        *out = (int32_t)value;
        printf("Using %s %" PRIi32 "\n", name, (int32_t)value);
    }

    return success;
}

bool getI64Number(const char *name, int64_t *out, bool hasDefault, int64_t defaultValue, int64_t min, int64_t max) {
    if (hasDefault) {
        printf("Enter %s (leave empty for %" PRIi64 "):\n", name, defaultValue);
    } else {
        printf("Enter %s:\n", name);
    }

    int64_t value;
    bool success = getI64(name, &value, hasDefault, defaultValue, min, max);

    if (success) {
        *out = value;
        printf("Using %s %" PRIi64 "\n", name, value);
    }

    return success;
}

bool getIntEnum(const char *name, int *out, const char *const *elements, int elementCount) {
    printf("Enter %s:\n", name);

    for (int i = 0; i < elementCount; i++) {
        printf("%i - %s\n", i, elements[i]);
    }

    int64_t value;
    bool success = getI64(name, &value, false, (int64_t)0, (int64_t)0, (int64_t)(elementCount - 1));

    if (success) {
        *out = (int)value;
        printf("Using %s %s\n", name, elements[value]);
    }

    return success;
}