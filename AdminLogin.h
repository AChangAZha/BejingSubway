#ifndef ADMINLOGIN_H
#define ADMINLOGIN_H

#include <QDialog>

namespace Ui
{
    class AdminLogin;
}

class AdminLogin : public QDialog //管理员登录窗口
{
    Q_OBJECT

public:
    //构造函数
    explicit AdminLogin(QWidget *parent = 0);
    //析构函数
    ~AdminLogin();

private slots:
    //登录按钮
    void on_login_pressed();

private:
    Ui::AdminLogin *ui; //UI
};

#endif // ADMINLOGIN_H
