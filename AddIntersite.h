/*
 * @Author: AChangAZha
 * @Date: 2021-11-26 12:15:16
 * @LastEditTime: 2021-12-07 19:33:19
 * @LastEditors: AChangAZha
 */
#ifndef ADDINTERSITE_H
#define ADDINTERSITE_H

#include <QDialog>

namespace Ui
{
    class AddIntersite;
}

class CIntersite;      //站点连接类声明
class ManageIntersite; //管理站点连接窗口

class AddIntersite : public QDialog //添加站点连接窗口
{
    Q_OBJECT

public:
    //构造函数
    explicit AddIntersite(QWidget *parent = 0);
    //析构函数
    ~AddIntersite();
    //加载线路
    void loadLine();

private slots:
    //加载站点
    void on_line_currentIndexChanged(int index);
    //添加站点连接按钮
    void on_addIntersite_pressed();
    //删除站点连接按钮
    void on_deleteIntersite_pressed();

private:
    Ui::AddIntersite *ui;             //UI
    ManageIntersite *manageIntersite; //管理站点连接窗口
    CIntersite *link;                 //当前管理的站点连接

    //友元声明
    friend class ManageIntersite;
    friend class Admin;
};

#endif // ADDINTERSITE_H
