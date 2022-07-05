#include "FortressGenerator.h"

#include <string.h>
// #include <stdio.h>

#include "util/Math.h"
#include "util/Random.h"
#include "Piece.h"
#include "PieceType.h"
#include "PieceData.h"

int32_t fortressGenerator_getRegionSize(Version version) {
    if (version < v1_16_1) {
        return 16;
    } else {
        return 27;
    }
}

void fortressGenerator_generateForRegion(FortressGenerator *fortressGenerator, uint64_t structureSeed, int32_t regionX, int32_t regionZ, int32_t salt, Version version) {
    Random random;
    if (version < v1_16_1) {
        random_setSeed(&random, (uint64_t)(int64_t)(regionX ^ (regionZ << 4)) ^ structureSeed);
        random_nextInt(&random);
        if (random_nextIntNotPow2(&random, 3) == 0) {
            int32_t chunkX = regionX * 16 + 4 + random_nextIntPow2(&random, 8);
            int32_t chunkZ = regionZ * 16 + 4 + random_nextIntPow2(&random, 8);
            fortressGenerator->random = random;
            fortressGenerator_generateForChunk(fortressGenerator, structureSeed, chunkX, chunkZ, version);
            return;
        }
    } else {
        random_setRegionSeed(&random, structureSeed, regionX, regionZ, salt);
        int32_t chunkX = regionX * 27 + random_nextIntNotPow2(&random, 27 - 4);
        int32_t chunkZ = regionZ * 27 + random_nextIntNotPow2(&random, 27 - 4);
        if (version >= v1_18) {
            random_setCarverSeed(&random, structureSeed, chunkX, chunkZ);
        }
        if (random_nextIntNotPow2(&random, 5) < 2) {
            fortressGenerator_generateForChunk(fortressGenerator, structureSeed, chunkX, chunkZ, version);
            return;
        }
    }

    fortressGenerator->piecesCount = 0;
}

void fortressGenerator_generateForChunk(FortressGenerator *fortressGenerator, uint64_t structureSeed, int32_t chunkX, int32_t chunkZ, Version version) {
    // printf("Running for chunk %i %i\n", chunkX, chunkZ);
    fortressGenerator->lastPieceType = -1;
    fortressGenerator->pendingChildrenCount = 0;
    fortressGenerator->piecesCount = 0;
    
    memcpy(fortressGenerator->bridgePieces, BRIDGE_PIECE_WEIGHTS, sizeof(BRIDGE_PIECE_WEIGHTS));
    memcpy(fortressGenerator->corridorPieces, CASTLE_PIECE_WEIGHTS, sizeof(CASTLE_PIECE_WEIGHTS));
    fortressGenerator->bridgePiecesCount = BRIDGE_PIECE_WEIGHTS_COUNT;
    fortressGenerator->corridorPiecesCount = CASTLE_PIECE_WEIGHTS_COUNT;
    
    fortressGenerator->startX = (chunkX << 4) + 2;
    fortressGenerator->startZ = (chunkZ << 4) + 2;
    
    if (version >= v1_13) {
        random_setCarverSeed(&(fortressGenerator->random), structureSeed, chunkX, chunkZ);
    }

    Piece *start = &(fortressGenerator->pieces[fortressGenerator->piecesCount]);
    start->pieceType = BRIDGE_CROSSING;
    start->boundingBox.minX = fortressGenerator->startX;
    start->boundingBox.minY = 64;
    start->boundingBox.minZ = fortressGenerator->startZ;
    start->boundingBox.maxX = fortressGenerator->startX + 19 - 1;
    start->boundingBox.maxY = 73;
    start->boundingBox.maxZ = fortressGenerator->startZ + 19 - 1;
    start->facing = random_nextIntPow2(&(fortressGenerator->random), 4);
    start->length = 0;
    fortressGenerator->piecesCount = 1;
    
    // printf("Placing piece %s at /tp %i ~ %i\n", PIECE_TYPE_NAMES[start->pieceType], start->boundingBox.minX, start->boundingBox.minZ);
    // printf("Placing children for %s at /tp %i ~ %i\n", PIECE_TYPE_NAMES[start->pieceType], start->boundingBox.minX, start->boundingBox.minZ);
    fortressGenerator_placeJigsaw(fortressGenerator, start);

    while (fortressGenerator->pendingChildrenCount != 0) {
        // printf("Placing a piece, %i left\n", fortressGenerator->pendingChildrenCount);

        int i = random_nextIntUnknown(&(fortressGenerator->random), fortressGenerator->pendingChildrenCount);
        Piece *piece = fortressGenerator->pendingChildren[i];
        if(i < fortressGenerator->pendingChildrenCount - 1) {
            memmove(&(fortressGenerator->pendingChildren[i]),
                    &(fortressGenerator->pendingChildren[i]) + 1,
                    (fortressGenerator->pendingChildrenCount - i - 1) * sizeof(Piece*));
        }
        fortressGenerator->pendingChildrenCount--;
        // printf("Placing children for %s at /tp %i ~ %i\n", PIECE_TYPE_NAMES[piece->pieceType], piece->boundingBox.minX, piece->boundingBox.minZ);
        fortressGenerator_placeJigsaw(fortressGenerator, piece);
    }

    BlockBox boundingBox = start->boundingBox;
    for (int i = 1; i < fortressGenerator->piecesCount; i++) {
        blockBox_encompass(&boundingBox, &(fortressGenerator->pieces[i].boundingBox));
    }

    int32_t k = 23 - (boundingBox.maxY - boundingBox.minY + 1);
    int32_t m = k > 1 ? 48 + random_nextIntUnknown(&(fortressGenerator->random), k) : 48;
    int32_t offsetY = m - boundingBox.minY;

    for (int i = 0; i < fortressGenerator->piecesCount; i++) {
        fortressGenerator->pieces[i].boundingBox.minY += offsetY;
        fortressGenerator->pieces[i].boundingBox.maxY += offsetY;
    }
}

