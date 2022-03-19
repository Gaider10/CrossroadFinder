#pragma once

int getIntNormal(char *name, int *result, int useDefault, int defaultValue, int min, int max);
int getIntEnum(char *name, int *result, const char *const *elements, int count);

long long getLongNormal(char *name, long long *result);