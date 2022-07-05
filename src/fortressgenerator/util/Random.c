#include "Random.h"

void random_setSeed(Random *random, uint64_t seed) {
	random->seed = (seed ^ RANDOM_MULTIPLIER) & RANDOM_MASK;
}

void random_setCarverSeed(Random *random, uint64_t structureSeed, int32_t chunkX, int32_t chunkZ) {
    random_setSeed(random, structureSeed);
    uint64_t a = (uint64_t)random_nextLong(random);
    uint64_t b = (uint64_t)random_nextLong(random);
    random_setSeed(random, chunkX * a ^ chunkZ * b ^ structureSeed);
}

void random_setRegionSeed(Random *random, uint64_t structureSeed, int32_t regionX, int32_t regionZ, int32_t salt) {
    random_setSeed(random, regionX * 341873128712LLU + regionZ * 132897987541LLU + structureSeed + (uint64_t)(int64_t)salt);
}

int32_t random_next(Random *random, int bits) {
	random->seed = (random->seed * RANDOM_MULTIPLIER + RANDOM_ADDEND) & RANDOM_MASK;
	return (int32_t)(random->seed >> (48 - bits));
}

int32_t random_nextInt(Random *random) {
	return random_next(random, 32);
}

uint32_t random_nextIntUnknown(Random *random, uint32_t bound) {
	if ((bound & (bound - 1)) == 0) {
		return ((uint64_t)bound * (uint64_t)random_next(random, 31)) >> 31;
	}
    uint32_t bits = (uint32_t)random_next(random, 31);
	uint32_t val = bits % bound;
	while ((int32_t)(bits - val + bound - 1) < 0) {
		bits = (uint32_t)random_next(random, 31);
		val = bits % bound;
	}
	return val;
}

uint32_t random_nextIntPow2(Random *random, uint32_t bound) {
	return ((uint64_t)bound * (uint64_t)random_next(random, 31)) >> 31;
}

uint32_t random_nextIntNotPow2(Random *random, uint32_t bound) {
	uint32_t bits = (uint32_t)random_next(random, 31);
	uint32_t val = bits % bound;
	while ((int32_t)(bits - val + bound - 1) < 0) {
		bits = (uint32_t)random_next(random, 31);
		val = bits % bound;
	}
	return val;
}

int64_t random_nextLong(Random *random) {
	return ((int64_t)random_next(random, 32) << 32) + (int64_t)random_next(random, 32);
}

bool random_nextBoolean(Random *random) {
	return random_next(random, 1) != 0;
}

float random_nextFloat(Random *random) {
	return (float)random_next(random, 24) / (float)(1 << 24);
}

double random_nextDouble(Random *random) {
	return (double)(((int64_t)random_next(random, 26) << 27u) + (int64_t)random_next(random, 27)) / (double)(1LLU << 53);
}