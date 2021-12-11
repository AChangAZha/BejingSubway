/*
 * @Author: AChangAZha
 * @Date: 2021-11-21 13:25:37
 * @LastEditTime: 2021-12-07 20:02:43
 * @LastEditors: AChangAZha
 */
#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>

namespace Ui
{
    class Search;
}

class MainWindow; //主窗口

class Search : public QWidget //搜索窗口
{
    Q_OBJECT

public:
    //构造函数
    Search(MainWindow *mainWindow, QWidget *parent = 0);
    //析构函数
    ~Search();

private slots:
    //切换线路
    void on_allLine_pressed(const QModelIndex &index);
    //重写paintEvent为按钮四周加上阴影
    void paintEvent(QPaintEvent *event);
    //显示下一条线路按钮
    void on_next_pressed();
    //搜索按钮
    void on_search_pressed();

private:
    Ui::Search *ui;         //UI
    MainWindow *mainWindow; //主窗口

    //友元声明
    friend class MainWindow;
    friend class MapItem;
    friend class Admin;
};

#endif // SEARCH_H
