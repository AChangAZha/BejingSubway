/*
 * @Author: AChangAZha
 * @Date: 2021-11-24 16:05:35
 * @LastEditTime: 2021-12-07 19:38:03
 * @LastEditors: AChangAZha
 */
#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>

namespace Ui
{
    class Admin;
}

class CStation;        //站点类声明
class CIntersite;      //站点连接类声明
class CSubwayMap;      //线路图类声明
class AddStation;      //添加站点窗口
class MainWindow;      //主窗口
class ManageIntersite; //管理站点连接窗口
class AddLine;         //添加线路窗口

class Admin : public QDialog //管理员窗口
{
    Q_OBJECT

public:
    //构造函数
    explicit Admin(QWidget *parent = 0);
    //析构函数
    ~Admin();
    //设置线路图
    void SetMap(CSubwayMap *const value);
    //设置主窗口
    void SetMainWindow(MainWindow *value);

    Ui::Admin *ui; //UI

private slots:
    //切换线路
    void on_lineComboBox_currentIndexChanged(const QString &arg1);
    //添加线路按钮
    void on_addLine_pressed();
    //添加站点按钮
    void on_addStation_pressed();
    //管理线路按钮
    void on_manageLine_pressed();
    //管理站点
    void on_stationTable_cellDoubleClicked(int row, int column);
    //发布公告按钮
    void on_announcement_pressed();

private:
    //加载线路信息
    void loadLineComboBox();
    //加载站点信息
    void loadStationTable();
    //添加站点表格元素
    void AddStationTableItem(const int &row, const int &line, const int &station);
    //添加线路
    void AddALine();
    //添加站点
    void AddAStation();
    //管理线路按钮
    void ModifyLine();
    //修改站点信息
    void ModifyStation();
    //添加站点连接
    void AddAIntersite();
    //修改站点连接信息
    void ModifyIntersite();
    //删除站点连接
    void DeleteIntersite(CStation *sta, CIntersite *link);
    //删除站点
    void DeleteStation(CStation *station);
    //删除线路
    void DeleteLine();
    //删除站点
    void DeleteAStation(CStation *station);

    CSubwayMap *map;        //线路图
    AddLine *addLine;       //添加线路窗口
    AddStation *addStation; //添加站点窗口
    MainWindow *mainWindow; //主窗口

    //友元声明
    friend class AddLine;
    friend class AddStation;
    friend class AddIntersite;
    friend class ManageIntersite;
};

#endif // ADMIN_H
