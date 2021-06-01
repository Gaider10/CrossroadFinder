#include "PieceData.h"

#include "PieceType.h"

const PieceData BRIDGE_PIECE_WEIGHTS[BRIDGE_PIECE_WEIGHTS_COUNT] = {
    { BRIDGE, 30, 0, 1, 0 },
    { BRIDGE_CROSSING, 10, 4, 0, 0 },
    { BRIDGE_SMALL_CROSSING, 10, 4, 0, 0 },
    { BRIDGE_STAIRS, 10, 3, 0, 0 },
    { BRIDGE_PLATFORM, 5, 2, 0, 0 },
    { CORRIDOR_EXIT, 5, 1, 0, 0 }
};
const PieceData CASTLE_PIECE_WEIGHTS[CASTLE_PIECE_WEIGHTS_COUNT] = {
    { SMALL_CORRIDOR, 25, 0, 1, 0 },
    { CORRIDOR_CROSSING, 15, 5, 0, 0 },
    { CORRIDOR_RIGHT_TURN, 5, 10, 0, 0 },
    { CORRIDOR_LEFT_TURN, 5, 10, 0, 0 },
    { CORRIDOR_STAIRS, 10, 3, 1, 0 },
    { CORRIDOR_BALCONY, 7, 2, 0, 0 },
    { CORRIDOR_NETHER_WARTS_ROOM, 5, 2, 0, 0 }
};

int pieceData_isInvalid(PieceData *pieceData) {
    return pieceData->maxPlaceCount != 0 && pieceData->placeCount >= pieceData->maxPlaceCount;
}