#include "game.h"
#include "king.h"
#include "button.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include <QTimer>
#include <QTextEdit>
#include <QDebug>
#include <QtMath>
#include <QGraphicsProxyWidget>

#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 700
#define BUTTON_WIDTH 250
#define BUTTON_HEIGHT 86
#define BUTTON_MARGIN 10


Game::Game()
{
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowTitle("Chess");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(scene);
    setFixedSize(WINDOW_WIDTH+1, WINDOW_HEIGHT+2);
}

Game::~Game()
{
    delete movesHistoryDisplay;
    delete board;
    delete scrollArea;
    delete scrollableHistoryWindow;
    delete movesHistoryDisplay;
    delete checkDisplay;
    delete restartButton;
    delete undoButton;
    delete surrenderButton;
    delete drawButton;
    delete panel;
    delete turnImage;
    for(auto item : alivePieces)
        delete item;
    for(auto item : whiteDead)
        delete item;
    for(auto item : blackDead)
        delete item;
}

//用图片提示现在是谁在走棋
void Game::setTurn(const QString & turn)
{
    m_turn = turn;
    if(turn == "WHITE"){
        turnImage->setPixmap(QPixmap(":/moves/images/Moves/whiteMove.png"));
    }
    else{
        turnImage->setPixmap(QPixmap(":/moves/images/Moves/blackMove.png"));
    }
}

QString Game::getTurn()
{
    return m_turn;
}

void Game::changeTurn()
{

    if(getTurn() == "WHITE"){
        setTurn("BLACK");
    }

    else{
        setTurn("WHITE");;
    }
}

void Game::placeInDeadPlace(ChessPiece *piece)
{
    if(!moveValidationCheck)
        resetMoveCounter();
    if(piece->side() == "WHITE") {
        whiteDead.append(piece);
        displayDeadWhite();
    } else {
        blackDead.append(piece);
        displayDeadBlack();
    }
    alivePieces.removeAll(piece);
}

//判断胜负（包括平局）
void Game::gameover(const QString & winner)
{
   disableMouseEventsWhenPanelIsShown();
   drawPanel(0, 0, width(), height(), Qt::black, 0.90);

   auto gameOverText = new QGraphicsPixmapItem();
   gameOverText->setPixmap(QPixmap(":/winners/images/Winners/" + winner + "Won.png"));
   gameOverText->setPos(width()/2.0-gameOverText->boundingRect().width()/2.0, 220);
   scene->addItem(gameOverText);

   auto playAgain = new Button(QString(":/buttons/images/Buttons/restartButton.png"), BUTTON_WIDTH, BUTTON_HEIGHT);
   playAgain->setPos(width()/2.0 - BUTTON_WIDTH - BUTTON_MARGIN, height()/2.0 + 50);
   connect(playAgain, SIGNAL(clicked()), this, SLOT(restart()));
   scene->addItem(playAgain);

   auto quit = new Button(QString(":/buttons/images/Buttons/quitButton.png"), BUTTON_WIDTH, BUTTON_HEIGHT);
   quit->setPos(width()/2.0 + BUTTON_MARGIN, height()/2.0 + 50);
   connect(quit, SIGNAL(clicked()), this, SLOT(close()));
   scene->addItem(quit);
}
//摆放被吃的白/黑子
void Game::displayDeadWhite()
{
    int shift = 80;
    int j = 0;
    int k = 0;

    for(int i = 0, n = whiteDead.size(); i < n; i++) {
        if(j == 4) {
            k++;
            j = 0;
        }
        whiteDead[i]->setPos(9 * board->fieldAt(0,0)->width() + shift * j ++, scene->height() - shift*(k+1));
    }
}

