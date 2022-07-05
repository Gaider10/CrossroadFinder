#pragma once

#include <stdint.h>
#include <stdbool.h>

#define RANDOM_MULTIPLIER 25214903917LLU
#define RANDOM_ADDEND 11LLU
#define RANDOM_MASK 281474976710655LLU

typedef struct {
    uint64_t seed;
} Random;

void random_setSeed(Random *random, uint64_t seed);

void random_setCarverSeed(Random *random, uint64_t structureSeed, int32_t chunkX, int32_t chunkZ);
void random_setRegionSeed(Random *random, uint64_t structureSeed, int32_t regionX, int32_t regionZ, int32_t salt);

int32_t random_next(Random *random, int bits);
int32_t random_nextInt(Random *random);
uint32_t random_nextIntUnknown(Random *random, uint32_t bound);
uint32_t random_nextIntPow2(Random *random, uint32_t bound);
uint32_t random_nextIntNotPow2(Random *random, uint32_t bound);
int64_t random_nextLong(Random *random);
bool random_nextBoolean(Random *random);
float random_nextFloat(Random *random);
double random_nextDouble(Random *random);