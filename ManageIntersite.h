/*
 * @Author: AChangAZha
 * @Date: 2021-11-24 09:01:15
 * @LastEditTime: 2021-12-07 19:51:45
 * @LastEditors: AChangAZha
 */
#ifndef MANAGEINTERSITE_H
#define MANAGEINTERSITE_H

#include <QDialog>

namespace Ui
{
    class ManageIntersite;
}

class AddIntersite; //添加站点连接窗口
class AddStation;   //添加站点窗口

class ManageIntersite : public QDialog //管理站点连接窗口
{
    Q_OBJECT

public:
    //构造函数
    ManageIntersite(QWidget *parent = 0);
    //析构函数
    ~ManageIntersite();

private slots:
    //添加站点连接按钮
    void on_addIntersite_pressed();
    //管理站点连接
    void on_intersiteTable_cellDoubleClicked(int row, int column);

private:
    Ui::ManageIntersite *ui;    //UI
    AddIntersite *addIntersite; //添加站点连接窗口
    AddStation *addStation;     //添加站点窗口

    //友元声明
    friend class AddStation;
    friend class AddIntersite;
    friend class Admin;
};

#endif // MANAGEINTERSITE_H