void Game::displayDeadBlack()
{
    int shift = 80;
    int j = 0;
    int k = 0;

    for(int i = 0, n = blackDead.size(); i < n; i++) {
        if(j == 4) {
            k++;
            j = 0;
        }
        blackDead[i]->setPos(9 * board->fieldAt(0,0)->width() + shift * j++, shift*k);
    }
}
//游戏主界面
void Game::displayMainMenu()
{
    auto background = new QGraphicsPixmapItem();
    background->setPixmap(QPixmap(":/background/images/background.png"));
    scene->addItem(background);

    auto playButton = new Button(QString(":/buttons/images/Buttons/playButton.png"), BUTTON_WIDTH, BUTTON_HEIGHT);
    int playButtonXPos = int(this->width()/2.0 - playButton->boundingRect().width()/2.0 - 200);
    int playButtonYPos = 450;
    playButton->setPos(playButtonXPos, playButtonYPos);
    connect(playButton,SIGNAL(clicked()),this,SLOT(start()));
    scene->addItem(playButton);

    auto quitButton = new Button(QString(":/buttons/images/Buttons/quitButton.png"), BUTTON_WIDTH, BUTTON_HEIGHT);
    int quitButtonXPos = int(this->width()/2.0 - quitButton->boundingRect().width()/2.0 + 200);
    int quitButtonYPos = 450;
    quitButton->setPos(quitButtonXPos, quitButtonYPos);
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(quitButton);
}

void Game::checkForCheck()
{
    //判断是否有将军
    //对角移动、左、右、上、下（在车范围内）
    //是否在对方控制范围内

     bool isBlackChecked = pawnAttack(blackKing)
                         || horizontalAttack(blackKing)
                         || verticalAttack(blackKing)
                         || diagonalAttack(blackKing)
                         || knightAttack(blackKing);
     if(isBlackChecked) {
         blackKing->setChecked(true);
         if(!moveValidationCheck) {
            blackKing->currentField()->setBrush(Qt::lightGray);
         }
     } else {
         blackKing->setChecked(false);
         if(!moveValidationCheck)
            blackKing->currentField()->resetOriginalColor();
     }


     bool whiteIsItChecked = pawnAttack(whiteKing)
                         || horizontalAttack(whiteKing)
                         || verticalAttack(whiteKing)
                         || diagonalAttack(whiteKing)
                         || knightAttack(whiteKing);

     if(whiteIsItChecked) {
         whiteKing->setChecked(true);
         if(!moveValidationCheck)
            whiteKing->currentField()->setBrush(Qt::lightGray);
     }  else {
         whiteKing->setChecked(false);
         if(!moveValidationCheck)
            whiteKing->currentField()->resetOriginalColor();
     }

     if(whiteIsItChecked || isBlackChecked) {
         checkDisplay->setVisible(true);
     } else {
         checkDisplay->setVisible(false);
     }

}


void Game::checkForCheckmateOrStalemate()
{
    bool whiteIsChecked = whiteKing->checked();
    bool blackIsChecked = blackKing->checked();

    if(numOfMovesWithoutEating == 50)
        gameover("none");

    //只剩两个王
    if(alivePieces.size() == 2) {
        gameover("none");
    }

    //有王，且车/象在攻击王
    if(alivePieces.size() == 3) {
        if(std::any_of(alivePieces.begin(), alivePieces.end(), [](ChessPiece * a){return a->type() == "KNIGHT" || a->type() == "BISHOP";}))
            gameover("none");
    }

    //有王，且两车攻击王
    if(alivePieces.size() == 4) {
        if(std::count_if(alivePieces.begin(), alivePieces.end(), [](ChessPiece * a) {return a->type() == "KNIGHT" && a->side() == "WHITE";}) == 2
         || std::count_if(alivePieces.begin(), alivePieces.end(), [](ChessPiece * a) {return a->type() == "KNIGHT" && a->side() == "BLACK";}) == 2)
            gameover("none");
    }

    //迭代未被吃的子，如果都不能动
    //根据是否有将军判断死局
    bool areTherePossibleMoves = false;
    moveValidationCheck = false;
    auto currentAlivePieces = alivePieces;
    for(auto item : currentAlivePieces) {
        if(item->side() == getTurn()) {
            item->generatePossibleMoves();
            item->decolorAvailableFields();
            if(!item->possibleMoves().empty()) {
                areTherePossibleMoves = true;
                break;
            }
        }
    }
    if(!areTherePossibleMoves) {
        if(getTurn() == "WHITE" && whiteIsChecked) {
            gameover("black");
        } else if(getTurn() == "BLACK" && blackIsChecked) {
            gameover("white");
        } else {
            gameover("none");
        }
    }
}

