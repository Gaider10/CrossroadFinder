#include "BlockBox.h"

#include "Direction.h"
#include "Math.h"

void blockBox_rotated(BlockBox *blockBox, int x, int y, int z, int offsetX, int offsetY, int offsetZ, int sizeX, int sizeY, int sizeZ, Direction facing) {
    switch(facing) {
        case NORTH:
            blockBox->minX = x + offsetX;
            blockBox->minY = y + offsetY;
            blockBox->minZ = z - sizeZ + 1 + offsetZ;
            blockBox->maxX = x + sizeX - 1 + offsetX;
            blockBox->maxY = y + sizeY - 1 + offsetY;
            blockBox->maxZ = z + offsetZ;
            break;
        case SOUTH:
            blockBox->minX = x + offsetX;
            blockBox->minY = y + offsetY;
            blockBox->minZ = z + offsetZ;
            blockBox->maxX = x + sizeX - 1 + offsetX;
            blockBox->maxY = y + sizeY - 1 + offsetY;
            blockBox->maxZ = z + sizeZ - 1 + offsetZ;
            break;
        case WEST:
            blockBox->minX = x - sizeZ + 1 + offsetZ;
            blockBox->minY = y + offsetY;
            blockBox->minZ = z + offsetX;
            blockBox->maxX = x + offsetZ;
            blockBox->maxY = y + sizeY - 1 + offsetY;
            blockBox->maxZ = z + sizeX - 1 + offsetX;
            break;
        case EAST:
            blockBox->minX = x + offsetZ;
            blockBox->minY = y + offsetY;
            blockBox->minZ = z + offsetX;
            blockBox->maxX = x + sizeZ - 1 + offsetZ;
            blockBox->maxY = y + sizeY - 1 + offsetY;
            blockBox->maxZ = z + sizeX - 1 + offsetX;
            break;
    }
}

int blockBox_intersects(BlockBox *blockBox, BlockBox *other) {
	return
		blockBox->maxX >= other->minX &&
		blockBox->minX <= other->maxX &&
		blockBox->maxZ >= other->minZ &&
		blockBox->minZ <= other->maxZ &&
		blockBox->maxY >= other->minY &&
		blockBox->minY <= other->maxY;
}

void blockBox_encompass(BlockBox *blockBox, BlockBox *region) {
    blockBox->minX = min(blockBox->minX, region->minX);
    blockBox->minY = min(blockBox->minY, region->minY);
    blockBox->minZ = min(blockBox->minZ, region->minZ);
    blockBox->maxX = max(blockBox->maxX, region->maxX);
    blockBox->maxY = max(blockBox->maxY, region->maxY);
    blockBox->maxZ = max(blockBox->maxZ, region->maxZ);
}