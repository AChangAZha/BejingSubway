/*
 * @Author: AChangAZha
 * @Date: 2021-11-19 23:28:35
 * @LastEditTime: 2021-12-07 19:49:10
 * @LastEditors: AChangAZha
 */
#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
