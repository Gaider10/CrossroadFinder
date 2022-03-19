#include "Inputs.h"

#include <stdio.h>
#include <stdlib.h>

int getInt(int *result, bool useDefault, int defaultValue, int min, int max) {
    char input[64];
    char *ptr;

    fgets(input, 64, stdin);

    if (input[0] == '\n') {
        if (useDefault) {
            *result = defaultValue;
            return 1;
        } else {
            printf("Entered value is not valid\n");
            return 0;
        }
    }

    *result = strtol(input, &ptr, 10);
    if (*ptr != '\n') {
        printf("Entered value is not valid\n");
        return 0;
    }

    if (*result < min) {
        printf("Entered value can't be smaller than %i\n", min);
        return 0;
    }

    if (*result > max) {
        printf("Entered value can't be bigger than %i\n", max);
        return 0;
    }

    return 1;
}

int getIntNormal(const char *name, int *result, bool useDefault, int defaultValue, int min, int max) {
    if (useDefault) {
        printf("Enter %s (leave empty for %i):\n", name, defaultValue);
    } else {
        printf("Enter %s:\n", name);
    }

    if (!getInt(result, useDefault, defaultValue, min, max)) {
        return 0;
    }

    printf("Using %s %i\n\n", name, *result);

    return 1;
}

int getIntEnum(const char *name, int *result, const char *const *elements, int count) {
    printf("Enter %s:\n", name);

    for (int i = 0; i < count; i++) {
        printf("%i - %s\n", i, elements[i]);
    }

    if (!getInt(result, false, 0, 0, count - 1)) {
        return 0;
    }

    printf("Using %s %s\n\n", name, elements[*result]);

    return 1;
}

long long getLong(long long *result) {
    char input[64];
    char *ptr;
    
    fgets(input, 64, stdin);

    if (input[0] == '\n') {
        printf("Entered value is not valid\n");
        return 0;
    }

    *result = strtoll(input, &ptr, 10);
    if (*ptr != '\n') {
        printf("Entered value is not valid\n");
        return 0;
    }

    return 1;
}

long long getLongNormal(const char *name, long long *result) {
    printf("Enter %s\n", name);

    if (!getLong(result)) {
        return 0;
    }

    printf("Using %s %lli\n\n", name, *result);

    return 1;
}