void Game::disableMouseEventsWhenPanelIsShown()
{
    //主窗口启动时关闭鼠标事件
    undoButton->setEnabled(false);
    restartButton->setEnabled(false);
    surrenderButton->setEnabled(false);
    drawButton->setEnabled(false);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            board->fieldAt(i, j)->setEnabled(false);
        }
    }

    for(auto item : alivePieces)
        item->setEnabled(false);
    for(auto item : whiteDead)
        item->setEnabled(false);
    for(auto item : blackDead)
        item->setEnabled(false);

}

void Game::enableMouseEventsAfterClosingPanel()
{
    //在子窗口关闭后重启鼠标事件（子窗口弹出时，不能移动棋子）
    undoButton->setEnabled(true);
    restartButton->setEnabled(true);
    surrenderButton->setEnabled(true);
    drawButton->setEnabled(true);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            board->fieldAt(i, j)->setEnabled(true);
        }
    }

    for(auto item : alivePieces)
        item->setEnabled(true);
    for(auto item : whiteDead)
        item->setEnabled(true);
    for(auto item : blackDead)
        item->setEnabled(true);
}

void Game::updateHistory(const QString & move)
{
    QTextCursor cursor = movesHistoryDisplay->textCursor();
    if(getTurn() == "WHITE") {
        cursor.movePosition(QTextCursor::End);
        cursor.insertText(move + "\n");
        movesHistoryDisplay->setTextCursor(cursor);
        timePanelB->stop();
        timePanelW->start();
    } else {
        cursor.movePosition(QTextCursor::End);
        cursor.insertText(move + "\t");
        movesHistoryDisplay->setTextCursor(cursor);
        timePanelW->stop();
        timePanelB->start();
    }
}

void Game::updateMoveCounter()
{
    numOfMovesWithoutEating++;
}

void Game::resetMoveCounter()
{
    numOfMovesWithoutEating = 0;
}

bool Game::isWhiteKingChecked()
{
    return whiteKing->checked();
}

