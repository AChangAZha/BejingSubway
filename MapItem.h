/*
 * @Author: AChangAZha
 * @Date: 2021-11-23 14:29:03
 * @LastEditTime: 2021-12-07 19:56:27
 * @LastEditors: AChangAZha
 */
#ifndef MAPITEM_H
#define MAPITEM

#include <QGraphicsEllipseItem>
#include <QPen>

class CStation;   //站点类声明
class CIntersite; //站点连接类声明
class MainWindow; //主窗口

class MapItem : public QObject, public QGraphicsEllipseItem, public QGraphicsLineItem //线路图元素
{
    Q_OBJECT

public:
    enum ItemType //元素类型
    {
        circle = 0, // 圆
        picture,    //换乘
        solidLine,  //实线
        dotteLine,  //虚线
        rectangle,  // 矩形
        arrows,     //箭头
        from,       //起点
        to,         //终点
        pos,        //定位

    };
    //构造函数
    MapItem(MainWindow *mainWindow, ItemType type, CStation *sta = NULL, CIntersite *link = NULL, QGraphicsItem *parent = Q_NULLPTR);
    //绘制
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    //点击站点
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    //双击空白
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //计算站点x坐标
    double GetX(double lon);
    //计算站点y坐标
    double GetY(double lat);

    ItemType type;          //元素类型
    QPen p;                 //画笔
    CStation *sta;          //当前站点
    CIntersite *link;       //当前站点连接
    MainWindow *mainWindow; //主窗口
};

#endif // MAPITEM_H
