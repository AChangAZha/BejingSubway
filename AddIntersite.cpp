#include "AddIntersite.h"
#include "ui_AddIntersite.h"
#include "ManageIntersite.h"
#include "CSubwayMap.h"
#include "AddStation.h"
#include "Admin.h"
#include "CLine.h"
#include "CStation.h"
#include "CIntersite.h"
#include <QMessageBox>

//构造函数
AddIntersite::AddIntersite(QWidget *parent) : QDialog(parent),
                                              ui(new Ui::AddIntersite)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icon/icon/logo.png"));
    setWindowModality(Qt::ApplicationModal);
    Qt::WindowFlags flags = this->windowFlags();
    setWindowFlags(flags & ~Qt::WindowContextHelpButtonHint);
    ui->addIntersite->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;}"
                                    "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                    "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->deleteIntersite->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;}"
                                       "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                       "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->addIntersite->setFocus();
    ui->addIntersite->setDefault(true);
}

//析构函数
AddIntersite::~AddIntersite()
{
    delete ui;
}

//加载线路
void AddIntersite::loadLine()
{
    CSubwayMap *map = manageIntersite->addStation->admin->map;
    for (int i = 0; i < map->numOfLine; i++)
    {
        ui->line->addItem(map->line[i]->name);
    }
}

//加载站点
void AddIntersite::on_line_currentIndexChanged(int index)
{
    CSubwayMap *map = manageIntersite->addStation->admin->map;
    ui->linkLine->clear();
    if (manageIntersite->addStation->station != NULL)
    {
        ui->linkLine->addItem(map->line[manageIntersite->addStation->station->line]->name);
    }
    ui->station->clear();
    for (int i = 0; i < map->line[index]->numOfStation; i++)
    {
        ui->station->addItem(map->line[index]->station[i]->name);
    }
    if (manageIntersite->addStation->station != NULL)
        if (index != manageIntersite->addStation->station->line)
        {
            ui->linkLine->addItem(map->line[index]->name);
        }
}

//添加按钮
void AddIntersite::on_addIntersite_pressed()
{
    if (ui->addIntersite->text() == "添加连接")
    {
        manageIntersite->addStation->admin->AddAIntersite();
    }
    else
    {
        manageIntersite->addStation->admin->ModifyIntersite();
    }
}

//删除按钮
void AddIntersite::on_deleteIntersite_pressed()
{
    QMessageBox box;
    box.setWindowTitle("删除连接");
    box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
    box.setIcon(QMessageBox::Warning);
    box.setText("确定要删除吗？");
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Ok);
    if (box.exec() == QMessageBox::Cancel)
    {
        return;
    }
    if (ui->bothway->isChecked())
    {
        CIntersite *bothLink = link->station->link;
        while (bothLink != NULL)
        {
            if (bothLink->station == manageIntersite->addStation->station)
            {
                manageIntersite->addStation->admin->DeleteIntersite(link->station, bothLink);
                break;
            }
            bothLink = bothLink->link;
        }
    }
    manageIntersite->addStation->admin->DeleteIntersite(manageIntersite->addStation->station, link);
    QMessageBox box_2;
    box_2.setWindowTitle(tr("删除连接"));
    box_2.setWindowIcon(QIcon(":/icon/icon/logo.png"));
    box_2.setIcon(QMessageBox::Information);
    box_2.setText(tr("删除成功！"));
    if (box_2.exec() == QMessageBox::Accepted)
    {
        box_2.close();
    }
    this->close();
    manageIntersite->close();
}
