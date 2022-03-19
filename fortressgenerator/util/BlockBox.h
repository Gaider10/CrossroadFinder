#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "Direction.h"

typedef struct {
    int32_t minX, minY, minZ;
    int32_t maxX, maxY, maxZ;
} BlockBox;

void blockBox_rotated(BlockBox *blockBox, int32_t x, int32_t y, int32_t z, int32_t offsetX, int32_t offsetY, int32_t offsetZ, int32_t sizeX, int32_t sizeY, int32_t sizeZ, Direction facing);

bool blockBox_intersects(BlockBox *blockBox, BlockBox *other);

void blockBox_encompass(BlockBox *blockBox, BlockBox *region);