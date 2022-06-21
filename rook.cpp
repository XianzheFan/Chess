#include "rook.h"
#include "game.h"
#include <QPixmap>
extern Game * game;
Rook::Rook(QString side, bool promotionPiece) : ChessPiece (side, promotionPiece)
{
    setImage();
}


QString Rook::type()
{
    return "ROOK";
}

void Rook::setImage()
{
    if(m_side == "WHITE") {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/WhiteRook.png"));
    } else {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/BlackRook.png"));
    }
}
//车（R）：横、竖均可以走，步数不受限制，不能斜走。除王车易位外不能越子

void Rook::generatePossibleMoves()
{
    m_possibleMoves.clear();
    int row = this->currentField()->row();
    int col = this->currentField()->column();

    for(int i = row - 1; i >= 0; i--) { // 上移
        if(!game->board->fieldAt(i, col)->isItFree()
                && game->board->fieldAt(i, col)->currentPiece->side() == this->side())
            break;
        m_possibleMoves.append(game->board->fieldAt(i, col));
        if(!isLegalMove(game->board->fieldAt(i, col))) {
            m_possibleMoves.removeLast();
            if(!game->board->fieldAt(i, col)->isItFree())
                break;
        } else {
            if(colorAvailableFields(m_possibleMoves.last()))
                break;
        }
    }

    for(int i = row + 1; i <= 7; i++) { //下移
        if(!game->board->fieldAt(i, col)->isItFree()
                && game->board->fieldAt(i, col)->currentPiece->side() == this->side())
            break;
        m_possibleMoves.append(game->board->fieldAt(i, col));
        if(!isLegalMove(game->board->fieldAt(i, col))) {
            m_possibleMoves.removeLast();
            if(!game->board->fieldAt(i, col)->isItFree())
                break;
        } else {
            if(colorAvailableFields(m_possibleMoves.last()))
                break;
        }
    }

    for(int j = col - 1; j >= 0; j--) { //左移
        if(!game->board->fieldAt(row, j)->isItFree()
                && game->board->fieldAt(row, j)->currentPiece->side() == this->side())
            break;
        m_possibleMoves.append(game->board->fieldAt(row, j));
        if(!isLegalMove(game->board->fieldAt(row, j))) {
            m_possibleMoves.removeLast();
            if(!game->board->fieldAt(row, j)->isItFree())
                break;
        } else {
            if(colorAvailableFields(m_possibleMoves.last()))
                break;
        }
    }

    for(int j = col + 1; j <= 7; j++) {
        if(!game->board->fieldAt(row, j)->isItFree()
                && game->board->fieldAt(row, j)->currentPiece->side() == this->side())
            break;
        m_possibleMoves.append(game->board->fieldAt(row, j));
        if(!isLegalMove(game->board->fieldAt(row, j))) {
            m_possibleMoves.removeLast();
            if(!game->board->fieldAt(row, j)->isItFree())
                break;
        } else {
            if(colorAvailableFields(m_possibleMoves.last()))
                break;
        }
    }

}
