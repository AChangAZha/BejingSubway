#ifndef CINTERSITE_H
#define CINTERSITE_H

class MapItem;  //线路图元素声明
class CStation; //站点类声明

class CIntersite //站点连接类
{
public:
    //构造函数
    CIntersite(CStation *const sta, const int &dist,
               const bool &pass, const int &charges = 0) : charges(charges), station(sta), distance(dist),
                                                           link(0), passable(pass) {}
    //获取连接站点
    CStation *GetStation() const;
    //获取下一个连接
    CIntersite *GetLink() const;
    //获取站间距
    int GetDistance() const;
    //获取通行状态
    bool GetPassable() const;
    //获取收费类型
    int GetCharges() const;

    MapItem *generalLink = 0; //普通线路
    MapItem *stateLink = 0;   //拥挤度线路

private:
    int charges;       //收费类型
    CStation *station; //连接站点
    int distance;      //站间距
    CIntersite *link;  //下一个连接站点
    bool passable;     //可通行

    //友元声明
    friend class CSubwayMap;
    friend class CStation;
    friend class Admin;
    friend class AddStation;
    friend class ManageIntersite;
    friend class AddIntersite;
};

#endif // CINTERSITE_H
