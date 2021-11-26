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
    explicit AddStation(QWidget* parent = 0);
    //析构函数
    ~AddStation();
    //添加站点
    void AddAStation(const QString& lineName);

private slots:
    //添加站点按钮
    void on_addSta_pressed();

    void on_manageIntersite_pressed();

    void on_deleteStation_pressed();

private:
    Ui::AddStation* ui; //UI
    Admin* admin;       //管理员窗口
    CStation* station = NULL;
    ManageIntersite* manageIntersite;

    //友元声明
    friend class Admin;
    friend class AddIntersite;
    friend class ManageIntersite;
};

#endif // ADDSTATION_H
