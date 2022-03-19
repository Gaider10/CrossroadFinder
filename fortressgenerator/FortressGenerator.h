#pragma once

#include "util/Version.h"
#include "util/BlockBox.h"
#include "util/Direction.h"
#include "util/Random.h"
#include "Piece.h"
#include "PieceData.h"
#include "PieceType.h"

#define FORTRESS_SPACING 27
#define FORTRESS_SEPARATION 4
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
    int startX;
    int startZ;
    Random random;
} FortressGenerator;

void fortressGenerator_generateForRegion(FortressGenerator *fortressGenerator, long long structureSeed, int regionX, int regionZ, int salt, Version version);

void fortressGenerator_generateForChunk(FortressGenerator *fortressGenerator, long long structureSeed, int chunkX, int chunkZ);

void fortressGenerator_placeJigsaw(FortressGenerator *fortressGenerator, Piece *piece);

void fortressGenerator_generateChildForward(FortressGenerator *fortressGenerator, Piece *piece, int i, int j, int bl);

void fortressGenerator_generateChildLeft(FortressGenerator *fortressGenerator, Piece *piece, int i, int j, int bl);

void fortressGenerator_generateChildRight(FortressGenerator *fortressGenerator, Piece *piece, int i, int j, int bl);

void fortressGenerator_generateAndAddPiece(FortressGenerator *fortressGenerator, int x, int y, int z, Direction direction, int l, int bl);

Piece *fortressGenerator_generateRandomPiece(FortressGenerator *fortressGenerator, int x, int y, int z, Direction direction, int length, int isCorridor);

int fortressGenerator_isOkBox(FortressGenerator *fortressGenerator, BlockBox *blockBox);

Piece *fortressGenerator_generatePiece(FortressGenerator *fortressGenerator, PieceType pieceType, int x, int y, int z, Direction direction, int length);