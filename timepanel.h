#ifndef TIMEPANEL_H
#define TIMEPANEL_H

#include <QGraphicsItem>
#include <QElapsedTimer>
#include <QTimer>


//时间面板：继承矩形类，其中有计时器、文本显示（倒计时+总时长）
class TimePanel : public QGraphicsItem
{
public:
    explicit TimePanel(QGraphicsItem *parent = nullptr);
    void start();
    void stop();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void setRole(const QString& role);
    virtual QRectF boundingRect() const;
    void setCountDown(bool enable);
private:
    void paintText(QPainter *painter);

private:
    QElapsedTimer timer;
    QTimer timerCount;
    QString role;
    bool isStart{false};
    bool countdown{false};
    quint64 secCount{0};
};

#endif // TIMEPANEL_H
