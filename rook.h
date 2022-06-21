#ifndef ROOK_H
#define ROOK_H
#include "chesspiece.h"

//车类：加载图片、种类、可行域
class Rook : public ChessPiece {
public:
    Rook(QString side, bool promotionPiece = false);
    ~Rook() = default;
    void setImage();
    void generatePossibleMoves();
    QString type();

};

#endif // ROOK_H
