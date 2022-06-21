#include "field.h"
#include "game.h"
#include "king.h"
#include "queen.h"
#include "knight.h"
#include "bishop.h"
#include "rook.h"
#include <QBrush>
extern Game * game;
//格子大小和颜色
Field::Field(int row, int column, const QString & color, int width, int height)
    : currentPiece(nullptr),
      m_width(width),
      m_height(height),
      m_rowLocation(row),
      m_columnLocation(column),
      m_isItFree(true)
{
    setZValue(-1);
    setRect(0, 0, m_width, m_height);
    if (color == "BLACK")
        m_originalColor.setRgb(0,250,250,250);
    else
        m_originalColor.setRgb(0,75,135,185);
    setBrush(m_originalColor);
}


int Field::width() const
{
    return m_width;
}

int Field::height() const
{
    return m_height;
}

QString Field::toString() const
{
    return QString(char('a' + column()))
            + QString(8 - row() + '0');
}

int Field::row() const
{
    return m_rowLocation;
}

int Field::column() const
{
    return m_columnLocation;
}

void Field::setIsItFree(bool value)
{
    m_isItFree = value;
}

void Field::placePiece(ChessPiece * piece)
{
    piece->setPos(x(), y());
    piece->setCurrentField(this);
    m_isItFree = false;
    currentPiece = piece;
}

void Field::resetOriginalColor()
{
    setBrush(m_originalColor);
}

bool Field::isItFree() const
{
    return m_isItFree;
}

void Field::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //如果选中棋子
    if(game->selectedPiece) {
        bool smallCastling = false;
        bool largeCastling = false;
        if(game->selectedPiece->type() == "KING"
                && !this->isItFree()
                && this->currentPiece->type() == "ROOK"
                && game->selectedPiece->side() == this->currentPiece->side()) {
            if(this->column() == 7)
                smallCastling = true;
            if(this->column() == 0)
                largeCastling = true;
        }

        //同方
        if(!smallCastling
                && !largeCastling
                && !this->isItFree()
                && this->currentPiece->side() == game->selectedPiece->side()) {
            if(this->currentPiece == game->selectedPiece)
                this->currentPiece->mousePressEvent(event);
            return ;
        }


        if(!game->selectedPiece->possibleMoves().contains(this))
            return ;
        QString fromField = game->selectedPiece->currentField()->toString();
        bool wasItCheck = game->checkDisplay->isVisible();
        bool firstMove = game->selectedPiece->firstMove();
        bool wasItFree = this->isItFree();
        ChessPiece * selected = game->selectedPiece;
        bool eatenFigureFirstMove = false;

        if(!this->isItFree()) {
            eatenFigureFirstMove = this->currentPiece->firstMove();
        }

        bool wasItEnpassant = false;  //吃过路兵
        if(game->enpassantAvailable && this == game->enpassantField) {
            wasItEnpassant = true;
            game->moves.push_back(
                        new GameMove(selected, selected->currentField(), this, wasItCheck, game->checkDisplay->isVisible(),
                                     firstMove, true, game->deadInEnpassant, game->deadInEnpassant->currentField(), eatenFigureFirstMove, smallCastling, largeCastling));
            game->deadInEnpassant->currentField()->setIsItFree(true);
            game->deadInEnpassant->setCurrentField(nullptr);
            game->placeInDeadPlace(game->deadInEnpassant);
        } else {
            game->moves.push_back(
                        new GameMove(selected, selected->currentField(), this, wasItCheck, game->checkDisplay->isVisible(), firstMove,
                                     !wasItFree, this->currentPiece, this, eatenFigureFirstMove, smallCastling, largeCastling));
        }

        if(!this->isItFree() && !smallCastling && !largeCastling) {
            this->currentPiece->setCurrentField(nullptr);
            game->placeInDeadPlace(this->currentPiece);
        }

        if(!game->moveValidationCheck) {
            game->enpassantAvailable = false;
            game->enpassantField = nullptr;
        }
        game->selectedPiece->setFirstMove(false);
        game->selectedPiece->currentField()->setIsItFree(true);
        game->selectedPiece->currentField()->currentPiece = nullptr;
        game->selectedPiece->currentField()->resetOriginalColor();

        if(smallCastling) {
            int row = game->selectedPiece->currentField()->row();
            ChessPiece * rook = game->board->fieldAt(row, 7)->currentPiece;
            game->board->fieldAt(row, 6)->placePiece(game->selectedPiece);
            game->board->fieldAt(row, 5)->placePiece(rook);
            game->board->fieldAt(row, 7)->setIsItFree(true);
            game->board->fieldAt(row, 7)->currentPiece = nullptr;
        } else if (largeCastling){
            int row = game->selectedPiece->currentField()->row();
            ChessPiece * rook = game->board->fieldAt(row, 0)->currentPiece;
            game->board->fieldAt(row, 2)->placePiece(game->selectedPiece); // c格
            game->board->fieldAt(row, 3)->placePiece(rook);  // d格
            game->board->fieldAt(row, 0)->setIsItFree(true);
            game->board->fieldAt(row, 0)->currentPiece = nullptr;
        } else {
                placePiece(game->selectedPiece);
        }

        // 兵升变
        if(game->selectedPiece->type() == "PAWN"
                && (this->row() == 0
                    || this->row() == 7)
                && !game->moveValidationCheck) {
            game->displayPromotion(game->selectedPiece->side());

        }

        game->selectedPiece = nullptr;
        game->changeTurn();
        game->checkForCheck();
        if(!game->moveValidationCheck)
            game->checkForCheckmateOrStalemate();
        game->moves.last()->m_wasOpponentInCheck = game->checkDisplay->isVisible();
        if(!game->moveValidationCheck) {
            game->updateMoveCounter();
            selected->decolorAvailableFields();
            if(smallCastling) {
                game->updateHistory("O-O");
            } else if(largeCastling) {
                game->updateHistory("O-O-O");
            } else {
                game->moveCode.clear();
                QString pieceType = selected->type();
                if(pieceType != "PAWN") {
                    if(pieceType == "KNIGHT")
                        game->moveCode.append("N");
                    else {
                        game->moveCode.append(pieceType.front());
                    }
                }
                game->moveCode.append(fromField);
                if(!wasItFree)
                    game->moveCode.append("x");
                game->moveCode.append(this->toString());
                if(wasItEnpassant)
                    game->moveCode.append("e.p");
                if(game->checkDisplay->isVisible())
                    game->moveCode.append("+");
                if(selected->type() == "PAWN"
                                && (this->row() == 0
                                    || this->row() == 7))
                    game->moveCode.append("=");
                else
                    game->updateHistory(game->moveCode);
            }
        }
    } else if(!this->isItFree()) {
        this->currentPiece->mousePressEvent(event);
    }
}

