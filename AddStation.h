/*
 * @Author: AChangAZha
 * @Date: 2021-11-26 00:00:13
 * @LastEditTime: 2021-12-07 19:32:20
 * @LastEditors: AChangAZha
 */
#ifndef ADDSTATION_H
#define ADDSTATION_H

#include <QDialog>

namespace Ui
{
    class AddStation;
}

class CStation;
class ManageIntersite;
class Admin; //管理员窗口

class AddStation : public QDialog //添加站点窗口
{
    Q_OBJECT

public:
    //构造函数
    explicit AddStation(QWidget *parent = 0);
    //析构函数
    ~AddStation();
    //添加站点
    void AddAStation(const QString &lineName);

private slots:
    //添加站点按钮
    void on_addSta_pressed();
    //管理站点连接按钮
    void on_manageIntersite_pressed();
    //删除站点按钮
    void on_deleteStation_pressed();

private:
    Ui::AddStation *ui;               //UI
    Admin *admin;                     //管理员窗口
    CStation *station = NULL;         //当前管理站点
    ManageIntersite *manageIntersite; //管理站点连接窗口

    //友元声明
    friend class Admin;
    friend class AddIntersite;
    friend class ManageIntersite;
};

#endif // ADDSTATION_H
