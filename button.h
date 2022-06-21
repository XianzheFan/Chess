#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

//按钮类：外加框框，有鼠标点击事件
class Button: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
//必加的宏
public:
    Button(const QString & path, int width = 250, int height = 86, QGraphicsItem* parent=nullptr);
    ~Button() = default;
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
signals:
    void clicked();
};
#endif // BUTTON_H
