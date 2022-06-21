#include "timepanel.h"

#include <QGraphicsItem>
#include <QPainter>

#include "game.h"
extern Game * game;

TimePanel::TimePanel(QGraphicsItem *parent) : QGraphicsItem(parent)
{
    QObject::connect(&timerCount,&QTimer::timeout,[this](){
         if(isStart){
            secCount ++;
         }

         update();
    });
    timerCount.start(1000);
}
//倒计时开始/结束
void TimePanel::start()
{
    timer.restart();
    isStart = true;
}

void TimePanel::stop()
{
    isStart = false;
    secCount = 0;
}
//绘制相关
void TimePanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing,true);
    QPen pen(Qt::black);
    auto font = painter->font();
    font.setBold(true);
    font.setPixelSize(20);
    painter->setFont(font);
    painter->setPen(pen);

    painter->drawRect(QRect(0,0,80,80));
    painter->drawText(QPoint(0,20),role);
    if(countdown){
        if(isStart){
            if(secCount < 90){
                painter->drawText(QPoint(10,40),QString::number(90 - secCount).append(" S"));
            }
            else{
                if(role == "white"){
                    game->gameover("black");
                }
                else if(role == "black"){
                    game->gameover("white");
                }

                painter->drawText(QPoint(10,40),"0 S");
            }
        }
        else{
            painter->drawText(QPoint(10,40),"0 S");
        }
    }
    else{
        painter->drawText(QPoint(10,40),QString::number(secCount).append(" S"));
    }
}

void TimePanel::setRole(const QString &role)
{
    this->role = role;
}

QRectF TimePanel::boundingRect() const
{
    return QRectF(0, 0, 100, 100);
}

void TimePanel::setCountDown(bool enable)
{
    countdown = enable;
}
