#pragma once

#include "util/BlockBox.h"
#include "util/Direction.h"
#include "PieceType.h"

typedef struct {
    PieceType pieceType;
    BlockBox boundingBox;
    Direction facing;
    int32_t length;
} Piece;