#ifndef QUEEN_H
#define QUEEN_H
#include "chesspiece.h"

//后类：加载图片、可行域、种类
class Queen : public ChessPiece {
public:
    Queen(QString side, bool promotionPiece = false);
    ~Queen() = default;
    void setImage();
    void generatePossibleMoves();
    QString type();

};

#endif // QUEEN_H
