#ifndef MAPITEM_H
#define MAPITEM

#include <QGraphicsEllipseItem>
#include <QPen>

class CStation;
class CIntersite;
class MainWindow;
class MapItem : public QObject, public QGraphicsEllipseItem, public QGraphicsLineItem
{
    Q_OBJECT

public:
    enum ItemType
    {
        circle = 0, // 圆
        picture,
        solidLine,
        dotteLine,
        rectangle, // 矩形
        arrows,
        from,
        to,
        pos,

    };
    ItemType type;
    QPen p;
    CStation *sta;
    CIntersite *link;
    MapItem(MainWindow *mainWindow, ItemType type, CStation *sta = NULL, CIntersite *link = NULL, QGraphicsItem *parent = Q_NULLPTR);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //计算站点x坐标
    double GetX(double lon);
    //计算站点y坐标
    double GetY(double lat);
    MainWindow *mainWindow;
};

#endif // MAPITEM_H
