#include "knight.h"
#include "game.h"
extern Game * game;
Knight::Knight(QString side, bool promotionPiece) : ChessPiece(side, promotionPiece)
{
    setImage();
}


QString Knight::type()
{
    return "KNIGHT";
}

void Knight::setImage()
{
    if(m_side == "WHITE") {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/WhiteKnight.png"));
    } else {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/BlackKnight.png"));
    }
}
//马 每步棋先横走或直走一格，然后再往外斜走一格；或者先斜走一格，
//最后再往外横走或竖走一格（即走“日”字）。可以越子，没有中国象棋中的“蹩马腿”限制
void Knight::generatePossibleMoves()
{
    m_possibleMoves.clear();
    int row = this->currentField()->row();
    int col = this->currentField()->column();

    int i, j;
    //上上左
    i = row - 2;
    j = col - 1;
    if(i >= 0 && j >= 0 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //上上右
    i = row - 2;
    j = col + 1;
    if(i >= 0 && j <= 7 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //上右右
    i = row - 1;
    j = col + 2;
    if(i >= 0 && j <= 7 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //上左左
    i = row - 1;
    j = col - 2;
    if(i >= 0 && j >= 0 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //下左左
    i = row + 1;
    j = col - 2;
    if(i <= 7 && j >= 0 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //下右右
    i = row + 1;
    j = col + 2;
    if(i <= 7 && j <= 7 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //下下右
    i = row + 2;
    j = col + 1;
    if(i <= 7 && j <= 7 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //下下左
    i = row + 2;
    j = col - 1;
    if(i <= 7 && j >= 0 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }


}
