#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>

namespace Ui
{
    class Admin;
}

class CStation;
class CIntersite;
class CSubwayMap; //线路图类声明
class AddStation; //添加站点窗口
class MainWindow; //主窗口
class ManageIntersite;
class AddLine; //添加线路窗口

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

    void on_manageLine_pressed();

    void on_stationTable_cellDoubleClicked(int row, int column);

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
    void ModifyLine();
    void ModifyStation();
    void AddAIntersite();
    void ModifyIntersite();
    void DeleteIntersite(CStation *sta, CIntersite *link);
    void DeleteStation(CStation *station);
    void DeleteLine();
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
