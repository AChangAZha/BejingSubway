#ifndef CSUBWAYMAP_H
#define CSUBWAYMAP_H

#include "SeqList.h"
#include "Stack.h"
#include <QString>

class CIntersite; //站点连接声明
class CStation;   //站点类声明
class CLine;      //线路类声明
class QColor;     //颜色
class MainWindow; //主窗口

class Node //站点与连接
{
public:
    //构造函数
    Node() : dist(0), link(0), station(0) {}
    //构造函数
    Node(int dist, CIntersite *link, CStation *station) : dist(dist), link(link), station(station) {}
    //站间距
    int dist;
    //站点连接
    CIntersite *link;
    //站点
    CStation *station;
    //运算符重载
    bool operator<(const Node &node) const { return dist > node.dist; }

    //友元声明
    friend class CIntersite;
};

class CSubwayMap //线路图类
{
public:
    //构造函数
    CSubwayMap() : Width(5650), Height(6670),
                   rightLon(116.904686), leftLon(115.903985),
                   topLat(40.353216), bottomLat(39.451556), numOfStation(0), numOfLine(0) {}
    //获取线路总数
    int GetNumOfLine() const;
    //获取线路
    CLine *GetLine(const int &index) const;
    //获取站点
    CStation *GetStation(const int &lineNum, const int &staNum) const;
    //获取站点连接
    CIntersite *GetLink(const int &lineNum, const int &staNum) const;
    //通过名称搜索线路
    CLine *SearchLine(const QString &lineName) const;
    //通过名称搜索站点
    CStation *SearchStation(const QString &staName) const;
    //查询所有线路
    void FindAllPaths(CStation *const from, CStation *const to) const;
    //DFS搜索所有线路
    void DepthFirstSearch(CStation *const from, CStation *const to, Stack<Node> &stack,
                          SeqList<Node> &path, SeqList<SeqList<int>> &staStatus,
                          SeqList<SeqList<int>> &transStatus, SeqList<SeqList<Node>> &paths) const;
    //查询最小换乘
    void FindMinTransferPath(CStation *from, CStation *to) const;
    //DFS搜索线路连接
    void DepthFirstSearch(CLine *const line_1, CLine *const line_2, Stack<CLine *> &stack,
                          SeqList<SeqList<CLine *>> &links, SeqList<int> &lineStatus,
                          int &minTrans) const;
    //查询最小换乘情况下的最短路径
    int Dijkstra(CStation *const from, CStation *const to, const SeqList<CLine *> &trans,
                 SeqList<SeqList<Node>> &paths, CIntersite **toLink) const;
    //查询最短路径
    void Dijkstra(CStation *const from, CStation *const to, const bool &minPath = false) const;
    //读取文件
    void FromJson();
    //保存文件
    void ToJson();
    //析构函数
    ~CSubwayMap();

    const double Width;     //线路图宽度
    const double Height;    //线路图高度
    const double rightLon;  //线路图最大经度
    const double leftLon;   //线路图最小经度
    const double topLat;    //线路图最大纬度
    const double bottomLat; //线路图最小纬度
    MainWindow *mainWindow; //主窗口

private:
    //添加线路
    bool AddLine(const QString &lineName, const QColor &color, const int &charges,
                 const QString &firstTime, const QString &lastTime);
    //添加站点
    CStation *AddStation(const int &lineNum, const QString &stationName,
                         const bool &en, const bool &ex, const double &lng,
                         const double &lat, CStation *const sta = NULL);
    //添加站点连接
    bool AddLink(CStation *const from, CStation *const to,
                 const int &dist, const bool &pass, const int &charges,
                 const bool &bothway, const bool &fromTrans = false);

    SeqList<CLine *> line; //所有线路
    int numOfStation;      //站点数量
    int numOfLine;         //线路数量

    //友元声明
    friend class Admin;
    friend class AddStation;
    friend class AddIntersite;
    friend class ManageIntersite;
};
#endif // CSUBWAYMAP_H
