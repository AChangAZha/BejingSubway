/*
 * @Author: AChangAZha
 * @Date: 2021-11-26 00:02:10
 * @LastEditTime: 2021-12-07 19:40:42
 * @LastEditors: AChangAZha
 */
#ifndef BUTTON_H
#define BUTTON_H

#include <QWidget>

namespace Ui
{
    class Button;
}

class MainWindow; //主窗口

class Button : public QWidget //左下角按钮
{
    Q_OBJECT

public:
    //构造函数
    Button(MainWindow *mainWindow, QWidget *parent = 0);
    //析构函数
    ~Button();
    //重写paintEvent为按钮四周加上阴影
    void paintEvent(QPaintEvent *event);

private slots:
    //管理员窗口按钮
    void on_admin_pressed();
    //切换普通/拥挤度线路图
    void on_state_pressed();
    //放大线路图
    void on_magnify_pressed();
    //缩小线路图
    void on_narrow_pressed();
    //关于作者
    void on_about_pressed();

private:
    Ui::Button *ui;         //UI
    MainWindow *mainWindow; //主窗口

    //友元声明
    friend class MainWindow;
};

#endif // BUTTON_H
