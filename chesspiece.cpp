#include "chesspiece.h"
#include "field.h"
#include "game.h"
#include "king.h"
#include <QDebug>

#include <QTimer>
extern Game *game;
ChessPiece::ChessPiece(QString & side, bool promotionPiece)
    : m_side(side),
      m_promotionPiece(promotionPiece),
      m_currentField(nullptr),
      m_currentFieldRow(-1),
      m_currentFieldColumn(-1)
{
    m_firstMove = true;
    promotionPiece = false;
}

ChessPiece::~ChessPiece()
{
    game->scene->removeItem(this);
}

void ChessPiece::setCurrentField(Field *currentField)
{
    if(currentField) {
        m_currentFieldRow = currentField->row();
        m_currentFieldColumn = currentField->column();
    } else {
        m_currentFieldRow = -1;
        m_currentFieldColumn = -1;
    }
}

void ChessPiece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(this == game->selectedPiece) {
        game->selectedPiece->decolorAvailableFields();
        game->selectedPiece->currentField()->resetOriginalColor();

        //判断被将
        if(this->type() == "KING")
            game->checkForCheck();
        game->selectedPiece = nullptr;
        return ;
    }
    if(!game->isAlive(this))
        return;

    if(game->getTurn() != side() && !game->selectedPiece && !this->m_promotionPiece)
        return ;

    if(!game->selectedPiece && !this->m_promotionPiece) {
        game->selectedPiece = this;
        game->selectedPiece->generatePossibleMoves();
        game->selectedPiece->currentField()->setBrush(Qt::yellow);
    } else if(game->selectedPiece && !this->m_promotionPiece){
        this->currentField()->mousePressEvent(event);
    }
    if(this->m_promotionPiece) {
        game->enableMouseEventsAfterClosingPanel();
        game->moves.last()->promotionPiece = this;
        game->placeInDeadPlace(game->moves.last()->m_movedPiece);
        game->moves.last()->m_to->placePiece(this);
        game->alivePieces.push_back(this);
        game->scene->removeItem(game->panel);
        for(auto item : game->promotionPieces)
            if(item != this) {
                game->scene->removeItem(item);
            }
        this->m_promotionPiece = false;
        setFirstMove(false);
        game->promotionPieces.clear();
        game->checkForCheck();
        game->checkForCheckmateOrStalemate();
        game->moveCode.append(this->type().front());
        if(game->checkDisplay->isVisible())
            game->moveCode.append("+");
        game->updateHistory(game->moveCode);
    }
}

Field *ChessPiece::currentField()
{
    if(m_currentFieldRow != -1 && m_currentFieldColumn != -1)
        return game->board->fieldAt(m_currentFieldRow, m_currentFieldColumn);
    return nullptr;
}

void ChessPiece::decolorAvailableFields()
{
    for(auto & move : m_possibleMoves)
        move->resetOriginalColor();
}

QString ChessPiece::side()
{
    return m_side;
}

//被将时王的颜色变化
bool ChessPiece::colorAvailableFields(Field *field)
{
    if(!field->isItFree()) {
        King * k = dynamic_cast<King*>(field->currentPiece);
        if(k) {
            field->setBrush(Qt::lightGray);
        } else {
            field->setBrush(Qt::yellow);
        }
        return true;
    }
    m_possibleMoves.last()->setBrush(Qt::yellow);
    return false;
}

QList<Field *> ChessPiece::possibleMoves()
{
    return m_possibleMoves;
}

//是否能行棋
bool ChessPiece::isLegalMove(Field * field)
{
    if(game->moveValidationCheck)
        return true;
    ChessPiece * oldSelectedPiece = game->selectedPiece;
    game->selectedPiece = this;
    game->moveValidationCheck = true;
    field->mousePressEvent();
    bool returnValue = true;
    if(side() == "WHITE" && game->isWhiteKingChecked()) {
        returnValue = false;
    } else if (side() == "BLACK" && game->isBlackKingChecked()) {
        returnValue = false;
    }
    game->selectedPiece = oldSelectedPiece;
    game->previousState();
    game->moveValidationCheck = false;
    return returnValue;
}

bool ChessPiece::firstMove()
{
    return m_firstMove;
}

void ChessPiece::setFirstMove(bool firstMove)
{
    m_firstMove = firstMove;
}