bool Game::isBlackKingChecked()
{
    return blackKing->checked();
}
//兵攻击王
bool Game::pawnAttack(King *a)
{
    bool isItChecked = false;
    int row = a->currentField()->row();
    int col = a->currentField()->column();
    if(a->side() == "BLACK") {
        if(row + 1 < 8
                && col - 1 >= 0
                && !board->fieldAt(row+1, col-1)->isItFree()
                && board->fieldAt(row+1, col-1)->currentPiece->side() =="WHITE"
                && board->fieldAt(row+1, col-1)->currentPiece->type() == "PAWN")
            isItChecked = true;

        if(row + 1 < 8
                && col + 1 < 8
                && !board->fieldAt(row+1, col+1)->isItFree()
                && board->fieldAt(row+1, col+1)->currentPiece->side() == "WHITE"
                && board->fieldAt(row+1, col+1)->currentPiece->type() == "PAWN")
            isItChecked = true;
    } else {
        if(row - 1 >= 0
                && col - 1 >= 0
                && !board->fieldAt(row-1, col-1)->isItFree()
                && board->fieldAt(row-1, col-1)->currentPiece->side() == "BLACK"
                && board->fieldAt(row-1, col-1)->currentPiece->type() == "PAWN")
            isItChecked = true;
        if(row - 1 >= 0
                && col + 1 < 8
                && !board->fieldAt(row-1, col+1)->isItFree()
                && board->fieldAt(row-1, col+1)->currentPiece->side() == "BLACK"
                && board->fieldAt(row-1, col+1)->currentPiece->type() == "PAWN")
            isItChecked = true;
    }

    return isItChecked;
}
//横斩
bool Game::horizontalAttack(King *a)
{

    bool isItChecked = false;
    int row = a->currentField()->row();
    int col = a->currentField()->column();
    for(int i = col + 1; i < 8; i++) {
        if(!board->fieldAt(row, i)->isItFree()
                && board->fieldAt(row, i)->currentPiece->side() == a->side())
           break;

        if(!board->fieldAt(row, i)->isItFree()
                && board->fieldAt(row, i)->currentPiece->side() != a->side()) {
            if(board->fieldAt(row, i)->currentPiece->type() == "QUEEN"
                || board->fieldAt(row, i)->currentPiece->type() == "ROOK") {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }

    }

    for(int i = col - 1; i >= 0; i--) {
        if(!board->fieldAt(row, i)->isItFree()
                && board->fieldAt(row, i)->currentPiece->side() == a->side())
           break;

        if(!board->fieldAt(row, i)->isItFree()
                && board->fieldAt(row, i)->currentPiece->side() != a->side()) {
            if(board->fieldAt(row, i)->currentPiece->type() == "QUEEN"
                || board->fieldAt(row, i)->currentPiece->type() == "ROOK") {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }
    }

    return isItChecked;
}
//竖斩
bool Game::verticalAttack(King *a)
{
    bool isItChecked = false;
    int row = a->currentField()->row();
    int col = a->currentField()->column();

    for(int i = row + 1; i < 8; i++) {
        if(!board->fieldAt(i, col)->isItFree()
                && board->fieldAt(i, col)->currentPiece->side() == a->side())
           break;

        if(!board->fieldAt(i, col)->isItFree()
                && board->fieldAt(i, col)->currentPiece->side() != a->side()) {
            if((board->fieldAt(i, col)->currentPiece->type() == "QUEEN"
                    || board->fieldAt(i, col)->currentPiece->type() == "ROOK")) {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }
    }

    for(int i = row - 1; i >= 0; i--) {
        if(!board->fieldAt(i, col)->isItFree()
                && board->fieldAt(i, col)->currentPiece->side() == a->side())
           break;

        if(!board->fieldAt(i, col)->isItFree()
                && board->fieldAt(i, col)->currentPiece->side() != a->side()) {
            if(board->fieldAt(i, col)->currentPiece->type() == "QUEEN"
                   || board->fieldAt(i, col)->currentPiece->type() == "ROOK") {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }

    }

    return isItChecked;
}
//对角斩
bool Game::diagonalAttack(King *a)
{
    bool isItChecked = false;
    int row = a->currentField()->row();
    int col = a->currentField()->column();

    for(int i = row + 1, j = col + 1; i < 8 && j < 8; i++, j++) {
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() ==  a->side())
           break;
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() != a->side()) {
            if(board->fieldAt(i, j)->currentPiece->type() == "QUEEN"
                || board->fieldAt(i, j)->currentPiece->type() == "BISHOP") {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }
    }

    for(int i = row + 1, j = col - 1; i < 8 && j >= 0; i++, j--) {
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() == a->side())
           break;
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() != a->side()) {
            if(board->fieldAt(i, j)->currentPiece->type() == "QUEEN"
                || board->fieldAt(i, j)->currentPiece->type() == "BISHOP") {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }

    }

    for(int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() == a->side())
           break;
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() != a->side()) {
            if(board->fieldAt(i, j)->currentPiece->type() == "QUEEN"
                || board->fieldAt(i, j)->currentPiece->type() == "BISHOP") {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }
    }

    for(int i = row - 1, j = col + 1; i >= 0 && j < 8; i--, j++) {
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() == a->side())
           break;
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() != a->side()) {
            if(board->fieldAt(i, j)->currentPiece->type() == "QUEEN"
                || board->fieldAt(i, j)->currentPiece->type() == "BISHOP") {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }
    }

    return isItChecked;
}
//车攻击
bool Game::knightAttack(King *a)
{
    bool isItChecked = false;
    int row = a->currentField()->row();
    int col = a->currentField()->column();

    //可能的车攻击范围
    int i = row - 2;
    int j = col + 1;

    if(i >= 0
            && j < 8
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }

    i = row - 2;
    j = col - 1;

    if(i >= 0
            && j >= 0
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }

    i = row - 1;
    j = col + 2;

    if(i >= 0
            && j < 8
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }


    i = row - 1;
    j = col - 2;

    if(i >= 0
            && j >= 0
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }


    i = row + 1;
    j = col - 2;

    if(i < 8
            && j >= 0
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }

    i = row + 1;
    j = col + 2;

    if(i < 8
            && j < 8
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }

    i = row + 2;
    j = col - 1;

    if(i < 8
            && j >= 0
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }

    i = row + 2;
    j = col + 1;

    if(i < 8
            && j < 8
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }

    return isItChecked;
}
//兵升变
void Game::displayPromotion(const QString & side)
{
    panel = drawPanel(0, 0, width(), height(), Qt::black, 0.90);
    scene->addItem(panel);
    disableMouseEventsWhenPanelIsShown();
    auto pickPromotionText = new QGraphicsPixmapItem();
    pickPromotionText->setPixmap(QPixmap(":/moves/images/pickPromotion.png"));
    pickPromotionText->setPos(width()/2.0-pickPromotionText->boundingRect().width()/2.0, 220);
    pickPromotionText->setParentItem(panel);
    scene->addItem(pickPromotionText);

    promotionPieces.append(new Queen(side, true));
    scene->addItem(promotionPieces[0]);

    promotionPieces[0]->setPos(width()/2.0 - 175, height()/2.0 + 50);
    promotionPieces.append(new Rook(side, true));

    scene->addItem(promotionPieces[1]);
    promotionPieces[1]->setPos(width()/2.0 - 175 + 90, height()/2.0 + 50);

    promotionPieces.append(new Bishop(side, true));
    scene->addItem(promotionPieces[2]);

    promotionPieces[2]->setPos(width()/2.0 - 175 + 180, height()/2.0 + 50);
    promotionPieces.append(new Knight(side, true));

    scene->addItem(promotionPieces[3]);
    promotionPieces[3]->setPos(width()/2.0 - 175 + 270, height()/2.0 + 50);
}

