#pragma once

#include "Direction.h"

typedef struct {
    int minX, minY, minZ;
    int maxX, maxY, maxZ;
} BlockBox;

void blockBox_rotated(BlockBox *blockBox, int x, int y, int z, int offsetX, int offsetY, int offsetZ, int sizeX, int sizeY, int sizeZ, Direction facing);

int blockBox_intersects(BlockBox *blockBox, BlockBox *other);

void blockBox_encompass(BlockBox *blockBox, BlockBox *region);