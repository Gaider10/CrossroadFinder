#pragma once

#include "PieceType.h"

typedef struct {
    PieceType pieceType;
    int weight;
    int maxPlaceCount;
    int allowInRow;
    int placeCount;
} PieceData;

#define BRIDGE_PIECE_WEIGHTS_COUNT 6
#define CASTLE_PIECE_WEIGHTS_COUNT 7

extern const PieceData BRIDGE_PIECE_WEIGHTS[BRIDGE_PIECE_WEIGHTS_COUNT];
extern const PieceData CASTLE_PIECE_WEIGHTS[CASTLE_PIECE_WEIGHTS_COUNT];

int pieceData_isInvalid(PieceData *pieceData);