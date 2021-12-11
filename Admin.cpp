#include "Admin.h"
#include "ui_Admin.h"
#include "AddStation.h"
#include "ui_AddStation.h"
#include "AddLine.h"
#include "ui_AddLine.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "search.h"
#include "ui_Search.h"
#include "ManageIntersite.h"
#include "AddIntersite.h"
#include "ui_AddIntersite.h"
#include "CSubwayMap.h"
#include "CLine.h"
#include "CIntersite.h"
#include "CStation.h"
#include "MapItem.h"
#include <QMessageBox>

//构造函数
Admin::Admin(QWidget *parent) : QDialog(parent),
                                ui(new Ui::Admin)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("地铁线路图维护"));
    setWindowModality(Qt::ApplicationModal);
    Qt::WindowFlags flags = this->windowFlags();
    setWindowFlags(flags & ~Qt::WindowContextHelpButtonHint);

    ui->addLine->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;}"
                               "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                               "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->addStation->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;}"
                                  "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                  "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->manageLine->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;}"
                                  "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                  "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->announcement->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;}"
                                    "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                    "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");

    addLine = new AddLine(this);
    addLine->admin = this;
    addStation = new AddStation(this);
    addStation->admin = this;

    ui->stationTable->setColumnCount(4);
    ui->stationTable->setHorizontalHeaderLabels(QStringList() << "所属线路"
                                                              << "站名"
                                                              << "换乘"
                                                              << "运营状态");
    ui->stationTable->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    ui->stationTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  //禁止修改
    ui->stationTable->verticalHeader()->setHidden(true);
}

//析构函数
Admin::~Admin()
{
    delete ui;
}

//加载线路信息
void Admin::loadLineComboBox()
{
    ui->lineComboBox->addItem(QString("全部线路"));
    for (int i = 0; i < map->numOfLine; i++)
    {
        ui->lineComboBox->addItem(map->GetLine(i)->name);
    }
}

//加载站点信息
void Admin::loadStationTable()
{
    ui->stationTable->clearContents();
    ui->stationTable->setRowCount(map->numOfStation);
    int num = 0;
    for (int i = 0; i < map->numOfLine; i++)
    {
        for (int j = 0; j < map->GetLine(i)->numOfStation; j++)
        {
            AddStationTableItem(num, i, j);
            num++;
        }
    }
}

