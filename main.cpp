#include <QApplication>
#include "mainwindow.h"
#include "game.h"

Game *game;
//程序入口
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //程序图标
    a.setWindowIcon(QIcon(":/icon/images/icon.png"));
    game = new Game();
    game->show();
    game->displayMainMenu();
    return a.exec();
}