void fortressGenerator_placeJigsaw(FortressGenerator *fortressGenerator, Piece *piece) {
    int i;
    switch (piece->pieceType) {
        case BRIDGE:
            fortressGenerator_generateChildForward(fortressGenerator, piece, 1, 3, false);
            return;
        case BRIDGE_CROSSING:
            fortressGenerator_generateChildForward(fortressGenerator, piece, 8, 3, false);
            fortressGenerator_generateChildLeft(fortressGenerator, piece, 3, 8, false);
            fortressGenerator_generateChildRight(fortressGenerator, piece, 3, 8, false);
            return;
        case BRIDGE_SMALL_CROSSING:
            fortressGenerator_generateChildForward(fortressGenerator, piece, 2, 0, false);
            fortressGenerator_generateChildLeft(fortressGenerator, piece, 0, 2, false);
            fortressGenerator_generateChildRight(fortressGenerator, piece, 0, 2, false);
            return;
        case BRIDGE_STAIRS:
            fortressGenerator_generateChildRight(fortressGenerator, piece, 6, 2, false);
            return;
        case BRIDGE_PLATFORM:
            //nop
            return;
        case CORRIDOR_EXIT:
            fortressGenerator_generateChildForward(fortressGenerator, piece, 5, 3, true);
            return;
        case SMALL_CORRIDOR:
            fortressGenerator_generateChildForward(fortressGenerator, piece, 1, 0, true);
            return;
        case CORRIDOR_RIGHT_TURN:
            fortressGenerator_generateChildRight(fortressGenerator, piece, 0, 1, true);
            return;
        case CORRIDOR_LEFT_TURN:
            fortressGenerator_generateChildLeft(fortressGenerator, piece, 0, 1, true);
            return;
        case CORRIDOR_STAIRS:
            fortressGenerator_generateChildForward(fortressGenerator, piece, 1, 0, true);
            return;
        case CORRIDOR_BALCONY:
            i = 1;
            Direction direction = piece->facing;
            if (direction == WEST || direction == NORTH) {
                i = 5;
            }
            fortressGenerator_generateChildLeft(fortressGenerator, piece, 0, i, random_nextIntPow2(&(fortressGenerator->random), 8) > 0);
            fortressGenerator_generateChildRight(fortressGenerator, piece, 0, i, random_nextIntPow2(&(fortressGenerator->random), 8) > 0);
            return;
        case CORRIDOR_CROSSING:
            fortressGenerator_generateChildForward(fortressGenerator, piece, 1, 0, true);
            fortressGenerator_generateChildLeft(fortressGenerator, piece, 0, 1, true);
            fortressGenerator_generateChildRight(fortressGenerator, piece, 0, 1, true);
            return;
        case CORRIDOR_NETHER_WARTS_ROOM:
            fortressGenerator_generateChildForward(fortressGenerator, piece, 5, 3, true);
            fortressGenerator_generateChildForward(fortressGenerator, piece, 5, 11, true);
            return;
        case BRIDGE_END:
            return;
    }
}

