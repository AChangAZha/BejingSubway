#include "MapItem.h"
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QObject>
#include "CStation.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "StaButton.h"
#include "MainWindow.h"
#include "CSubwayMap.h"
#include "CIntersite.h"
#include "CLine.h"
#include "Search.h"
#include "ui_Search.h"
#include "ui_MainWindow.h"
#include <QtMath>

MapItem::MapItem(MainWindow *mainWindow, ItemType type, CStation *sta, CIntersite *link, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent)
{
    this->sta = sta;
    this->link = link;
    this->type = type;
    this->mainWindow = mainWindow;
    if (type == circle)
    {
        p.setColor(mainWindow->map->GetLine(sta->GetLine())->GetLineColor());
        double x = GetX(sta->GetLongitude());
        double y = GetY(sta->GetLatitude());
        setRect(x - 11, y - 11, 22, 22);
    }
    else if (type == picture)
    {
        double x = GetX(sta->GetLongitude());
        double y = GetY(sta->GetLatitude());
        setRect(x - 12.5, y - 12.5, 25, 25);
    }
    else if (type == solidLine || type == dotteLine)
    {
        p.setColor(mainWindow->map->GetLine(sta->GetLine())->GetLineColor());
        double x1 = GetX(sta->GetLongitude());
        double y1 = GetY(sta->GetLatitude());
        double x2 = GetX(link->GetStation()->GetLongitude());
        double y2 = GetY(link->GetStation()->GetLatitude());
        setLine(x1, y1, x2, y2);
    }
    else if (type == arrows)
    {
        double x1 = GetX(sta->GetLongitude());
        double y1 = GetY(sta->GetLatitude());
        double x2 = GetX(link->GetStation()->GetLongitude());
        double y2 = GetY(link->GetStation()->GetLatitude());
        double deltaX = x2 - x1;
        double deltaY = y1 - y2;
        double distance = qSqrt(qPow(x1 - x2, 2) + qPow(y1 - y2, 2));
        setLine(x1 + 6 * deltaY / distance, y1 + 6 * deltaX / distance, x2 + 6 * deltaY / distance, y2 + 6 * deltaX / distance);
    }
}

void MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    setBrush(QColor("white"));
    if (type == circle && sta->GetTransfer())
        type = picture;
    if (type == picture && !sta->GetTransfer())
        type = circle;
    if (type == solidLine && !link->GetPassable())
        type = dotteLine;
    if (type == dotteLine && link->GetPassable())
        type = solidLine;
    if (type == arrows && !link->GetPassable())
        return;
    if (type == circle)
    {
        p.setColor(mainWindow->map->GetLine(sta->GetLine())->GetLineColor());
        double x = GetX(sta->GetLongitude());
        double y = GetY(sta->GetLatitude());
        setRect(x - 11, y - 11, 22, 22);
        p.setWidth(3);
        painter->setPen(p);
        painter->setBrush(this->brush());
        painter->drawEllipse(this->rect());
        if (!sta->GetEntrance())
        {
            QPen whitePen;
            whitePen.setWidth(3);
            whitePen.setColor(QColor(255, 255, 255, 180));
            painter->setPen(whitePen);
            painter->setBrush(QBrush(QColor(255, 255, 255, 180)));
            painter->drawEllipse(this->rect());
        }
        QFont font("Microsoft YaHei", 8, QFont::Bold);
        painter->setFont(font);
        painter->setPen(Qt::gray);
        painter->drawText(this->rect().x() + 11 - 40 + 1, this->rect().y() + 11 + 12 + 1, 80, 15, Qt::AlignCenter, sta->GetName());
        painter->setPen(Qt::black);
        painter->drawText(this->rect().x() + 11 - 40, this->rect().y() + 11 + 12, 80, 15, Qt::AlignCenter, sta->GetName());
    }
    else if (type == picture)
    {
        double x = GetX(sta->GetLongitude());
        double y = GetY(sta->GetLatitude());
        setRect(x - 12.5, y - 12.5, 25, 25);
        QPixmap pixmap;
        pixmap.load(":/icon/icon/transfer.png");
        painter->drawPixmap(this->rect(), pixmap, QRectF());
        if (!sta->GetEntrance())
        {
            QPen whitePen;
            whitePen.setWidth(0);
            whitePen.setColor(QColor(255, 255, 255, 180));
            painter->setPen(whitePen);
            painter->setBrush(QBrush(QColor(255, 255, 255, 180)));
            painter->drawEllipse(this->rect());
        }
        QFont font("Microsoft YaHei", 8, QFont::Bold);
        painter->setFont(font);
        painter->setPen(Qt::gray);
        painter->drawText(this->rect().x() + 12.5 - 40 + 1, this->rect().y() + 12.5 + 12 + 1, 80, 15, Qt::AlignCenter, sta->GetName());
        painter->setPen(Qt::black);
        painter->drawText(this->rect().x() + 12.5 - 40, this->rect().y() + 12.5 + 12, 80, 15, Qt::AlignCenter, sta->GetName());
    }
    else if (type == solidLine)
    {
        p.setColor(mainWindow->map->GetLine(sta->GetLine())->GetLineColor());
        double x1 = GetX(sta->GetLongitude());
        double y1 = GetY(sta->GetLatitude());
        double x2 = GetX(link->GetStation()->GetLongitude());
        double y2 = GetY(link->GetStation()->GetLatitude());
        setLine(x1, y1, x2, y2);
        p.setStyle(Qt::SolidLine);
        p.setWidth(8);
        QGraphicsLineItem::setPen(p);
        QGraphicsLineItem::paint(painter, option, widget);
    }
    else if (type == dotteLine)
    {
        p.setColor(mainWindow->map->GetLine(sta->GetLine())->GetLineColor());
        double x1 = GetX(sta->GetLongitude());
        double y1 = GetY(sta->GetLatitude());
        double x2 = GetX(link->GetStation()->GetLongitude());
        double y2 = GetY(link->GetStation()->GetLatitude());
        setLine(x1, y1, x2, y2);
        p.setStyle(Qt::DashDotLine);
        p.setWidth(8);
        QGraphicsLineItem::setPen(p);
        QGraphicsLineItem::paint(painter, option, widget);
    }
    else if (type == rectangle)
    {
        painter->setPen(p);
        painter->setBrush(QBrush(QColor(255, 255, 255, 200)));
        painter->drawRect(this->rect());
    }
    else if (type == arrows)
    {
        double x1 = GetX(sta->GetLongitude());
        double y1 = GetY(sta->GetLatitude());
        double x2 = GetX(link->GetStation()->GetLongitude());
        double y2 = GetY(link->GetStation()->GetLatitude());
        double deltaX = x2 - x1;
        double deltaY = y1 - y2;
        double distance = qSqrt(qPow(x1 - x2, 2) + qPow(y1 - y2, 2));
        setLine(x1 + 6 * deltaY / distance, y1 + 6 * deltaX / distance, x2 + 6 * deltaY / distance, y2 + 6 * deltaX / distance);
        static const double Pi = 3.14159265358979323846264338327950288419717;
        static double TwoPi = 2.0 * Pi;
        p.setWidth(8);
        QGraphicsLineItem::setPen(p);
        QGraphicsLineItem::paint(painter, option, widget);
        QLineF line = QGraphicsLineItem::line();
        qreal x = (line.x1() + line.x2()) / 2;
        qreal y = (line.y1() + line.y2()) / 2;
        QPointF center = QPointF(x, y);
        QPen pen;
        pen.setColor(Qt::white);
        pen.setWidth(4);
        pen.setCapStyle(Qt::RoundCap);
        double angle = acos(line.dx() / line.length());
        if (line.dy() >= 0)
            angle = TwoPi - angle;
        QPointF destArrowP1 = center + QPointF(sin(angle - Pi / 3) * 8,
                                               cos(angle - Pi / 3) * 8);
        QPointF destArrowP2 = center + QPointF(sin(angle - Pi + Pi / 3) * 8,
                                               cos(angle - Pi + Pi / 3) * 8);
        painter->setPen(pen);
        painter->drawLine(destArrowP1, center);
        painter->drawLine(destArrowP2, center);
    }
    else if (type == from)
    {
        QPen whitePen;
        whitePen.setWidth(0);
        whitePen.setColor(QColor(255, 255, 255, 225));
        painter->setPen(whitePen);
        painter->setBrush(QBrush(QColor(255, 255, 255, 225)));
        painter->drawEllipse(this->rect());
        QPixmap pixmap;
        pixmap.load(":/icon/icon/from.png");
        painter->drawPixmap(this->rect(), pixmap, QRectF());
    }
    else if (type == to)
    {
        QPen whitePen;
        whitePen.setWidth(0);
        whitePen.setColor(QColor(255, 255, 255, 225));
        painter->setPen(whitePen);
        painter->setBrush(QBrush(QColor(255, 255, 255, 225)));
        painter->drawEllipse(this->rect());
        QPixmap pixmap;
        pixmap.load(":/icon/icon/to.png");
        painter->drawPixmap(this->rect(), pixmap, QRectF());
    }
    else if (type == pos)
    {
        QPen whitePen;
        whitePen.setWidth(0);
        whitePen.setColor(QColor(255, 255, 255, 225));
        painter->setPen(whitePen);
        painter->setBrush(QBrush(QColor(255, 255, 255, 225)));
        painter->drawEllipse(this->rect());

        QPixmap pixmap;
        pixmap.load(":/icon/icon/pos.png");
        painter->drawPixmap(this->rect(), pixmap, QRectF());
    }
}

void MapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (mainWindow->isWhite == false && (type == circle || type == picture))
    {
        if (event->buttons() == Qt::LeftButton)
        {
            if (sta != NULL)
            {
                mainWindow->ShowStaButton(sta);
            }
        }
    }
    else
    {
        mainWindow->staButton->hide();
        QGraphicsEllipseItem::mousePressEvent(event);
    }
}

void MapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (type == rectangle)
    {
        mainWindow->Reset();
        mainWindow->isSearch = false;
        mainWindow->isWhite = false;
        mainWindow->search->ui->next->setVisible(0);
        mainWindow->allPaths.Clear();
        mainWindow->CurrResult = -1;
        mainWindow->allFrom = NULL;
        mainWindow->search->ui->charges->clear();
        mainWindow->generalScene->removeItem((QGraphicsEllipseItem *)mainWindow->fromItem);
        mainWindow->stateScene->removeItem((QGraphicsEllipseItem *)mainWindow->fromItem_2);
        mainWindow->generalScene->removeItem((QGraphicsEllipseItem *)mainWindow->toItem);
        mainWindow->stateScene->removeItem((QGraphicsEllipseItem *)mainWindow->toItem_2);
        mainWindow->generalScene->removeItem((QGraphicsEllipseItem *)mainWindow->posItem);
        mainWindow->stateScene->removeItem((QGraphicsEllipseItem *)mainWindow->posItem_2);
        mainWindow->search->ui->stationName->setEnabled(true);
        mainWindow->search->ui->search->setDisabled(false);
        mainWindow->from = NULL;
        mainWindow->to = NULL;
    }
    else
    {
        QGraphicsEllipseItem::mousePressEvent(event);
    }
}

//计算站点x坐标
double MapItem::GetX(double lon)
{
    return ((lon - mainWindow->map->leftLon) / (mainWindow->map->rightLon - mainWindow->map->leftLon)) * mainWindow->map->Width;
}

//计算站点y坐标
double MapItem::GetY(double lat)
{
    return mainWindow->map->Height - (lat - mainWindow->map->bottomLat) / (mainWindow->map->topLat - mainWindow->map->bottomLat) * mainWindow->map->Height;
}