//添加站点表格元素
void Admin::AddStationTableItem(const int &row, const int &line, const int &station)
{
    CStation *sta = map->GetStation(line, station);
    ui->stationTable->setItem(row, 0, new QTableWidgetItem(ui->lineComboBox->itemText(line + 1)));
    ui->stationTable->item(row, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->stationTable->setItem(row, 1, new QTableWidgetItem(sta->name));
    ui->stationTable->item(row, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    if (sta->transfer)
    {
        ui->stationTable->setItem(row, 2, new QTableWidgetItem("√"));
        ui->stationTable->item(row, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    if (sta->entrance && sta->exit)
        ui->stationTable->setItem(row, 3, new QTableWidgetItem("正常运营"));
    else if (sta->entrance && !sta->exit)
        ui->stationTable->setItem(row, 3, new QTableWidgetItem("不可出站"));
    else if (!sta->entrance && sta->exit)
        ui->stationTable->setItem(row, 3, new QTableWidgetItem("不可进站"));
    else if (!sta->entrance && !sta->exit)
        ui->stationTable->setItem(row, 3, new QTableWidgetItem("暂停运营"));
    ui->stationTable->item(row, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

//切换线路
void Admin::on_lineComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "全部线路")
    {
        for (int i = 0; i < ui->stationTable->rowCount(); i++)
        {
            ui->stationTable->setRowHidden(i, false);
        }
        ui->manageLine->setVisible(0);
        ui->addStation->setVisible(0);
        return;
    }
    ui->manageLine->setVisible(1);
    ui->addStation->setVisible(1);
    for (int i = 0; i < ui->stationTable->rowCount(); i++)
    {
        if (ui->stationTable->item(i, 0)->text() == arg1)
        {
            ui->stationTable->setRowHidden(i, false);
        }
        else
        {
            ui->stationTable->setRowHidden(i, true);
        }
    }
}

//添加线路按钮
void Admin::on_addLine_pressed()
{
    addLine->ui->lineName->clear();
    addLine->lineColor = QColor(QString("#000000"));
    addLine->ui->lineName->setFocus();
    addLine->ui->charges1->setChecked(1);
    addLine->ui->charges->setValue(0);
    addLine->ui->firstTime->setTime(QTime(0, 00));
    addLine->ui->lastTime->setTime(QTime(0, 00));
    addLine->ui->addLine->setText("添加线路");
    addLine->ui->addLine->setIcon(QIcon(":icon/icon/add.png"));
    addLine->ui->deleteLine->setVisible(0);
    addLine->show();
}

//添加线路
void Admin::AddALine()
{
    QMessageBox box;
    box.setWindowTitle(tr("添加线路"));
    box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
    if (addLine->ui->lineName->text().isEmpty())
    {
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("请输入线路名称！"));
        if (box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
    }
    else
    {
        int charges;
        if (addLine->ui->charges1->isChecked())
            charges = 0;
        else if (addLine->ui->charges2->isChecked())
            charges = -1;
        else if (addLine->ui->charges3->isChecked())
            charges = addLine->ui->charges->value();
        if (map->AddLine(addLine->ui->lineName->text(), addLine->lineColor, charges, addLine->ui->firstTime->text(), addLine->ui->lastTime->text()))
        {
            box.setIcon(QMessageBox::Information);
            box.setText(tr("添加成功！"));
            ui->lineComboBox->addItem(addLine->ui->lineName->text());
            mainWindow->search->ui->allLine->addItem(addLine->ui->lineName->text());
            addStation->manageIntersite->addIntersite->ui->line->addItem(addLine->ui->lineName->text());
            addLine->close();
        }
        else
        {
            box.setIcon(QMessageBox::Critical);
            box.setText(tr("添加失败，线路名已存在！"));
        }
        box.addButton(tr("确定"), QMessageBox::AcceptRole);
        if (box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
    }
}

//添加站点按钮
void Admin::on_addStation_pressed()
{
    addStation->AddAStation(ui->lineComboBox->currentText());
    addStation->ui->staName->clear();
    addStation->ui->entrance->setChecked(false);
    addStation->ui->exit->setChecked(false);
    addStation->ui->lng->setValue(115.903985);
    addStation->ui->lat->setValue(39.451556);
    addStation->ui->manageIntersite->setVisible(0);
    addStation->ui->deleteStation->setVisible(0);
    addStation->ui->addSta->setText("添加站点");
    addStation->ui->addSta->setIcon(QIcon(":icon/icon/modify.png"));
    addStation->ui->addSta->setFocus();
    addStation->show();
}

//添加站点
void Admin::AddAStation()
{
    QMessageBox box;
    box.setWindowTitle(tr("添加站点"));
    box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
    if (addStation->ui->staName->text().isEmpty())
    {
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("请输入站点名称！"));
        if (box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
    }
    else
    {
        bool entrance = addStation->ui->entrance->checkState() == Qt::Checked ? false : true;
        bool exit = addStation->ui->exit->checkState() == Qt::Checked ? false : true;
        if (map->AddStation(ui->lineComboBox->currentIndex() - 1, addStation->ui->staName->text(), entrance, exit, addStation->ui->lng->value(), addStation->ui->lat->value()) != NULL)
        {
            box.setIcon(QMessageBox::Information);
            box.setText(tr("添加成功！"));
            int count = 0;
            int i = 0;
            for (i = 0; i < ui->lineComboBox->currentIndex(); i++)
            {
                count += map->line[i]->numOfStation;
            }
            ui->stationTable->insertRow(count - 1);
            AddStationTableItem(count - 1, i - 1, map->line[i - 1]->numOfStation - 1);
            addStation->ui->staName->clear();
            addStation->ui->entrance->setChecked(false);
            addStation->ui->exit->setChecked(false);
            addStation->ui->lng->setValue(115.903985);
            addStation->ui->lat->setValue(39.451556);
            addStation->ui->addSta->setText("添加站点");
            addStation->ui->addSta->setIcon(QIcon(":icon/icon/add.png"));
            addStation->ui->staName->setFocus();
        }
        else
        {
            box.setIcon(QMessageBox::Critical);
            box.setText(tr("添加失败！"));
        }
        box.addButton(tr("确定"), QMessageBox::AcceptRole);
        if (box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
    }
}

//设置线路图
void Admin::SetMap(CSubwayMap *const value)
{
    map = value;
    loadLineComboBox();
    loadStationTable();
    addStation->manageIntersite->addIntersite->loadLine();
}

//设置主窗口
void Admin::SetMainWindow(MainWindow *value)
{
    mainWindow = value;
}

//管理线路按钮
void Admin::on_manageLine_pressed()
{
    CLine *line = map->line[ui->lineComboBox->currentIndex() - 1];
    addLine->ui->lineName->setText(line->name);
    addLine->lineColor = line->lineColor;
    if (line->charges == 0)
    {
        addLine->ui->charges1->setChecked(1);
    }
    else if (line->charges == -1)
    {
        addLine->ui->charges2->setChecked(1);
    }
    else
    {
        addLine->ui->charges3->setChecked(1);
        addLine->ui->charges->setValue(line->charges);
    }
    addLine->ui->firstTime->setTime(QTime::fromString(line->firstTime, "h:mm"));
    addLine->ui->lastTime->setTime(QTime::fromString(line->lastTime, "h:mm"));
    addLine->ui->addLine->setText("确认修改");
    addLine->ui->addLine->setIcon(QIcon(":icon/icon/modify.png"));
    addLine->ui->deleteLine->setVisible(1);
    addLine->show();
}

//管理线路按钮
void Admin::ModifyLine()
{
    QMessageBox box;
    box.setWindowTitle(tr("修改线路信息"));
    box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
    CLine *line = map->line[ui->lineComboBox->currentIndex() - 1];
    QString lineName = addLine->ui->lineName->text();
    if (line->name != lineName)
    {
        if (map->SearchLine(lineName))
        {
            box.setIcon(QMessageBox::Critical);
            box.setText(tr("修改失败，新线路名与其他线路重复！"));
            if (box.exec() == QMessageBox::Accepted)
            {
                box.close();
            }
            addLine->ui->addLine->setFocus();
            return;
        }
        line->name = lineName;
        int count = 0;
        for (int i = 0; i < line->index; i++)
        {
            count += map->line[i]->numOfStation;
        }
        for (int i = 0; i < line->numOfStation; i++)
        {
            ui->stationTable->item(count, 0)->setText(lineName);
            count++;
        }
        ui->lineComboBox->setItemText(line->index + 1, lineName);
        mainWindow->search->ui->allLine->item(line->index + 1)->setText(lineName);
    }
    QColor lineColor = addLine->lineColor;
    if (line->lineColor != lineColor)
    {
        line->lineColor = lineColor;
    }
    QString firstTime = addLine->ui->firstTime->text();
    if (line->firstTime != firstTime)
    {
        line->firstTime = firstTime;
    }
    QString lastTime = addLine->ui->lastTime->text();
    if (line->lastTime != lastTime)
    {
        line->lastTime = lastTime;
    }
    int charges;
    if (addLine->ui->charges1->isChecked())
        charges = 0;
    else if (addLine->ui->charges2->isChecked())
        charges = -1;
    else if (addLine->ui->charges3->isChecked())
        charges = addLine->ui->charges->value();
    if (line->charges != charges)
    {
        line->charges = charges;
        for (int i = 0; i < line->numOfStation; i++)
        {
            CIntersite *link = line->station[i]->link;
            while (link != NULL)
            {
                if (link->distance != 0)
                {
                    if (charges == 0)
                        link->charges = 0;
                    else if (charges == -1)
                        link->charges = 3;
                    else
                        link->charges = -1;
                }
                link = link->link;
            }
        }
    }
    box.setIcon(QMessageBox::Information);
    box.setText(tr("修改成功！"));
    if (box.exec() == QMessageBox::Accepted)
    {
        box.close();
    }
    addLine->close();
}

//管理站点
void Admin::on_stationTable_cellDoubleClicked(int row, int column)
{
    CStation *sta;
    int count = 0;
    for (int i = 0; i < map->numOfLine; i++)
    {
        for (int j = 0; j < map->line[i]->numOfStation; j++)
        {
            if (count == row)
            {
                sta = map->line[i]->station[j];
                count++;
                break;
            }
            count++;
        }
        if (count == row + 1)
            break;
    }
    QString text = "所属线路：";
    text += map->line[sta->line]->name;
    addStation->ui->lineName->setText(text);
    addStation->station = sta;
    addStation->ui->deleteStation->setVisible(1);
    addStation->ui->staName->setText(sta->name);
    addStation->ui->lng->setValue(sta->longitude);
    addStation->ui->lat->setValue(sta->latitude);
    addStation->ui->entrance->setChecked(!sta->entrance);
    addStation->ui->exit->setChecked(!sta->exit);
    addStation->ui->manageIntersite->setVisible(1);
    addStation->ui->addSta->setText("确认修改");
    addStation->ui->addSta->setIcon(QIcon(":icon/icon/modify.png"));
    addStation->ui->addSta->setFocus();
    addStation->show();
}

//修改站点信息
void Admin::ModifyStation()
{
    QMessageBox box;
    box.setWindowTitle(tr("修改站点信息"));
    box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
    CStation *sta = addStation->station;
    QString staName = addStation->ui->staName->text();
    SeqList<CStation *> staList;
    staList.Add(sta);
    if (sta->transfer)
    {
        CIntersite *link = sta->link;
        while (link != NULL)
        {
            if (link->distance == 0)
            {
                staList.Add(link->station);
            }
            link = link->link;
        }
    }
    for (int i = 0; i < staList.GetSize(); i++)
    {
        sta = staList[i];
        if (sta->name != staName)
        {
            if (i == 0 && map->SearchStation(staName))
            {
                box.setIcon(QMessageBox::Critical);
                box.setText(tr("修改失败，新站点名与其他站点重复！"));
                if (box.exec() == QMessageBox::Accepted)
                {
                    box.close();
                }
                addStation->ui->staName->setFocus();
                return;
            }
            sta->name = staName;
        }
        double lng = addStation->ui->lng->value();
        if (sta->longitude != lng)
        {
            sta->longitude = lng;
        }
        double lat = addStation->ui->lat->value();
        if (sta->latitude != lat)
        {
            sta->latitude = lat;
        }
    }
    sta = staList[0];
    bool entrance = !addStation->ui->entrance->isChecked();
    if (sta->entrance != entrance)
    {
        sta->entrance = entrance;
        if (!entrance)
            sta->mapitem->QGraphicsEllipseItem::setZValue(-10);
        else
            sta->mapitem->QGraphicsEllipseItem::setZValue(0);
    }
    bool exit = !addStation->ui->exit->isChecked();
    if (sta->exit != exit)
    {
        sta->exit = exit;
    }
    box.setIcon(QMessageBox::Information);
    box.setText(tr("修改成功！"));
    if (box.exec() == QMessageBox::Accepted)
    {
        box.close();
    }
    ui->lineComboBox->setCurrentIndex(0);
    loadStationTable();
    addStation->close();
}

//添加站点连接
void Admin::AddAIntersite()
{
    QMessageBox box;
    box.setWindowTitle(tr("添加连接"));
    box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
    CStation *from = addStation->station;
    CLine *fromLine = map->line[addStation->manageIntersite->addIntersite->ui->line->currentIndex()];
    CStation *to = fromLine->station[addStation->manageIntersite->addIntersite->ui->station->currentIndex()];
    if (from->name == to->name)
    {
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("不允许连接相同站点！"));
        if (box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
        return;
    }
    int distance = addStation->manageIntersite->addIntersite->ui->distance->value();
    bool passable = addStation->manageIntersite->addIntersite->ui->passable->isChecked();
    int charges;
    if (fromLine->charges == 0)
        charges = 0;
    else if (fromLine->charges == -1)
        charges = addStation->manageIntersite->addIntersite->ui->charges->value();
    else
        charges = -1;
    bool bothway = addStation->manageIntersite->addIntersite->ui->bothway->isChecked();
    bool fromTrans = addStation->manageIntersite->addIntersite->ui->linkLine->currentIndex() == 1;
    if (map->AddLink(from, to, distance, passable, charges, bothway, fromTrans))
    {
        box.setIcon(QMessageBox::Information);
        box.setText(tr("添加成功！"));
        ui->lineComboBox->setCurrentIndex(0);
        loadStationTable();
        addStation->manageIntersite->addIntersite->close();
        addStation->manageIntersite->close();
    }
    else
    {
        box.setIcon(QMessageBox::Critical);
        box.setText(tr("添加失败，该连接已存在！"));
    }
    if (box.exec() == QMessageBox::Accepted)
    {
        box.close();
    }
}

//修改站点连接信息
void Admin::ModifyIntersite()
{
    SeqList<CIntersite *> links;
    CIntersite *link = addStation->manageIntersite->addIntersite->link;
    links.Add(link);
    if (addStation->manageIntersite->addIntersite->ui->bothway->isChecked())
    {
        CIntersite *bothLink = link->station->link;
        while (bothLink != NULL)
        {
            if (bothLink->station == addStation->station)
            {
                links.Add(bothLink);
                break;
            }
            bothLink = bothLink->link;
        }
    }
    for (int i = 0; i < links.GetSize(); i++)
    {
        bool passable = addStation->manageIntersite->addIntersite->ui->passable->isChecked();
        if (links[i]->passable != passable)
        {
            links[i]->passable = passable;
        }
        if (links[i]->distance != 0)
        {
            int distance = addStation->manageIntersite->addIntersite->ui->distance->value();
            if (links[i]->distance != distance)
            {
                links[i]->distance = distance;
            }
        }
        if (links[i]->charges > 0)
        {
            int charges = addStation->manageIntersite->addIntersite->ui->charges->value();
            if (links[i]->charges != charges)
            {
                links[i]->charges = charges;
            }
        }
    }
    QMessageBox box;
    box.setWindowTitle(tr("修改连接"));
    box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
    box.setIcon(QMessageBox::Information);
    box.setText(tr("修改成功！"));
    if (box.exec() == QMessageBox::Accepted)
    {
        box.close();
    }
    addStation->manageIntersite->addIntersite->close();
    addStation->manageIntersite->close();
}

//删除站点连接
void Admin::DeleteIntersite(CStation *sta, CIntersite *link)
{
    if (sta->link == link)
    {
        sta->link = link->link;
    }
    else
    {
        CIntersite *preLink = sta->link;
        while (preLink->link != NULL)
        {
            if (preLink->link == link)
            {
                preLink->link = link->link;
                break;
            }
            preLink = preLink->link;
        }
    }
    if (link->generalLink != NULL)
        mainWindow->generalScene->removeItem((QGraphicsLineItem *)link->generalLink);
    if (link->stateLink != NULL)
        mainWindow->stateScene->removeItem((QGraphicsLineItem *)link->stateLink);
    delete link;
    link = NULL;
}

//删除站点
void Admin::DeleteStation(CStation *station)
{
    SeqList<CStation *> stations;
    SeqList<CLine *> lines;
    stations.Add(station);
    lines.Add(map->line[station->line]);
    if (station->transfer)
    {
        CIntersite *link = station->link;
        while (link != NULL)
        {
            if (link->distance == 0)
            {
                stations.Add(link->station);
                lines.Add(map->line[link->station->line]);
            }
            link = link->link;
        }
    }
    for (int i = 0; i < stations.GetSize(); i++)
    {
        while (stations[i]->link != NULL)
        {
            DeleteIntersite(stations[i], stations[i]->link);
        }
        mainWindow->ui->subwayMap->scene()->removeItem((QGraphicsEllipseItem *)stations[i]->mapitem);
        lines[i]->station.Delete(stations[i]->index);
        lines[i]->numOfStation--;
        map->numOfStation--;
        lines[i]->linkLine.Clear();
        for (int j = 0; j < lines[i]->numOfStation; j++)
        {
            CStation *sta = lines[i]->station[j];
            sta->index = j;
            CIntersite *link = sta->link;
            while (link != NULL)
            {
                if (link->station == stations[i])
                {
                    DeleteIntersite(sta, link);
                    break;
                }
                link = link->link;
            }
            link = sta->link;
            while (link != NULL)
            {
                if (link->distance == 0)
                    lines[i]->AddLinkLine(map->line[link->station->line]);
                link = link->link;
            }
        }
        delete stations[i];
        stations[i] = NULL;
    }
    ui->lineComboBox->setCurrentIndex(0);
    loadStationTable();
}

//删除站点
void Admin::DeleteAStation(CStation *station)
{
    SeqList<CStation *> stations;
    SeqList<CLine *> lines;
    stations.Add(station);
    lines.Add(map->line[station->line]);
    if (station->transfer)
    {
        CIntersite *link = station->link;
        while (link != NULL)
        {
            if (link->distance == 0)
            {
                stations.Add(link->station);
                lines.Add(map->line[link->station->line]);
            }
            link = link->link;
        }
    }
    for (int i = 0; i < stations.GetSize(); i++)
    {
        if (i == 0)
        {
            while (stations[i]->link != NULL)
            {
                DeleteIntersite(stations[i], stations[i]->link);
            }
            mainWindow->ui->subwayMap->scene()->removeItem((QGraphicsEllipseItem *)stations[i]->mapitem);
            lines[i]->station.Delete(stations[i]->index);
            lines[i]->numOfStation--;
            map->numOfStation--;
            lines[i]->linkLine.Clear();
            for (int j = 0; j < lines[i]->numOfStation; j++)
            {
                CStation *sta = lines[i]->station[j];
                sta->index = j;
                CIntersite *link = sta->link;
                while (link != NULL)
                {
                    if (link->station == stations[i])
                    {
                        DeleteIntersite(sta, link);
                        break;
                    }
                    link = link->link;
                }
                link = sta->link;
                while (link != NULL)
                {
                    if (link->distance == 0)
                        lines[i]->AddLinkLine(map->line[link->station->line]);
                    link = link->link;
                }
            }
        }
        else
        {
            CIntersite *link = stations[i]->link;
            stations[i]->transfer = false;
            while (link != NULL)
            {
                if (link->distance == 0)
                {
                    if (link->station == stations[0])
                    {
                        CIntersite *delLink = link;
                        link = link->link;
                        DeleteIntersite(stations[i], delLink);
                        continue;
                    }
                    else
                    {
                        stations[i]->transfer = true;
                    }
                }
                link = link->link;
            }
            lines[i]->linkLine.Clear();
            for (int j = 0; j < lines[i]->numOfStation; j++)
            {
                CStation *sta = lines[i]->station[j];
                CIntersite *link = sta->link;
                while (link != NULL)
                {
                    if (link->distance == 0)
                        lines[i]->AddLinkLine(map->line[link->station->line]);
                    link = link->link;
                }
            }
        }
    }
    delete stations[0];
    stations[0] == NULL;
    ui->lineComboBox->setCurrentIndex(0);
    loadStationTable();
}

//删除线路
void Admin::DeleteLine()
{
    CLine *line = map->line[ui->lineComboBox->currentIndex() - 1];
    int num = line->numOfStation;
    for (int i = 0; i < num; i++)
    {
        DeleteAStation(line->station[0]);
    }
    map->line.Delete(line->index);
    map->numOfLine--;
    for (int i = line->index; i < map->numOfLine; i++)
    {
        map->line[i]->index = i;
        for (int j = 0; j < map->line[i]->numOfStation; j++)
        {
            map->line[i]->station[j]->line = i;
        }
    }
    ui->lineComboBox->setCurrentIndex(0);
    ui->lineComboBox->removeItem(line->index + 1);
    addStation->manageIntersite->addIntersite->ui->line->removeItem(line->index);
    mainWindow->search->ui->allLine->setCurrentRow(0);
    mainWindow->search->ui->allLine->takeItem(line->index + 1);
    delete line;
    line = NULL;
}

//发布公告按钮
void Admin::on_announcement_pressed()
{
    mainWindow->announcement = ui->textEdit->document()->toPlainText();
    mainWindow->search->ui->textBrowser->setText(mainWindow->announcement);
    QMessageBox box;
    box.setWindowTitle(tr("发布公告"));
    box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
    box.setIcon(QMessageBox::Information);
    box.setText(tr("发布成功！"));
    if (box.exec() == QMessageBox::Accepted)
    {
        box.close();
    }
}
