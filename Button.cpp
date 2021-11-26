#include "Button.h"
#include "ui_Button.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Admin.h"
#include <QtMath>
#include <QPainter>
#include <QBitmap>
#include <QMessageBox>

//构造函数
Button::Button(MainWindow *mainWindow, QWidget *parent) : QWidget(parent),
                                                          ui(new Ui::Button)
{
    this->mainWindow = mainWindow;
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    QPixmap magnifyIco(":/icon/icon/magnify.png"); //从资源文件载入图片
    ui->magnify->setIcon(magnifyIco);
    ui->magnify->setIconSize(QSize(60, 60));
    ui->magnify->setFlat(true);
    ui->magnify->setStyleSheet("border:0px solid;");
    ui->magnify->setCursor(QCursor(Qt::PointingHandCursor));

    QPixmap narrowIco(":/icon/icon/narrow.png");
    ui->narrow->setIcon(narrowIco);
    ui->narrow->setIconSize(QSize(60, 60));
    ui->narrow->setFlat(true);
    ui->narrow->setStyleSheet("border:0px solid;");
    ui->narrow->setCursor(QCursor(Qt::PointingHandCursor));

    QPixmap adminIco(":/icon/icon/admin.png");
    ui->admin->setIcon(adminIco);
    ui->admin->setIconSize(QSize(60, 60));
    ui->admin->setFlat(true);
    ui->admin->setStyleSheet("border:0px solid;");
    ui->admin->setCursor(QCursor(Qt::PointingHandCursor));

    QPixmap aboutIco(":/icon/icon/about.png");
    ui->about->setIcon(aboutIco);
    ui->about->setIconSize(QSize(60, 60));
    ui->about->setFlat(true);
    ui->about->setStyleSheet("border:0px solid;");
    ui->about->setCursor(QCursor(Qt::PointingHandCursor));

    QPixmap generalIco(":/icon/icon/general.png");
    ui->state->setIcon(generalIco);
    ui->state->setIconSize(QSize(60, 60));
    ui->state->setFlat(true);
    ui->state->setStyleSheet("border:0px solid;");
    ui->state->setCursor(QCursor(Qt::PointingHandCursor));
}

//析构函数
Button::~Button()
{
    delete ui;
}

//重写paintEvent为按钮四周加上阴影
void Button::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(5, 5, this->width() - 10, this->height() - 10);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 50);
    for (int i = 0; i < 5; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(5 - i, 5 - i, this->width() - (5 - i) * 2, this->height() - (5 - i) * 2);
        color.setAlpha(50 - qSqrt(i) * 25);
        painter.setPen(color);
        painter.drawPath(path);
    }
}

//管理员窗口按钮
void Button::on_admin_pressed()
{
    mainWindow->AdminShow();
}

//切换普通/拥挤度线路图
void Button::on_state_pressed()
{
    if (mainWindow->ui->subwayMap->scene() == mainWindow->generalScene)
    {
        QPixmap stateIco(":/icon/icon/state.png");
        ui->state->setIcon(stateIco);
        ui->state->setIconSize(QSize(60, 60));
        mainWindow->SwitchToStateMap();
    }
    else
    {
        QPixmap generalIco(":/icon/icon/general.png");
        ui->state->setIcon(generalIco);
        ui->state->setIconSize(QSize(60, 60));
        mainWindow->SwitchToGeneralMap();
    }
}

//放大线路图
void Button::on_magnify_pressed()
{
    if (mainWindow->scale > 2)
        return;
    if (mainWindow->scale * 1.25 > 2.0)
    {
        mainWindow->ui->subwayMap->scale(2.0 / mainWindow->scale, 2.0 / mainWindow->scale);
        mainWindow->scale = mainWindow->scale * (2.0 / mainWindow->scale);
    }
    else
    {
        mainWindow->ui->subwayMap->scale(1.25, 1.25);
        mainWindow->scale = mainWindow->scale * (1.25);
    }
}

//缩小线路图
void Button::on_narrow_pressed()
{
    if (mainWindow->scale < 0.3)
        return;
    if (mainWindow->scale * (2.0 / 3) < 0.3)
    {
        mainWindow->ui->subwayMap->scale(0.3 / mainWindow->scale, 0.3 / mainWindow->scale);
        mainWindow->scale = mainWindow->scale * (0.3 / mainWindow->scale);
    }
    else
    {
        mainWindow->ui->subwayMap->scale(2.0 / 3, 2.0 / 3);
        mainWindow->scale = mainWindow->scale * (2.0 / 3);
    }
}

void Button::on_about_pressed()
{
    QMessageBox box;
    box.setWindowTitle(tr("关于作者"));
    box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
    box.setIcon(QMessageBox::Information);
    box.setText(tr("作者邮箱 : 202041404130@dgut.edu.cn \n"
                   "数据来源 : 北京地铁官网、高德地图（截止2021年11月24日） \n"
                   "本系统查询结果仅供参考，如有错误，请参考北京地铁官网"));
    box.addButton(tr("确定"), QMessageBox::AcceptRole);
    if (box.exec() == QMessageBox::Accepted)
        box.close();
}