bool Game::isAlive(ChessPiece *piece)
{
    if(piece->side() == "WHITE" && whiteDead.contains(piece))
        return false;
    if(piece->side() == "BLACK" && blackDead.contains(piece))
        return false;
    return true;
}

bool Game::nextToKing(Field *field, const QString &kingSide)
{
    King * currentKing = nullptr;
    if(kingSide == "WHITE")
        currentKing = whiteKing;
    else
        currentKing = blackKing;

    if(field->row() >= 0
            && field->row() <= 7
            && field->column() >= 0
            && field->column() <= 7
            && qFabs(currentKing->currentField()->row() - field->row()) <= 1
            && qFabs(currentKing->currentField()->column() - field->column()) <= 1)
        return true;
    return false;
}

QGraphicsRectItem *Game::drawPanel(int x, int y, int width, int height, const QColor & color, double opacity)
{
    //游戏结束、逼和等子窗口的背景
       auto panel = new QGraphicsRectItem(x, y, width, height);
       QBrush brush;
       brush.setStyle(Qt::SolidPattern);
       brush.setColor(color);
       panel->setBrush(brush);
       panel->setOpacity(opacity);
       scene->addItem(panel);

       //放按钮
       auto panel2 = new QGraphicsPixmapItem();
       panel2->setPixmap(QPixmap(":/textures/images/Textures/panelBackground.png").scaled(600, 400));
       panel2->setPos(x+this->width()/2.0-300, y+this->height()/2.0-200);
       panel2->setOpacity(1);

       scene->addItem(panel2);
       panel2->setParentItem(panel);

       return panel;

}

