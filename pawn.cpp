#include "pawn.h"
#include "game.h"
extern Game * game;
Pawn::Pawn(QString side) : ChessPiece(side)
{
    setImage();
}


QString Pawn::type()
{
    return "PAWN";
}

void Pawn::setImage()
{
    if(m_side == "WHITE") {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/WhitePawn.png"));
    } else {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/BlackPawn.png"));
    }
}


//兵（P）：只能向前直走，从第二步开始每次只能走一格。但走第一步时，可以走一格或两格。
//兵的吃子方法与行棋方向不一样，它是直走斜吃，即如果兵的斜进一格内有对方棋子，就可以吃掉它而占据该格。
void Pawn::generatePossibleMoves()
{
    m_possibleMoves.clear();
    int row = this->currentField()->row();
    int col = this->currentField()->column();
    if(this->side() == "WHITE") {
        if(col > 0 && row > 0
                && !game->board->fieldAt(row-1, col-1)->isItFree()
                && game->board->fieldAt(row-1, col-1)->currentPiece->side() == "BLACK") { //attacking upper left
            m_possibleMoves.append(game->board->fieldAt(row-1, col-1));

            if(!isLegalMove(game->board->fieldAt(row-1, col-1))) {
                m_possibleMoves.removeLast();
            } else {
                colorAvailableFields(m_possibleMoves.last());
            }

        }

        if(col < 7 && row > 0
                && !game->board->fieldAt(row-1, col+1)->isItFree()
                && game->board->fieldAt(row-1, col+1)->currentPiece->side() == "BLACK") { // attacking upper right
            m_possibleMoves.append(game->board->fieldAt(row-1, col+1));
            if(!isLegalMove(game->board->fieldAt(row-1, col+1))) {
                m_possibleMoves.removeLast();
            } else {
                colorAvailableFields(m_possibleMoves.last());
            }
        }

        if(row > 0 && game->board->fieldAt(row-1, col)->isItFree()) { // going upward
            m_possibleMoves.append(game->board->fieldAt(row-1, col));
            if(!isLegalMove(game->board->fieldAt(row-1, col))) {
                m_possibleMoves.removeLast();
            } else {
                colorAvailableFields(m_possibleMoves.last());
            }

            if(firstMove() && game->board->fieldAt(row-2, col)->isItFree()) {
                m_possibleMoves.append(game->board->fieldAt(row-2, col));
                if(!isLegalMove(game->board->fieldAt(row-2, col))) {
                    m_possibleMoves.removeLast();
                } else {

                    colorAvailableFields(m_possibleMoves.last());
                }
            }
        }

        if(row == 3
                && game->moves.last()->m_to->row() == 3
                && (game->moves.last()->m_to->column() == col - 1
                    || game->moves.last()->m_to->column() == col + 1)
                && game->moves.last()->m_movedPiece->type() == "PAWN"
                && game->moves.last()->m_wasItFirstMove) {
            game->enpassantAvailable = true;
            game->enpassantField = game->board->fieldAt(2, game->moves.last()->m_to->column());
            game->deadInEnpassant = game->moves.last()->m_movedPiece;
            m_possibleMoves.append(game->board->fieldAt(2, game->moves.last()->m_to->column()));
            if(!isLegalMove(game->board->fieldAt(2, game->moves.last()->m_to->column()))) {
                m_possibleMoves.removeLast();
            } else {
                colorAvailableFields(m_possibleMoves.last());
            }
        }

    } else {

        if(col > 0 && row < 7
                && !game->board->fieldAt(row+1, col-1)->isItFree()
                && game->board->fieldAt(row+1, col-1)->currentPiece->side() == "WHITE") {
            m_possibleMoves.append(game->board->fieldAt(row+1, col-1));
            if(!isLegalMove(game->board->fieldAt(row+1, col-1))) {
                m_possibleMoves.removeLast();
            } else {
                colorAvailableFields(m_possibleMoves.last());
            }
        }
        if(col < 7 && row < 7
                && !game->board->fieldAt(row+1, col+1)->isItFree()
                && game->board->fieldAt(row+1, col+1)->currentPiece->side() == "WHITE") {

            m_possibleMoves.append(game->board->fieldAt(row+1, col+1));
            if(!isLegalMove(game->board->fieldAt(row+1, col+1))) {
                m_possibleMoves.removeLast();
            } else {
                colorAvailableFields(m_possibleMoves.last());
            }
        }
        if(row < 7 && game->board->fieldAt(row+1, col)->isItFree()) {

            m_possibleMoves.append(game->board->fieldAt(row+1, col));
            if(!isLegalMove(game->board->fieldAt(row+1, col))) {
                m_possibleMoves.removeLast();
            } else {
                colorAvailableFields(m_possibleMoves.last());
            }
            if(firstMove() && game->board->fieldAt(row+2, col)->isItFree()) {
                m_possibleMoves.append(game->board->fieldAt(row+2, col));
                if(!isLegalMove(game->board->fieldAt(row+2, col))) {
                    m_possibleMoves.removeLast();
                } else {
                    colorAvailableFields(m_possibleMoves.last());
                }
            }
        }
        if(row == 4
                && game->moves.last()->m_to->row() == 4
                && (game->moves.last()->m_to->column() == col - 1
                    || game->moves.last()->m_to->column() == col + 1)
                && game->moves.last()->m_movedPiece->type() == "PAWN"
                && game->moves.last()->m_wasItFirstMove) {
            game->enpassantAvailable = true;
            game->enpassantField = game->board->fieldAt(5, game->moves.last()->m_to->column());
            game->deadInEnpassant = game->moves.last()->m_movedPiece;
            m_possibleMoves.append(game->board->fieldAt(5, game->moves.last()->m_to->column()));
            if(!isLegalMove(game->board->fieldAt(5, game->moves.last()->m_to->column()))) {
                m_possibleMoves.removeLast();
            } else {
                colorAvailableFields(m_possibleMoves.last());
            }
        }

    }
}