void fortressGenerator_generateChildForward(FortressGenerator *fortressGenerator, Piece *piece, int32_t i, int32_t j, bool isCorridor) {
    switch (piece->facing) {
        case NORTH:
            fortressGenerator_generateAndAddPiece(fortressGenerator, piece->boundingBox.minX + i, piece->boundingBox.minY + j, piece->boundingBox.minZ - 1, piece->facing, piece->length, isCorridor);
            return;
        case SOUTH:
            fortressGenerator_generateAndAddPiece(fortressGenerator, piece->boundingBox.minX + i, piece->boundingBox.minY + j, piece->boundingBox.maxZ + 1, piece->facing, piece->length, isCorridor);
            return;
        case WEST:
            fortressGenerator_generateAndAddPiece(fortressGenerator, piece->boundingBox.minX - 1, piece->boundingBox.minY + j, piece->boundingBox.minZ + i, piece->facing, piece->length, isCorridor);
            return;
        case EAST:
            fortressGenerator_generateAndAddPiece(fortressGenerator, piece->boundingBox.maxX + 1, piece->boundingBox.minY + j, piece->boundingBox.minZ + i, piece->facing, piece->length, isCorridor);
    }
}

void fortressGenerator_generateChildLeft(FortressGenerator *fortressGenerator, Piece *piece, int32_t i, int32_t j, bool isCorridor) {
    switch (piece->facing) {
        case NORTH:
        case SOUTH:
            fortressGenerator_generateAndAddPiece(fortressGenerator, piece->boundingBox.minX - 1, piece->boundingBox.minY + i, piece->boundingBox.minZ + j, WEST, piece->length, isCorridor);
            return;
        case WEST:
        case EAST:
            fortressGenerator_generateAndAddPiece(fortressGenerator, piece->boundingBox.minX + j, piece->boundingBox.minY + i, piece->boundingBox.minZ - 1, NORTH, piece->length, isCorridor);
    }
}

void fortressGenerator_generateChildRight(FortressGenerator *fortressGenerator, Piece *piece, int32_t i, int32_t j, bool isCorridor) {
    switch (piece->facing) {
        case NORTH:
        case SOUTH:
            fortressGenerator_generateAndAddPiece(fortressGenerator, piece->boundingBox.maxX + 1, piece->boundingBox.minY + i, piece->boundingBox.minZ + j, EAST, piece->length, isCorridor);
            return;
        case WEST:
        case EAST:
            fortressGenerator_generateAndAddPiece(fortressGenerator, piece->boundingBox.minX + j, piece->boundingBox.minY + i, piece->boundingBox.maxZ + 1, SOUTH, piece->length, isCorridor);
    }
}

