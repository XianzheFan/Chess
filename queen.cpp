#include "queen.h"
#include "game.h"
extern Game * game;
Queen::Queen(QString side, bool promotionPiece) : ChessPiece (side, promotionPiece)
{
    setImage();
}

QString Queen::type()
{
    return "QUEEN";
}

void Queen::setImage()
{
    if(m_side == "WHITE") {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/WhiteQueen.png"));
    } else {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/BlackQueen.png"));
    }
}

void Queen::generatePossibleMoves()
{
    m_possibleMoves.clear();
    int row = this->currentField()->row();
    int col = this->currentField()->column();

    //后（Q）：横、直、斜都可以走，移动步数不受限制，但不能转向或越过其他棋子
    //后=车+象
    //车
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

    //象

    //右上
    for(int i = row - 1, j = col + 1; i >= 0 && j <= 7; i--, j++) {
        if(!game->board->fieldAt(i, j)->isItFree()
                && game->board->fieldAt(i, j)->currentPiece->side() == this->side())
            break;    
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
            if(!game->board->fieldAt(i, j)->isItFree())
                break;
        } else {
            if(colorAvailableFields(m_possibleMoves.last()))
                break;
        }
    }

    //左上
    for(int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if(!game->board->fieldAt(i, j)->isItFree()
                && game->board->fieldAt(i, j)->currentPiece->side() == this->side())
            break;
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
            if(!game->board->fieldAt(i, j)->isItFree())
                break;
        } else {
            if(colorAvailableFields(m_possibleMoves.last()))
                break;
        }
    }

    //左下
    for(int i = row + 1, j = col - 1; i <= 7 && j >= 0; i++, j--) {
        if(!game->board->fieldAt(i, j)->isItFree()
                && game->board->fieldAt(i, j)->currentPiece->side() == this->side())
            break;
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
            if(!game->board->fieldAt(i, j)->isItFree())
                break;
        } else {
            if(colorAvailableFields(m_possibleMoves.last()))
                break;
        }
    }

    //右下
    for(int i = row + 1, j = col + 1; i <= 7 && j <= 7; i++, j++) {
        if(!game->board->fieldAt(i, j)->isItFree()
                && game->board->fieldAt(i, j)->currentPiece->side() == this->side())
            break;
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
            if(!game->board->fieldAt(i, j)->isItFree())
                break;
        } else {
            if(colorAvailableFields(m_possibleMoves.last()))
                break;
        }
    }
}
