#include "chessboard.h"
#include "game.h"
#include "bishop.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "queen.h"
#include "rook.h"
extern Game * game;

//棋盘格摆放、黑白方棋子图片放置
Chessboard::Chessboard(int width, int height)
    : m_fields{{nullptr}},
      m_width(width),
      m_height(height)
{
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            Field * f = nullptr;
            if((i + j) % 2)
                f = new Field(i, j, "BLACK");
            else
                f = new Field(i, j, "WHITE");

            f->setPos(x()+80 + f->width() * j, y() + f->height() * i);
            m_fields.at(i).at(j) = f;
        }
    }
    setWhitePieces();
    setBlackPieces();
}


Chessboard::~Chessboard()
{
    for(auto & row : m_fields)
        for(auto & field : row)
            delete field;
}

void Chessboard::setWhitePieces()
{

    ChessPiece * piece;
    for(int i = 0; i < 8; i++) {
        piece = new Pawn("WHITE");
        white.append(piece);
    }
    piece = new Rook("WHITE");
    white.append(piece);
    piece = new Knight("WHITE");
    white.append(piece);
    piece = new Bishop("WHITE");
    white.append(piece);
    piece = new Queen("WHITE");
    white.append(piece);
    piece = new King("WHITE");
    white.append(piece);
    piece = new Bishop("WHITE");
    white.append(piece);
    piece = new Knight("WHITE");
    white.append(piece);
    piece = new Rook("WHITE");
    white.append(piece);


}

void Chessboard::setBlackPieces()
{
    ChessPiece * piece;

    piece = new Rook("BLACK");
    black.append(piece);
    piece = new Knight("BLACK");
    black.append(piece);
    piece = new Bishop("BLACK");
    black.append(piece);
    piece = new Queen("BLACK");
    black.append(piece);
    piece = new King("BLACK");
    black.append(piece);
    piece = new Bishop("BLACK");
    black.append(piece);
    piece = new Knight("BLACK");
    black.append(piece);
    piece = new Rook("BLACK");
    black.append(piece);
    for(int i = 0; i < 8; i++) {
        piece = new Pawn("BLACK");
        black.append(piece);
    }
}

void Chessboard::drawFields() const
{
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            game->scene->addItem(fieldAt(i, j));
}

//向容器里增加棋子并区分种类
void Chessboard::drawChessPieces() const
{

    int k = 0;
    int h = 0;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            Field * f = fieldAt(i, j);
            if(i < 2) {
                f->placePiece(black[k]);
                game->alivePieces.append(black[k]);
                game->scene->addItem(black[k++]);
            }

            if(i > 5) {
                f->placePiece(white[h]);
                game->alivePieces.append(white[h]);
                game->scene->addItem(white[h++]);
            }
        }
    }
}

Field *Chessboard::fieldAt(int i, int j) const
{
    return m_fields.at(i).at(j);
}
