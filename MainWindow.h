#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "SeqList.h"

namespace Ui
{
    class MainWindow;
}

class CSubwayMap;         //线路图类声明
class CStation;           //站点类声明
class Node;               //站点与连接类声明
class CIntersite;         //站点连接类声明
class QGraphicsScene;     //图形场景类声明
class Graphics_view_zoom; //可缩放图形视图类声明
class MapItem;            //线路图元素声明
class StaButton;          //站点点击窗口
class AdminLogin;         //管理员登录窗口
class Admin;              //管理员窗口
class Button;             //左下角按钮
class Search;             //搜索窗口

class MainWindow : public QWidget //主窗口
{
    Q_OBJECT

public:
    //构造函数
    explicit MainWindow(QWidget *parent = 0);
    //析构函数
    ~MainWindow();
    //显示线路图
    void ShowSubwayMap();
    //绘制站点
    MapItem *PaintStation(CStation *sta, QGraphicsScene *scene);
    //绘制普通线路
    MapItem *PaintGeneralLink(CStation *sta, CIntersite *link);
    //绘制拥挤度线路
    MapItem *PaintStateLink(CStation *sta, CIntersite *link);
    //切换到普通线路图
    void SwitchToGeneralMap();
    //切换到拥挤度线路图
    void SwitchToStateMap();
    //重置线路图
    void Reset();
    //拥挤度更新
    void StateChange();
    //切换线路
    void SwitchLine(int line);
    //搜索路径
    void SearchPath();
    void SelectFrom(CStation *from);
    void SelectTo(CStation *to);
    void FindShortestPath(CStation *to, SeqList<SeqList<Node>> &path);
    void PaintPath(CStation *toSta, CIntersite *toLink);
    void ShowStaButton(CStation *sta);
    void ShowNextPaths();
    int CountCharges(int sum);
    void CountCharges(CIntersite *aLink);
    void SearchStation(CStation *station);
    void AdminShow();

    double scale = 1.0; //线路图放大倍数

private:
    Ui::MainWindow *ui;              //UI
    CSubwayMap *map;                 //地铁线路图
    CStation *from = NULL;           //起点
    CStation *to = NULL;             //终点
    SeqList<Node> path;              //路径
    SeqList<SeqList<Node>> allPaths; //所有路径
    bool isWhite = false;            //白色背景是否显示
    bool isSearch = false;           //搜索路径状态
    CStation *allFrom = NULL;        //全部路径起点
    bool adminLogined = false;       //管理员登录状态
    int CurrResult = -1;             //全部路径当前搜索结果
    Button *button;                  //左下角按钮
    Search *search;                  //搜索窗口
    Admin *admin;                    //管理员窗口
    StaButton *staButton;            //站点点击窗口
    AdminLogin *adminLogin;          //管理员登录窗口
    Graphics_view_zoom *view;        //图形视图
    QGraphicsScene *generalScene;    //普通线路图
    QGraphicsScene *stateScene;      //拥挤度线路图
    MapItem *white;                  //白色背景
    MapItem *fromItem;               //普通线路图起点定位
    MapItem *toItem;                 //普通线路图终点定位
    MapItem *fromItem_2;             //拥挤度线路图起点定位
    MapItem *toItem_2;               //拥挤度线路图终点定位
    MapItem *posItem;                //普通线路图搜索定位
    MapItem *posItem_2;              //拥挤度线路图搜索定位
    QString announcement = "欢迎乘坐北京地铁！\n运营提示：2021年10月8日至12月4日，2号线前门站封闭改造；\n2021年9月19日至2022年1月11日，10号线西土城站封闭改造";

    //友元声明
    friend class Search;
    friend class Button;
    friend class StaButton;
    friend class Admin;
    friend class AdminLogin;
    friend class MapItem;
    friend class CSubwayMap;
};

#endif // MAINWINDOW_H
