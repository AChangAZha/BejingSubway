#include "AdminLogin.h"
#include "ui_AdminLogin.h"
#include <QMessageBox>

//构造函数
AdminLogin::AdminLogin(QWidget *parent) : QDialog(parent),
                                          ui(new Ui::AdminLogin)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("管理员登陆"));
    setWindowIcon(QIcon(":/icon/icon/logo.png"));
    setWindowModality(Qt::ApplicationModal);
    Qt::WindowFlags flags = this->windowFlags();
    setWindowFlags(flags & ~Qt::WindowContextHelpButtonHint);

    ui->login->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;}"
                             "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                             "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
}

//析构函数
AdminLogin::~AdminLogin()
{
    delete ui;
}

//登录按钮
void AdminLogin::on_login_pressed()
{
    if (ui->userName->text() == "Admin" && ui->password->text() == "beijingsubway")
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this, "错误", "用户名或密码错误！", QMessageBox::Ok);
        ui->userName->clear();
        ui->password->clear();
        ui->userName->setFocus();
    }
}
