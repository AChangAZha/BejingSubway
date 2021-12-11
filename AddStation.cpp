#include "AddStation.h"
#include "ui_AddStation.h"
#include "Admin.h"
#include "CStation.h"
#include "CIntersite.h"
#include "ManageIntersite.h"
#include "ui_ManageIntersite.h"
#include "CSubwayMap.h"
#include "CLine.h"
#include <QMessageBox>

//构造函数
AddStation::AddStation(QWidget *parent) : QDialog(parent),
                                          ui(new Ui::AddStation)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("站点管理"));
    setWindowIcon(QIcon(":/icon/icon/logo.png"));
    setWindowModality(Qt::ApplicationModal);
    Qt::WindowFlags flags = this->windowFlags();
    setWindowFlags(flags & ~Qt::WindowContextHelpButtonHint);
    ui->addSta->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;}"
                              "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                              "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->manageIntersite->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;}"
                                       "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                       "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->deleteStation->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;}"
                                     "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                     "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->addSta->setFocus();
    ui->addSta->setDefault(true);
    manageIntersite = new ManageIntersite(this);
    manageIntersite->addStation = this;
}

//析构函数
AddStation::~AddStation()
{
    delete ui;
}

//添加站点
void AddStation::AddAStation(const QString &lineName)
{
    ui->lineName->clear();
    QString text = "所属线路：";
    text += lineName;
    ui->lineName->setText(text);
    ui->staName->clear();
    ui->lng->setValue(115.903985);
    ui->lat->setValue(39.451556);
    ui->entrance->setChecked(false);
    ui->exit->setChecked(false);
    ui->staName->setFocus();
}

//添加站点按钮
void AddStation::on_addSta_pressed()
{
    if (ui->addSta->text() == "添加站点")
        admin->AddAStation();
    else
        admin->ModifyStation();
}

//管理站点连接按钮
void AddStation::on_manageIntersite_pressed()
{
    manageIntersite->setWindowTitle(station->name + "  " + QString::fromUtf8("连接管理"));
    manageIntersite->ui->staName->setText(admin->map->line[station->line]->name + "  " + station->name);
    CIntersite *link = station->link;
    int row = 0;
    manageIntersite->ui->intersiteTable->clearContents();
    while (link != NULL)
    {
        if (link->distance != 0)
            manageIntersite->ui->intersiteTable->setItem(row, 0, new QTableWidgetItem(link->station->name));
        else
            manageIntersite->ui->intersiteTable->setItem(row, 0, new QTableWidgetItem(admin->map->line[link->station->line]->name + " " + link->station->name));
        manageIntersite->ui->intersiteTable->item(row, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        manageIntersite->ui->intersiteTable->setItem(row, 1, new QTableWidgetItem(link->distance != 0 ? QString::number(link->distance) : "换乘"));
        manageIntersite->ui->intersiteTable->item(row, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        manageIntersite->ui->intersiteTable->setItem(row, 2, new QTableWidgetItem(link->passable ? "√" : "×"));
        manageIntersite->ui->intersiteTable->item(row, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        link = link->link;
        row++;
    }
    manageIntersite->show();
}

//删除站点按钮
void AddStation::on_deleteStation_pressed()
{
    QMessageBox box;
    box.setWindowTitle("删除站点");
    box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
    box.setIcon(QMessageBox::Warning);
    box.setText("确定要删除吗？");
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Ok);
    if (box.exec() == QMessageBox::Cancel)
    {
        return;
    }
    admin->DeleteStation(station);
    QMessageBox box_2;
    box_2.setWindowTitle(tr("删除站点"));
    box_2.setWindowIcon(QIcon(":/icon/icon/logo.png"));
    box_2.setIcon(QMessageBox::Information);
    box_2.setText(tr("删除成功！"));
    if (box_2.exec() == QMessageBox::Accepted)
    {
        box_2.close();
    }
    this->close();
}