void fortressGenerator_generateAndAddPiece(FortressGenerator *fortressGenerator, int32_t x, int32_t y, int32_t z, Direction direction, int32_t length, bool isCorridor) {
    //printf("fortressGenerator_generateAndAddPiece\n");
    if (abs(x - fortressGenerator->startX) <= 112 && abs(z - fortressGenerator->startZ) <= 112) {
        Piece *piece = fortressGenerator_generateRandomPiece(fortressGenerator, x, y, z, direction, length + 1, isCorridor);
        if (piece != NULL) {
            fortressGenerator->piecesCount++;
            fortressGenerator->pendingChildren[fortressGenerator->pendingChildrenCount++] = piece;
            // printf("Placing piece %s at /tp %i ~ %i\n", PIECE_TYPE_NAMES[piece->pieceType], piece->boundingBox.minX, piece->boundingBox.minZ);
        }
    } else {
        BlockBox blockBox;
        blockBox_rotated(&blockBox, x, y, z, -1, -3, 0, 5, 10, 8, direction);
        if (fortressGenerator_isOkBox(fortressGenerator, &blockBox)) {
            random_nextInt(&(fortressGenerator->random));
        }
    }
}

Piece *fortressGenerator_generateRandomPiece(FortressGenerator *fortressGenerator, int32_t x, int32_t y, int32_t z, Direction direction, int32_t length, bool isCorridor) {
    //printf("fortressGenerator_generateRandomPiece\n");
    PieceData *pieceDataList = fortressGenerator->bridgePieces;
    int *pieceDataCount = &(fortressGenerator->bridgePiecesCount);
    if (isCorridor) {
        pieceDataList = fortressGenerator->corridorPieces;
        pieceDataCount = &(fortressGenerator->corridorPiecesCount);
    }
    
    int hasPiecesLeft = 0;
    int totalWeight = 0;

    for (int i = 0; i < *pieceDataCount; i++) {
        if (pieceDataList[i].maxPlaceCount > 0 && pieceDataList[i].placeCount < pieceDataList[i].maxPlaceCount) {
            hasPiecesLeft = 1;
        }
        totalWeight += pieceDataList[i].weight;
    }

    if (!hasPiecesLeft) totalWeight = -1;
    
    // printf(" totalWeight: %i isCorridor: %i length: %i\n", totalWeight, isCorridor, length);
    
    if (totalWeight > 0 && length <= 30){
        for (int n = 0; n < 5; n++){
            int o = random_nextIntUnknown(&(fortressGenerator->random), totalWeight);

            //printf("  selected: %i\n", o);

            PieceData *lastPieceData = pieceDataList + *pieceDataCount - 1;
            for (PieceData *pieceData = pieceDataList; pieceData <= lastPieceData; pieceData++) {
                o -= pieceData->weight;
                if (o < 0) {
                    if (pieceData->pieceType == fortressGenerator->lastPieceType && !pieceData->allowInRow) {
                        break;
                    }

                    Piece *piece = fortressGenerator_generatePiece(fortressGenerator, pieceData->pieceType, x, y, z, direction, length);
                    if (piece != 0) {
                        pieceData->placeCount++;
                        fortressGenerator->lastPieceType = pieceData->pieceType;
                        if (pieceData_isInvalid(pieceData)) {
                            // printf("Removing %i at index %llu\n", pieceData->pieceType, pieceData - pieceDataList);
                            //printf("Current size is %i\n", *pieceDataCount);
                            if (pieceData != lastPieceData) {
                                memmove(pieceData,
                                        pieceData + 1,
                                        (uintptr_t)lastPieceData - (uintptr_t)pieceData);
                                //printf("Now it is %i\n", pieceData->pieceType);
                            }
                            (*pieceDataCount)--;
                        }
                        
                        // printf("  Selected peice %s\n", PIECE_TYPES[piece->pieceType]);

                        return piece;
                    }
                }
            }
        }
    }

    Piece *piece = &(fortressGenerator->pieces[fortressGenerator->piecesCount]);
    blockBox_rotated(&(piece->boundingBox), x, y, z, -1, -3, 0, 5, 10, 8, direction);
    if (fortressGenerator_isOkBox(fortressGenerator, &(piece->boundingBox))) {
        piece->pieceType = BRIDGE_END;
        piece->facing = direction;
        piece->length = length;
        random_nextInt(&(fortressGenerator->random));
        return piece;
    }

    return NULL;
}

