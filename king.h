#ifndef KING_H
#define KING_H
#include "chesspiece.h"

//王类：加载图片、种类、可行域、是否被将
class King : public ChessPiece {
public:
    King(QString side);
    ~King() = default;
    QString type();
    void setImage();
    void generatePossibleMoves();
    void setChecked(bool value);
    bool checked();
private:
    bool m_isChecked;
};

#endif // KING_H
