#ifndef GAMEMOVE_H
#define GAMEMOVE_H
#include "chesspiece.h"
#include <QBrush>

//涉及王车易位、被吃子、兵升变、将军
class GameMove {
public:
    GameMove(ChessPiece * movedPiece, Field * from, Field * to, bool wasItCheck, bool didItCauseCheck, bool wasItFirstMove, bool hasEatenSomething, ChessPiece * deadPiece, Field * deadPieceField, bool eatenPieceFirstMove, bool smallCastling, bool largeCastling);
    ChessPiece * m_movedPiece;
    Field * m_from;
    Field * m_to;
    bool m_wasMovedInCheck;
    bool m_wasOpponentInCheck;
    bool m_wasItFirstMove;
    bool m_hasEatenSomething;
    ChessPiece * m_deadPiece;
    Field * m_deadPieceField;
    bool m_eatenPieceFirstMove;
    bool m_smallCastling;
    bool m_largeCastling;
    ChessPiece * promotionPiece;
};

#endif // GAMEMOVE_H