bool fortressGenerator_isOkBox(FortressGenerator *fortressGenerator, BlockBox *blockBox) {
    if (blockBox->minY <= 10) return false;
    // Piece *lastPiece = fortressGenerator->pieces + fortressGenerator->piecesCount - 1;
    // for(Piece *piece = fortressGenerator->pieces; piece <= lastPiece; piece++){
    //     if(blockBox_intersects(&(piece->boundingBox), blockBox)) return 0;
    // }
    for (Piece *piece = fortressGenerator->pieces + fortressGenerator->piecesCount - 1; piece >= fortressGenerator->pieces; piece--){
        if (blockBox_intersects(&(piece->boundingBox), blockBox)) {
            return false;
        }
    }
    return true;
}

Piece *fortressGenerator_generatePiece(FortressGenerator *fortressGenerator, PieceType pieceType, int32_t x, int32_t y, int32_t z, Direction direction, int32_t length) {
    //printf("fortressGenerator_generatePiece for pieceType %i\n", pieceType);
    Piece *piece = &(fortressGenerator->pieces[fortressGenerator->piecesCount]);
    switch (pieceType) {
        case BRIDGE:
            blockBox_rotated(&(piece->boundingBox), x, y, z, -1, -3, 0, 5, 10, 19, direction);
            break;
        case BRIDGE_CROSSING:
            blockBox_rotated(&(piece->boundingBox), x, y, z, -8, -3, 0, 19, 10, 19, direction);
            break;
        case BRIDGE_SMALL_CROSSING:
            blockBox_rotated(&(piece->boundingBox), x, y, z, -2, 0, 0, 7, 9, 7, direction);
            break;
        case BRIDGE_STAIRS:
            blockBox_rotated(&(piece->boundingBox), x, y, z, -2, 0, 0, 7, 11, 7, direction);
            break;
        case BRIDGE_PLATFORM:
            blockBox_rotated(&(piece->boundingBox), x, y, z, -2, 0, 0, 7, 8, 9, direction);
            break;
        case CORRIDOR_EXIT:
            blockBox_rotated(&(piece->boundingBox), x, y, z, -5, -3, 0, 13, 14, 13, direction);
            break;
        case SMALL_CORRIDOR:
            blockBox_rotated(&(piece->boundingBox), x, y, z, -1, 0, 0, 5, 7, 5, direction);
            break;
        case CORRIDOR_RIGHT_TURN:
            blockBox_rotated(&(piece->boundingBox), x, y, z, -1, 0, 0, 5, 7, 5, direction);
            break;
        case CORRIDOR_LEFT_TURN:
            blockBox_rotated(&(piece->boundingBox), x, y, z, -1, 0, 0, 5, 7, 5, direction);
            break;
        case CORRIDOR_STAIRS:
            blockBox_rotated(&(piece->boundingBox), x, y, z, -1, -7, 0, 5, 14, 10, direction);
            break;
        case CORRIDOR_BALCONY:
            blockBox_rotated(&(piece->boundingBox), x, y, z, -3, 0, 0, 9, 7, 9, direction);
            break;
        case CORRIDOR_CROSSING:
            blockBox_rotated(&(piece->boundingBox), x, y, z, -1, 0, 0, 5, 7, 5, direction);
            break;
        case CORRIDOR_NETHER_WARTS_ROOM:
            blockBox_rotated(&(piece->boundingBox), x, y, z, -5, -3, 0, 13, 14, 13, direction);
            break;
        default:
            return NULL;
    }

    if (!fortressGenerator_isOkBox(fortressGenerator, &(piece->boundingBox))) {
        return NULL;
    }

    piece->pieceType = pieceType;
    piece->facing = direction;
    piece->length = length;

    if (pieceType == CORRIDOR_RIGHT_TURN || pieceType == CORRIDOR_LEFT_TURN) {
        random_nextIntNotPow2(&(fortressGenerator->random), 3);
    }

    //printf("Generated piece %i", pieceType);

    return piece;
}