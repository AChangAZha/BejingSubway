#include "StaButton.h"
#include "ui_StaButton.h"
#include "CStation.h"
#include "CLine.h"
#include "CIntersite.h"
#include "CSubwayMap.h"
#include "MainWindow.h"
#include <QPainter>
#include <QtMath>

//构造函数
StaButton::StaButton(MainWindow *mainWindow, QWidget *parent) : QDialog(parent),
                                                                ui(new Ui::StaButton)
{
    ui->setupUi(this);
    this->mainWindow = mainWindow;
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

    QPixmap fromIco(":/icon/icon/from.png"); //从资源文件载入图片
    ui->from->setIcon(fromIco);
    ui->from->setIconSize(QSize(35, 35));
    ui->from->setFlat(true);
    ui->from->setStyleSheet("border:0px solid;");
    ui->from->setCursor(QCursor(Qt::PointingHandCursor));

    QPixmap toIco(":/icon/icon/to.png"); //从资源文件载入图片
    ui->to->setIcon(toIco);
    ui->to->setIconSize(QSize(35, 35));
    ui->to->setFlat(true);
    ui->to->setStyleSheet("border:0px solid;");
    ui->to->setCursor(QCursor(Qt::PointingHandCursor));

    ui->staName->setAlignment(Qt::AlignCenter);
    ui->lineName->setAlignment(Qt::AlignCenter);
}

//析构函数
StaButton::~StaButton()
{
    delete ui;
}

//选择起点按钮
void StaButton::on_from_pressed()
{
    mainWindow->SelectFrom(sta);
    this->close();
}

//选择终点按钮
void StaButton::on_to_pressed()
{
    mainWindow->SelectTo(sta);
    this->hide();
}

//设置站点
void StaButton::SetSta(CStation *value)
{
    sta = value;
    ui->staName->clear();
    ui->lineName->clear();
    ui->staName->setText(sta->GetName());
    QString line;
    if (sta->GetTransfer())
    {
        CIntersite *link = sta->GetLink();
        while (link != NULL)
        {
            if (link->GetDistance() == 0)
            {
                line += mainWindow->map->GetLine(link->GetStation()->GetLine())->GetName();
                line += "   ";
            }
            link = link->GetLink();
        }
    }
    line += mainWindow->map->GetLine(sta->GetLine())->GetName();
    ui->lineName->setText(line);
}

//重写paintEvent为按钮四周加上阴影
void StaButton::paintEvent(QPaintEvent *event)
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
