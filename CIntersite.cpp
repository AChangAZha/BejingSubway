#include "CIntersite.h"

//获取连接站点
CStation *CIntersite::GetStation() const
{
    return station;
}

//获取下一个连接
CIntersite *CIntersite::GetLink() const
{
    return link;
}

//获取站间距
int CIntersite::GetDistance() const
{
    return distance;
}

//获取通行状态
bool CIntersite::GetPassable() const
{
    return passable;
}

int CIntersite::GetCharges() const
{
    return charges;
}
