#pragma once

#include "util/Version.h"
#include "util/BlockBox.h"
#include "util/Direction.h"
#include "util/Random.h"
#include "Piece.h"
#include "PieceData.h"
#include "PieceType.h"

#define FORTRESS_SALT 30084232

typedef struct {
    PieceData bridgePieces[BRIDGE_PIECE_WEIGHTS_COUNT];
    int bridgePiecesCount;
    
    PieceData corridorPieces[CASTLE_PIECE_WEIGHTS_COUNT];
    int corridorPiecesCount;
    
    Piece *pendingChildren[1024];
    int pendingChildrenCount;
    
    Piece pieces[1024];
    int piecesCount;
    
    PieceType lastPieceType;
    int32_t startX;
    int32_t startZ;
    Random random;
} FortressGenerator;

int32_t fortressGenerator_getRegionSize(Version version);

void fortressGenerator_generateForRegion(FortressGenerator *fortressGenerator, uint64_t structureSeed, int32_t regionX, int32_t regionZ, int32_t salt, Version version);

void fortressGenerator_generateForChunk(FortressGenerator *fortressGenerator, uint64_t structureSeed, int32_t chunkX, int32_t chunkZ, Version version);

void fortressGenerator_placeJigsaw(FortressGenerator *fortressGenerator, Piece *piece);

void fortressGenerator_generateChildForward(FortressGenerator *fortressGenerator, Piece *piece, int32_t i, int32_t j, bool isCorridor);

void fortressGenerator_generateChildLeft(FortressGenerator *fortressGenerator, Piece *piece, int32_t i, int32_t j, bool isCorridor);

void fortressGenerator_generateChildRight(FortressGenerator *fortressGenerator, Piece *piece, int32_t i, int32_t j, bool isCorridor);

void fortressGenerator_generateAndAddPiece(FortressGenerator *fortressGenerator, int32_t x, int32_t y, int32_t z, Direction direction, int32_t l, bool isCorridor);

Piece *fortressGenerator_generateRandomPiece(FortressGenerator *fortressGenerator, int32_t x, int32_t y, int32_t z, Direction direction, int32_t length, bool isCorridor);

bool fortressGenerator_isOkBox(FortressGenerator *fortressGenerator, BlockBox *blockBox);

Piece *fortressGenerator_generatePiece(FortressGenerator *fortressGenerator, PieceType pieceType, int32_t x, int32_t y, int32_t z, Direction direction, int32_t length);