void Game::previousState()
{
    if(!moves.isEmpty()) {  // move是否为空串
        //回放，但不增加走子记录（只恢复）
        if(!moveValidationCheck) {
            QTextCursor cursor = movesHistoryDisplay->textCursor();
            cursor.movePosition(QTextCursor::End);
            if(getTurn() == "BLACK") {
                cursor.movePosition(QTextCursor::End);
                cursor.select(QTextCursor::LineUnderCursor);
                cursor.removeSelectedText();
                movesHistoryDisplay->setTextCursor(cursor);
            } else {
                QString newText = movesHistoryDisplay->toPlainText();
                newText.truncate(newText.lastIndexOf('\t'));
                movesHistoryDisplay->clear();
                movesHistoryDisplay->setText(newText + '\t');
            }
        }


        GameMove * lastMove = moves.last();

        //落子
        setTurn(lastMove->m_movedPiece->side());
        lastMove->m_from->placePiece(lastMove->m_movedPiece);
        lastMove->m_movedPiece->setFirstMove(lastMove->m_wasItFirstMove);

        if(!moveValidationCheck) {
            if(selectedPiece) {
                selectedPiece->decolorAvailableFields();
                selectedPiece->currentField()->resetOriginalColor();
                selectedPiece = nullptr;
            }
        }
        //重置 'to' field
        lastMove->m_to->setIsItFree(true);
        lastMove->m_to->currentPiece = nullptr;
        //在走步之前被将
        checkDisplay->setVisible(lastMove->m_wasMovedInCheck);
        if(lastMove->m_wasMovedInCheck) {
            if(lastMove->m_movedPiece->side() == "WHITE") {
                whiteKing->currentField()->setBrush(Qt::lightGray);
             } else {
                blackKing->currentField()->setBrush(Qt::lightGray);
            }
        } else if(!lastMove->m_wasMovedInCheck) {
            if(lastMove->m_movedPiece->side() == "WHITE") {
                whiteKing->currentField()->resetOriginalColor();
             } else {
                blackKing->currentField()->resetOriginalColor();
            }
        }

        //在落子后吃了对方的子
        if(lastMove->m_hasEatenSomething) {
            if(lastMove->m_deadPiece->side() == "WHITE") {
                whiteDead.removeAll(lastMove->m_deadPiece);
            } else {
                blackDead.removeAll(lastMove->m_deadPiece);
            }
            alivePieces.push_back(lastMove->m_deadPiece);
            lastMove->m_deadPiece->setFirstMove(lastMove->m_eatenPieceFirstMove);
            lastMove->m_deadPieceField->placePiece(lastMove->m_deadPiece);
            lastMove->m_deadPiece->setPos(lastMove->m_deadPieceField->x(), lastMove->m_deadPieceField->y());
        }

        if(lastMove->m_wasOpponentInCheck) {
            if(lastMove->m_movedPiece->side() == "WHITE") {
                blackKing->currentField()->resetOriginalColor();
                blackKing->setChecked(false);
            } else {
                whiteKing->currentField()->resetOriginalColor();
                whiteKing->setChecked(false);
            }
        }


        if(moveValidationCheck) {
            if(lastMove->m_movedPiece->type() == "KING") {
                King * p = dynamic_cast<King*>(lastMove->m_movedPiece);
                if(p->checked())
                    p->setChecked(false);
            }
        }



        //取消小易位
        if(lastMove->m_smallCastling) {
            int row = lastMove->m_movedPiece->currentField()->row();
            board->fieldAt(row, 5)->setIsItFree(true);
            board->fieldAt(row, 6)->setIsItFree(true);
        }

        //取消长易位
        if(lastMove->m_largeCastling) {
            int row = lastMove->m_movedPiece->currentField()->row();
            board->fieldAt(row, 1)->setIsItFree(true);
            board->fieldAt(row, 2)->setIsItFree(true);
            board->fieldAt(row, 3)->setIsItFree(true);
        }

        //取消兵升变
        if(lastMove->m_movedPiece->type() == "PAWN"
                && (lastMove->m_to->row() == 7
                    || lastMove->m_to->row() == 0)
                && !moveValidationCheck){
            alivePieces.removeAll(lastMove->promotionPiece);
            if(lastMove->m_movedPiece->side() == "WHITE")
                whiteDead.removeAll(lastMove->m_movedPiece);
            else {
                blackDead.removeAll(lastMove->m_movedPiece);
            }
            scene->removeItem(lastMove->promotionPiece);
            promotionPieces.clear();
        }
        moves.removeLast();
    }
}
//投子认负
void Game::surrender()
{
    QString winner = "none";
    if(getTurn() == "WHITE") {
        winner = "black";
    } else {
        winner = "white";
    }
        gameover(winner);
}

