#include "gamemove.h"
GameMove::GameMove(ChessPiece *movedPiece, Field *from, Field *to, bool wasItCheck, bool didItCauseCheck, bool wasItFirstMove, bool hasEatenSomething, ChessPiece *deadPiece, Field * deadPieceField, bool eatenPieceFirstMove, bool smallCastling, bool largeCastling)
    : m_movedPiece(movedPiece),
      m_from(from),
      m_to(to),
      m_wasMovedInCheck(wasItCheck),
      m_wasOpponentInCheck(didItCauseCheck),
      m_wasItFirstMove(wasItFirstMove),
      m_hasEatenSomething(hasEatenSomething),
      m_deadPiece(deadPiece),
      m_deadPieceField(deadPieceField),
      m_eatenPieceFirstMove(eatenPieceFirstMove),
      m_smallCastling(smallCastling),
      m_largeCastling(largeCastling)
{

}
//构造函数
