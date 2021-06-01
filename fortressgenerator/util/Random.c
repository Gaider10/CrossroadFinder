#include "Random.h"

void random_setSeed(Random *random, long long seed) {
	random->seed = (seed ^ RANDOM_MULTIPLIER) & RANDOM_MASK;
}

void random_setCarverSeed(Random *random, long long structureSeed, int chunkX, int chunkZ) {
    random_setSeed(random, structureSeed);
    long long a = random_nextLong(random);
    long long b = random_nextLong(random);
    random_setSeed(random, chunkX * a ^ chunkZ * b ^ structureSeed);
}

void random_setRegionSeed(Random *random, long long structureSeed, int regionX, int regionZ, int salt) {
    random_setSeed(random, regionX * 341873128712LL + regionZ * 132897987541LL + structureSeed + salt);
}

int random_next(Random *random, int bits) {
	random->seed = (random->seed * RANDOM_MULTIPLIER + RANDOM_ADDEND) & RANDOM_MASK;
	return random->seed >> (48 - bits);
}

int random_nextInt(Random *random) {
	return random_next(random, 32);
}

int random_nextIntUnknown(Random *random, int bound) {
	if ((bound & (bound - 1)) == 0) {
		return ((unsigned long long)bound * (unsigned long long)random_next(random, 31)) >> 31;
	}
	return random_next(random, 31) % bound;
}

int random_nextIntPow2(Random *random, int bound) {
	return ((unsigned long long)bound * (unsigned long long)random_next(random, 31)) >> 31;
}

int random_nextIntNotPow2(Random *random, int bound) {
	return random_next(random, 31) % bound;
}

long long random_nextLong(Random *random) {
	return ((long long)random_next(random, 32) << 32) + random_next(random, 32);
}

int random_nextBoolean(Random *random) {
	return random_next(random, 1) != 0;
}

float random_nextFloat(Random *random) {
	return random_next(random, 24) / ((float)(1 << 24));
}

double random_nextDouble(Random *random) {
	return (((long long)random_next(random, 26) << 27u) + (long long)random_next(random, 27)) / (double)(1LLU << 53);
}