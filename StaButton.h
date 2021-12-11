/*
 * @Author: AChangAZha
 * @Date: 2021-11-21 23:02:27
 * @LastEditTime: 2021-12-07 20:06:06
 * @LastEditors: AChangAZha
 */
#ifndef STABUTTON_H
#define STABUTTON_H

#include <QDialog>

namespace Ui
{
    class StaButton;
}

class CStation;   //站点类声明
class MainWindow; //主窗口

class StaButton : public QDialog //站点信息窗口
{
    Q_OBJECT

public:
    //构造函数
    StaButton(MainWindow *mainWindow, QWidget *parent = 0);
    //析构函数
    ~StaButton();
    //重写paintEvent为按钮四周加上阴影
    void paintEvent(QPaintEvent *event);
    //设置站点
    void SetSta(CStation *value);

private slots:
    //选择起点按钮
    void on_from_pressed();
    //选择终点按钮
    void on_to_pressed();

private:
    Ui::StaButton *ui;      //UI
    CStation *sta;          //当前点击站点
    MainWindow *mainWindow; //主窗口
};
#endif // STABUTTON_H