void Game::draw()
{
    disableMouseEventsWhenPanelIsShown();
    panel = drawPanel(0, 0, width(), height(), Qt::black, 0.90);

    auto askForDrawText = new QGraphicsPixmapItem();
    askForDrawText->setPixmap(QPixmap(":/moves/images/Moves/offeredDraw.png"));
    askForDrawText->setPos(width()/2.0-askForDrawText->boundingRect().width()/2.0, 220);
    askForDrawText->setParentItem(panel);
    scene->addItem(askForDrawText);

    auto acceptDraw = new Button(QString(":/buttons/images/Buttons/acceptButton.png"), BUTTON_WIDTH, BUTTON_HEIGHT);
    acceptDraw->setPos(width()/2.0 - 260, height()/2.0 + 50);
    connect(acceptDraw, SIGNAL(clicked()), this, SLOT(acceptDraw()));
    acceptDraw->setParentItem(panel);
    scene->addItem(acceptDraw);

    auto declineDraw = new Button(QString(":/buttons/images/Buttons/declineButton.png"), BUTTON_WIDTH, BUTTON_HEIGHT);
    declineDraw->setPos(width()/2.0 + 10, height()/2.0 + 50);
    connect(declineDraw, SIGNAL(clicked()), this, SLOT(declineDraw()));
    declineDraw->setParentItem(panel);
    scene->addItem(declineDraw);
}
//同意和棋申请，平局
void Game::acceptDraw()
{
    scene->removeItem(panel);
    gameover("none");
}
//取消
void Game::declineDraw()
{
    enableMouseEventsAfterClosingPanel();
    scene->removeItem(panel);
    delete panel;
}
//倒计时和回放的配合
void Game::onUndo()
{
    auto size = moves.size();
    if(size > 0){
        if(getTurn() == "WHITE") {
            timePanelW->stop();
            timePanelB->start();
        } else {
            timePanelB->stop();
            timePanelW->start();
        }
    }
}
//开始，初始化所有控件
void Game::start()
{
    scene->clear();
    moveValidationCheck = false;
    enpassantAvailable = false;
    numOfMovesWithoutEating = 0;

    undoButton = new Button(QString(":/buttons/images/Buttons/undoButton.png"), BUTTON_WIDTH, BUTTON_HEIGHT);
    undoButton->setPos(width() - BUTTON_WIDTH - BUTTON_MARGIN, BUTTON_MARGIN);
    scene->addItem(undoButton);
    connect(undoButton, SIGNAL(clicked()), this, SLOT(onUndo()));
    connect(undoButton, SIGNAL(clicked()), this, SLOT(previousState()));

    restartButton = new Button(QString(":/buttons/images/Buttons/restartButton.png"), BUTTON_WIDTH, BUTTON_HEIGHT);
    restartButton->setPos(width() - BUTTON_WIDTH - BUTTON_MARGIN, 2 * BUTTON_MARGIN + BUTTON_HEIGHT);
    scene->addItem(restartButton);
    connect(restartButton, SIGNAL(clicked()), this, SLOT(restart()));

    surrenderButton = new Button(QString(":/buttons/images/Buttons/surrenderButton.png"), BUTTON_WIDTH, BUTTON_HEIGHT);
    surrenderButton->setPos(width() - BUTTON_WIDTH - BUTTON_MARGIN, 3 * BUTTON_MARGIN + 2 * BUTTON_HEIGHT);
    scene->addItem(surrenderButton);
    connect(surrenderButton, SIGNAL(clicked()), this, SLOT(surrender()));

    drawButton = new Button(QString(":/buttons/images/Buttons/drawButton.png"), BUTTON_WIDTH, BUTTON_HEIGHT);
    drawButton->setPos(width() - BUTTON_WIDTH - BUTTON_MARGIN, 4 * BUTTON_MARGIN + 3 * BUTTON_HEIGHT);
    scene->addItem(drawButton);
    connect(drawButton, SIGNAL(clicked()), this, SLOT(draw()));

    checkDisplay = new QGraphicsPixmapItem();
    checkDisplay->setPixmap(QPixmap(":/moves/images/Moves/check.png"));
    checkDisplay->setPos(WINDOW_WIDTH / 2 - checkDisplay->boundingRect().width(), WINDOW_HEIGHT - checkDisplay->boundingRect().height() + 1);
    checkDisplay->setVisible(false);
    scene->addItem(checkDisplay);

    auto movesBackground = new QGraphicsPixmapItem();
    movesBackground->setPixmap(QPixmap(":/background/images/movesBackground.png"));
    movesBackground->setPos(width() - BUTTON_WIDTH - BUTTON_MARGIN, 5 * BUTTON_MARGIN + 4 * BUTTON_HEIGHT);
    scene->addItem(movesBackground);

    scrollArea = new QScrollArea();
    movesHistoryDisplay = new QTextEdit();
    movesHistoryDisplay->setReadOnly(true);
    movesHistoryDisplay->setFixedWidth(190);
    movesHistoryDisplay->setFixedHeight(230);
    movesHistoryDisplay->setFrameShape(QFrame::NoFrame);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidget(movesHistoryDisplay);
    scrollArea->setFixedWidth(190);
    scrollableHistoryWindow = scene->addWidget(scrollArea);
    scrollableHistoryWindow->setPos(width() - scrollArea->width() - 14, 5 * BUTTON_MARGIN + 4 * BUTTON_HEIGHT + 66);

    board = new Chessboard();
    board->drawFields();
    board->drawChessPieces();
    selectedPiece = nullptr;
    whiteKing = dynamic_cast<King*>(board->fieldAt(7, 4)->currentPiece);
    blackKing = dynamic_cast<King*>(board->fieldAt(0, 4)->currentPiece);

    turnImage = new QGraphicsPixmapItem();
    turnImage->setPixmap(QPixmap(":/moves/images/Moves/whiteMove.png"));
    turnImage->setPos(2, WINDOW_HEIGHT - turnImage->boundingRect().height());
    scene->addItem(turnImage);

    timePanelW = new TimePanel;
    timePanelW->setCountDown(true);
    scene->addItem(timePanelW);
    timePanelW->setPos(0,150);
    timePanelW->setRole("white");

    timePanelDuration = new TimePanel;
    scene->addItem(timePanelDuration);
    timePanelDuration->setPos(0,250);
    timePanelDuration->setRole("游戏时长");
    timePanelDuration->start();

    timePanelB = new TimePanel;
    timePanelB->setCountDown(true);
    scene->addItem(timePanelB);
    timePanelB->setPos(0,350);
    timePanelB->setRole("black");
    setTurn("WHITE");
    timePanelW->start();

}
//重玩
void Game::restart()
{
    enableMouseEventsAfterClosingPanel();
    moves.clear();
    alivePieces.clear();
    blackDead.clear();
    whiteDead.clear();
    promotionPieces.clear();
    movesHistoryDisplay->clear();
    QTimer::singleShot(1, this, SLOT(start()));
}
