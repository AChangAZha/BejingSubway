#ifndef CSTATION_H
#define CSTATION_H

#include <QString>

class MapItem;    //线路图元素声明
class CIntersite; //站点连接类声明
class MainWindow;

class CStation //站点类
{
public:
    //构造函数
    CStation(const int &lineNum,
             const QString &stationName,
             const bool &en, const bool &ex,
             const double &lng, const double &lat,
             const bool &trans = false) : index(-1),
                                          line(lineNum), name(stationName),
                                          entrance(en), exit(ex), longitude(lng),
                                          latitude(lat), transfer(trans), link(0) {}
    //获取经度
    double GetLongitude() const;
    //获取纬度
    double GetLatitude() const;
    //获取线路
    int GetLine() const;
    //获取换乘状态
    bool GetTransfer() const;
    //获取站点连接
    CIntersite *GetLink() const;
    //获取站点名称
    QString GetName() const;
    //获取进站状态
    bool GetEntrance() const;
    //获取出站状态
    bool GetExit() const;
    //获取站点编号
    int GetIndex() const;

    MapItem *mapitem; //线路图元素

private:
    //添加连接站点
    bool AddLink(CStation *const station, const int &dist,
                 const bool &pass, MainWindow *mainWindow,
                 const int &charges = 0, const bool &bothway = false);

    int index;        //所在下标
    int line;         //所在线路
    QString name;     //站名
    bool entrance;    //允许进站
    bool exit;        //允许出站
    double longitude; //经度
    double latitude;  //纬度
    bool transfer;    //换乘站
    CIntersite *link; //连接站点

    //友元声明
    friend class CSubwayMap;
    friend class CLine;
    friend class Admin;
    friend class AddStation;
    friend class ManageIntersite;
    friend class AddIntersite;
};

#endif // CSTATION_H
