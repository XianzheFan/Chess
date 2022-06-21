#include "bishop.h"
#include "game.h"
extern Game * game;

//常规的构造函数、析构函数、加载图像
Bishop::Bishop(QString side, bool promotionPiece) : ChessPiece (side, promotionPiece)
{
    setImage();
}


QString Bishop::type()
{
    return "BISHOP";
}

void Bishop::setImage()
{
    if(m_side == "WHITE") {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/WhiteBishop.png"));
    } else {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/BlackBishop.png"));
    }
}

//可行域
//只能斜走。格数不限，不能越子。因此白格象只能在白格走动，黑格象只能在黑格走动。
//每局开始时，每一方有双象，一在黑格，一在白格
void Bishop::generatePossibleMoves()
{
    m_possibleMoves.clear();
    int row = this->currentField()->row();
    int col = this->currentField()->column();

    //右上
    for(int i = row - 1, j = col + 1; i >= 0 && j <= 7; i--, j++) {
        if(!game->board->fieldAt(i, j)->isItFree()
                && game->board->fieldAt(i, j)->currentPiece->side() == this->side()) {
            break;
        }
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
                && game->board->fieldAt(i, j)->currentPiece->side() == this->side()) {
            break;
        }
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
                && game->board->fieldAt(i, j)->currentPiece->side() == this->side()) {
            break;
        }
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            if(colorAvailableFields(m_possibleMoves.last()))
                break;
        }

    }

    //右下
    for(int i = row + 1, j = col + 1; i <= 7 && j <= 7; i++, j++) {
        if(!game->board->fieldAt(i, j)->isItFree()
                && game->board->fieldAt(i, j)->currentPiece->side() == this->side()){
            break;
        }
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
