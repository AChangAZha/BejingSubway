#ifndef CLINE_H
#define CLINE_H

#include <QString>
#include <QColor>
#include "SeqList.h"

class CStation; //站点类声明

class CLine //线路类
{
public:
    //构造函数
    CLine(const QString& lineName, const QColor& color, const int& charges,
          const QString& firstTime, const QString& lastTime) : name(lineName), numOfStation(0),
        lineColor(color), index(-1), charges(charges), firstTime(firstTime), lastTime(lastTime) {}
    //获取线路颜色
    QColor GetLineColor() const;
    //获取线路编号
    int GetNumOfStation() const;
    //获取线路名称
    QString GetName() const;
    //获取收费标准
    int GetCharges() const;

    QString getFirstTime() const;

    QString getLastTime() const;

private:
    //添加连接线路
    void AddLinkLine(CLine* line);
    //添加站点
    CStation* AddStation(const QString& stationName,
                         const bool& en, const bool& ex, const double& lng,
                         const double& lat, const bool& trans);

    QString name;                //线路名称
    SeqList<CStation*> station;  //所有站点
    int numOfStation;            //线路站点总数
    QColor lineColor;            //线路颜色
    int index;                   //线路编号
    SeqList<CLine*> linkLine;    //连接线路
    int charges;                 //收费标准
    QString firstTime;           //首车时间
    QString lastTime;            //末车时间

    //友元声明
    friend class CSubwayMap;
    friend class Admin;
    friend class AddStation;
    friend class AddIntersite;
    friend class ManageIntersite;
};

#endif // CLINE_H
