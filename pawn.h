#ifndef PAWN_H
#define PAWN_H
#include "chesspiece.h"

//兵类：加载图片、可行域、种类
class Pawn : public ChessPiece {
public:
    Pawn(QString side);
    ~Pawn() = default;
    void setImage();
    void generatePossibleMoves();
    QString type();
};

#endif // PAWN_H
