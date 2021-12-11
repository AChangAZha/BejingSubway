#include "ManageIntersite.h"
#include "ui_ManageIntersite.h"
#include "AddIntersite.h"
#include "ui_AddIntersite.h"
#include "AddStation.h"
#include "Admin.h"
#include "CLine.h"
#include "CStation.h"
#include "CIntersite.h"
#include "CSubwayMap.h"

//构造函数
ManageIntersite::ManageIntersite(QWidget *parent) : QDialog(parent),
                                                    ui(new Ui::ManageIntersite)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icon/icon/logo.png"));
    setWindowModality(Qt::ApplicationModal);
    Qt::WindowFlags flags = this->windowFlags();
    setWindowFlags(flags & ~Qt::WindowContextHelpButtonHint);

    ui->intersiteTable->setColumnCount(3);
    ui->intersiteTable->setRowCount(5);
    ui->intersiteTable->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    ui->intersiteTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  //禁止修改
    ui->intersiteTable->setHorizontalHeaderLabels(QStringList() << "连接站点"
                                                                << "站间距"
                                                                << "通行状态");
    ui->intersiteTable->verticalHeader()->setHidden(true);
    ui->intersiteTable->setColumnWidth(0, 150);
    ui->addIntersite->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;}"
                                    "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                    "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");

    addIntersite = new AddIntersite(this);
    addIntersite->manageIntersite = this;
}

//析构函数
ManageIntersite::~ManageIntersite()
{
    delete ui;
}

//添加站点连接按钮
void ManageIntersite::on_addIntersite_pressed()
{
    addIntersite->setWindowTitle(addStation->station->name + "  " + QString::fromUtf8("连接管理"));
    addIntersite->ui->linkSta_2->setVisible(0);
    addIntersite->ui->linkSta_1->setVisible(1);
    addIntersite->ui->line->setVisible(1);
    addIntersite->ui->station->setVisible(1);
    addIntersite->ui->linkLine->setVisible(1);
    addIntersite->ui->linkLineLabel->setVisible(1);
    addIntersite->ui->deleteIntersite->setVisible(0);
    addIntersite->ui->line->setCurrentIndex(0);
    addIntersite->ui->distance->setValue(1);
    addIntersite->ui->passable->setChecked(true);
    addIntersite->ui->bothway->setChecked(true);
    if (addStation->admin->map->line[addStation->station->line]->charges == -1)
    {
        addIntersite->ui->charges->setVisible(1);
        addIntersite->ui->chargesLabel->setVisible(1);
        addIntersite->ui->charges->setValue(3);
    }
    else
    {
        addIntersite->ui->charges->setVisible(0);
        addIntersite->ui->chargesLabel->setVisible(0);
    }
    addIntersite->ui->linkLine->clear();
    addIntersite->ui->linkLine->addItem(addStation->admin->map->line[addStation->station->line]->name);
    if (0 != addStation->station->line)
    {
        addIntersite->ui->linkLine->addItem(addStation->admin->map->line[0]->name);
    }
    addIntersite->ui->addIntersite->setText("添加连接");
    addIntersite->ui->addIntersite->setIcon(QIcon(":icon/icon/add.png"));
    addIntersite->ui->addIntersite->setFocus();
    addIntersite->show();
}

//管理站点连接
void ManageIntersite::on_intersiteTable_cellDoubleClicked(int row, int column)
{
    CStation *sta = addStation->station;
    addIntersite->setWindowTitle(sta->name + "  " + QString::fromUtf8("连接管理"));
    int count = 0;
    CIntersite *link = sta->link;
    while (link != NULL)
    {
        if (count == row)
            break;
        link = link->link;
        count++;
    }
    if (link == NULL)
        return;
    addIntersite->link = link;
    QString toSta = "连接站点：";
    toSta += addStation->admin->map->line[link->station->line]->name;
    toSta += "  ";
    toSta += link->station->name;
    int distance = link->distance;
    int charges = link->charges;
    bool passable = link->passable;
    addIntersite->ui->bothway->setChecked(true);
    addIntersite->ui->deleteIntersite->setVisible(1);
    addIntersite->ui->linkSta_2->setVisible(1);
    addIntersite->ui->linkSta_2->setText(toSta);
    addIntersite->ui->linkSta_1->setVisible(0);
    addIntersite->ui->line->setVisible(0);
    addIntersite->ui->station->setVisible(0);
    if (distance == 0)
    {
        addIntersite->ui->distanceLabel->setVisible(0);
        addIntersite->ui->distance->setVisible(0);
        addIntersite->ui->deleteIntersite->setVisible(0);
    }
    else
    {
        addIntersite->ui->distanceLabel->setVisible(1);
        addIntersite->ui->distance->setVisible(1);
        addIntersite->ui->distance->setValue(distance);
    }
    addIntersite->ui->passable->setChecked(passable);
    addIntersite->ui->linkLine->setVisible(0);
    addIntersite->ui->linkLineLabel->setVisible(0);
    if (charges > 0)
    {
        addIntersite->ui->charges->setVisible(1);
        addIntersite->ui->chargesLabel->setVisible(1);
        addIntersite->ui->charges->setValue(charges);
    }
    else
    {
        addIntersite->ui->charges->setVisible(0);
        addIntersite->ui->chargesLabel->setVisible(0);
    }
    addIntersite->ui->addIntersite->setText("确认修改");
    addIntersite->ui->addIntersite->setIcon(QIcon(":icon/icon/modify.png"));
    addIntersite->ui->addIntersite->setFocus();
    addIntersite->show();
}
