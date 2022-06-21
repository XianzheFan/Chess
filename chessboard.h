#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include "field.h"
#include "chesspiece.h"
#include <QVector>
#include <QVBoxLayout>

//棋盘类
class Chessboard : public QGraphicsRectItem{
public:
    Chessboard(int width = 800, int height = 800);
    ~Chessboard();
    void setWhitePieces();
    void setBlackPieces();
    void drawFields() const;
    void drawChessPieces() const;
    Field * fieldAt(int i, int j) const;
private:
    std::array<std::array<Field *, 8>, 8> m_fields;
//声明QList类变量（容器）
    QList <ChessPiece *> white;
    QList <ChessPiece *> black;
    int m_width;
    int m_height;
};

#endif // CHESSBOARD_H
