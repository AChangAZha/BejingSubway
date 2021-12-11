/*
 * @Author: AChangAZha
 * @Date: 2021-11-26 00:00:08
 * @LastEditTime: 2021-12-07 19:31:40
 * @LastEditors: AChangAZha
 */
#ifndef ADDLINE_H
#define ADDLINE_H

#include <QDialog>

namespace Ui
{
    class AddLine;
}

class Admin; //管理员窗口

class AddLine : public QDialog //添加线路窗口
{
    Q_OBJECT

public:
    //构造函数
    explicit AddLine(QWidget *parent = 0);
    //析构函数
    ~AddLine();

private slots:
    //添加线路按钮
    void on_addLine_pressed();
    //拾取颜色按钮
    void on_setColor_pressed();
    //单独收费按钮
    void on_charges3_toggled(bool checked);
    //删除线路按钮
    void on_deleteLine_pressed();

private:
    //显示单独收费
    void ShowCharges();

    Ui::AddLine *ui;  //UI
    QColor lineColor; //线路颜色
    Admin *admin;     //管理员窗口

    //友元声明
    friend class Admin;
};

#endif // ADDLINE_H
