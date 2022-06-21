#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#include <QGraphicsPixmapItem>
class ChessPiece;
#include "field.h"
//棋子总类，涉及走法、坐标、种类等
class ChessPiece : public QGraphicsPixmapItem {
public:
    ChessPiece(QString & side, bool promotionPiece = false);
    ~ChessPiece();
    virtual QString type() = 0;
    virtual void generatePossibleMoves() = 0;
    void mousePressEvent(QGraphicsSceneMouseEvent * event = nullptr);
    void setCurrentField(Field * currentField);
    void decolorAvailableFields();
    void setFirstMove(bool firstMove);
    bool colorAvailableFields(Field *field);
    bool isLegalMove(Field * field);
    bool firstMove();
    Field * currentField();
    QString side();
    QList <Field*> possibleMoves();
protected:
    QString m_side;
    QList <Field *> m_possibleMoves;
private:
    bool m_firstMove;
    bool m_promotionPiece;
    Field * m_currentField;
    int m_currentFieldRow;
    int m_currentFieldColumn;
};

#endif // CHESSPIECE_H
