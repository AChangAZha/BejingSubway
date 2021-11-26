#include "CStation.h"
#include "CIntersite.h"
#include <cstddef>
#include "MainWindow.h"

//添加站点连接
bool CStation::AddLink(CStation *const linkSta, const int &dist, const bool &pass, MainWindow *mainWindow, const int &charges, const bool &bothway)
{
    bool success = false;
    CIntersite *ptr = this->link;
    bool find = false;
    while (ptr != NULL)
    {
        if (ptr->station == linkSta)
        {
            find = true;
        }
        ptr = ptr->link;
    }
    if (find)
    {
        if (bothway)
            success = linkSta->AddLink(this, dist, pass, mainWindow, charges);
        return success;
    }
    CIntersite *const newLink = new CIntersite(linkSta, dist, pass, charges);
    if (mainWindow != NULL)
    {
        if (newLink->distance != 0)
        {
            MapItem *item = mainWindow->PaintGeneralLink(this, newLink);
            newLink->generalLink = item;
            item = mainWindow->PaintStateLink(this, newLink);
            newLink->stateLink = item;
        }
    }
    newLink->link = this->link;
    this->link = newLink;
    success = true;
    if (bothway)
        linkSta->AddLink(this, dist, pass, mainWindow, charges);
    return success;
}

//获取经度
double CStation::GetLongitude() const
{
    return longitude;
}

//获取纬度
double CStation::GetLatitude() const
{
    return latitude;
}

//获取线路
int CStation::GetLine() const
{
    return line;
}

//获取换乘状态
bool CStation::GetTransfer() const
{
    return transfer;
}

//获取站点连接
CIntersite *CStation::GetLink() const
{
    return link;
}

//获取站点名称
QString CStation::GetName() const
{
    return name;
}

//获取进站状态
bool CStation::GetEntrance() const
{
    return entrance;
}

//获取出站状态
bool CStation::GetExit() const
{
    return exit;
}

//获取站点编号
int CStation::GetIndex() const
{
    return index;
}
