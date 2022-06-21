#ifndef FIELD_H
#define FIELD_H
#include <QGraphicsRectItem>
#include <QBrush>
class Field;
#include "chesspiece.h"
//矩形类，为小格子
class Field : public QGraphicsRectItem {
public:
    Field(int row, int column, const QString & color, int width = 80, int height = 80);
    ~Field() = default;
    int width() const;
    int height() const;
    int row() const;
    int column() const;
    void setIsItFree(bool value);
    void placePiece(ChessPiece * piece);
    void resetOriginalColor();
    void mousePressEvent(QGraphicsSceneMouseEvent *event = nullptr);
    bool isItFree() const;
    QString toString() const;

    ChessPiece * currentPiece;
private:
    int m_width;
    int m_height;
    int m_rowLocation;
    int m_columnLocation;
    bool m_isItFree;
    QColor m_originalColor;
};

#endif // FIELD_H
