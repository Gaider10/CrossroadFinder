#pragma once

#define RANDOM_MULTIPLIER 25214903917LLU
#define RANDOM_ADDEND 11LLU
#define RANDOM_MASK 281474976710655LLU

typedef struct {
    long long seed;
} Random;

void random_setSeed(Random *random, long long seed);

void random_setCarverSeed(Random *random, long long structureSeed, int chunkX, int chunkZ);
void random_setRegionSeed(Random *random, long long structureSeed, int regionX, int regionZ, int salt);

int random_next(Random *random, int bits);
int random_nextInt(Random *random);
int random_nextIntUnknown(Random *random, int bound);
int random_nextIntPow2(Random *random, int bound);
int random_nextIntNotPow2(Random *random, int bound);
long long random_nextLong(Random *random);
int random_nextBoolean(Random *random);
float random_nextFloat(Random *random);
double random_nextDouble(Random *random);