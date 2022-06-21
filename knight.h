#ifndef KNIGHT_H
#define KNIGHT_H
#include "chesspiece.h"

//马类：可行域、加载图片、种类
class Knight : public ChessPiece {
public:
    Knight(QString side, bool promotionPiece = false);
    ~Knight() = default;
    void setImage();
    void generatePossibleMoves();
    QString type();
};

#endif // KNIGHT_H
