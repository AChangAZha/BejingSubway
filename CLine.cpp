/*
 * @Author: AChangAZha
 * @Date: 2021-11-24 15:16:29
 * @LastEditTime: 2021-12-07 19:41:46
 * @LastEditors: AChangAZha
 */
#include "SeqList.h"
#include "CLine.h"
#include "CStation.h"
#include "SeqList.h"

//添加站点
CStation *CLine::AddStation(const QString &stationName,
                            const bool &en, const bool &ex, const double &lng,
                            const double &lat, const bool &trans)
{
    CStation *const newStation = new CStation(index, stationName, en, ex, lng, lat, trans);
    station.Add(newStation);
    newStation->index = station.GetSize() - 1;
    numOfStation++;
    return newStation;
}

//获取首车时间
QString CLine::GetLastTime() const
{
    return lastTime;
}

//获取末车时间
QString CLine::GetFirstTime() const
{
    return firstTime;
}

//获取线路颜色
QColor CLine::GetLineColor() const
{
    return lineColor;
}

//获取线路编号
int CLine::GetNumOfStation() const
{
    return numOfStation;
}

//获取线路名称
QString CLine::GetName() const
{
    return name;
}

//添加连接线路
void CLine::AddLinkLine(CLine *line)
{
    for (int i = 0; i < linkLine.GetSize(); i++)
    {
        if (linkLine[i] == line)
            return;
    }
    linkLine.Add(line);
}

//获取收费标准
int CLine::GetCharges() const
{
    return charges;
}
