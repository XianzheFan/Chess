#ifndef GAME_H
#define GAME_H
#include "field.h"
#include "chessboard.h"
#include "gamemove.h"
#include "king.h"
#include "button.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QString>
#include <QTextEdit>

#include <QTimer>
#include <QElapsedTimer>
#include <timepanel.h>


//游戏主界面，设置相关按钮控件和游戏规则
class Game : public QGraphicsView {
    Q_OBJECT
public:
    Game();
    ~Game();
    QString getTurn();
    void setTurn(const QString & turn);
    void changeTurn();
    void placeInDeadPlace(ChessPiece * piece);
    void gameover(const QString & winner);
    void displayDeadWhite();
    void displayDeadBlack();
    void displayMainMenu();
    void checkForCheck();
    void checkForCheckmateOrStalemate();
    void disableMouseEventsWhenPanelIsShown();
    void enableMouseEventsAfterClosingPanel();
    void updateHistory(const QString & move);
    void updateMoveCounter();
    void resetMoveCounter();
    bool isWhiteKingChecked();
    bool isBlackKingChecked();
    bool pawnAttack(King *a);
    bool horizontalAttack(King *a);
    bool verticalAttack(King *a);
    bool diagonalAttack(King *a);
    bool knightAttack(King *a);
    void displayPromotion(const QString & side);
    bool isAlive(ChessPiece * piece);
    bool nextToKing(Field* field, const QString & kingSide);
    QGraphicsRectItem* drawPanel(int x, int y, int width, int height, const QColor & color, double opacity);
    QGraphicsScene * scene;
    QList <ChessPiece *> alivePieces;
    QList <GameMove *> moves;
    Chessboard * board;
    ChessPiece * selectedPiece;
    QGraphicsPixmapItem * checkDisplay;
    bool moveValidationCheck;
    bool enpassantAvailable;
    Field * enpassantField;
    ChessPiece * deadInEnpassant;
    QList<ChessPiece *> promotionPieces;
    QGraphicsRectItem * panel;
    QString moveCode;
public slots:
    void start();
    void restart();
    void previousState();
    void surrender();
    void draw();
    void acceptDraw();
    void declineDraw();
    void onUndo();
private:
    QScrollArea * scrollArea;
    QGraphicsProxyWidget * scrollableHistoryWindow;
    QTextEdit * movesHistoryDisplay;
    King * whiteKing;
    King * blackKing;
    QList <ChessPiece *> whiteDead;
    QList <ChessPiece *> blackDead;
    QGraphicsPixmapItem * turnImage;
    QString m_turn;
    Button *restartButton;
    Button *undoButton;
    Button *surrenderButton;
    Button *drawButton;
    int numOfMovesWithoutEating;

    TimePanel* timePanelW;
    TimePanel* timePanelB;
    TimePanel* timePanelDuration;
};
#endif // GAME_H
