#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CSubwayMap.h"
#include "CStation.h"
#include "CLine.h"
#include "CIntersite.h"
#include "Button.h"
#include "ui_Button.h"
#include "Search.h"
#include "ui_Search.h"
#include "AdminLogin.h"
#include "Admin.h"
#include "ui_Admin.h"
#include "StaButton.h"
#include "SeqList.h"
#include "MapItem.h"
#include "graphics_view_zoom.h"
#include <QGraphicsDropShadowEffect>
#include <QtMath>
#include <QComboBox>
#include <QMessageBox>

//构造函数
MainWindow::MainWindow(QWidget *parent) : QWidget(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("北京地铁"));
    setWindowIcon(QIcon(":/icon/icon/logo.png"));

    map = new CSubwayMap;
    map->FromJson();
    map->mainWindow = this;

    view = new Graphics_view_zoom(ui->subwayMap);
    view->set_modifiers(Qt::NoModifier);
    view->mainWindow = this;
    ui->subwayMap->setFrameStyle(QFrame::NoFrame);
    ui->subwayMap->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->subwayMap->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->subwayMap->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->subwayMap->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->subwayMap->setStyleSheet("padding: 0px;");

    generalScene = new QGraphicsScene;
    generalScene->setSceneRect(0, 0, map->Width, map->Height);
    generalScene->setBackgroundBrush(QBrush(Qt::white));
    stateScene = new QGraphicsScene;
    stateScene->setSceneRect(0, 0, map->Width, map->Height);
    stateScene->setBackgroundBrush(QBrush(Qt::white));
    ShowSubwayMap();

    button = new Button(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(ui->subwayMap);
    buttonLayout->setSpacing(10);
    buttonLayout->setMargin(10);
    buttonLayout->addWidget(button);
    buttonLayout->setAlignment(button, Qt::AlignBottom | Qt::AlignRight);

    search = new Search(this, this);
    QHBoxLayout *rightLayout = new QHBoxLayout(ui->subwayMap);
    rightLayout->setSpacing(0);
    rightLayout->setMargin(0);
    rightLayout->addWidget(search);
    rightLayout->setAlignment(search, Qt::AlignTop | Qt::AlignLeft);
    search->ui->next->setVisible(0);

    admin = new Admin(this);
    admin->SetMap(map);
    admin->SetMainWindow(this);

    adminLogin = new AdminLogin(this);

    white = new MapItem(this, MapItem::rectangle);
    white->setRect(0, 0, map->Width, map->Height);
    white->QGraphicsEllipseItem::setZValue(-50);
    generalScene->addItem((QGraphicsEllipseItem *)white);

    fromItem = new MapItem(this, MapItem::from);
    toItem = new MapItem(this, MapItem::to);
    posItem = new MapItem(this, MapItem::pos);

    fromItem_2 = new MapItem(this, MapItem::from);
    toItem_2 = new MapItem(this, MapItem::to);
    posItem_2 = new MapItem(this, MapItem::pos);

    staButton = new StaButton(this, this);

    ui->subwayMap->setScene(generalScene);
    ui->subwayMap->show();

    search->ui->textBrowser->setText(announcement);
}

//析构函数
MainWindow::~MainWindow()
{
    map->ToJson();
    delete ui;
    delete generalScene;
    delete stateScene;
    delete map;
    delete view;
}

//显示线路图
void MainWindow::ShowSubwayMap()
{
    for (int i = 0; i < map->GetNumOfLine(); i++)
    {
        int numOfStation = map->GetLine(i)->GetNumOfStation();
        for (int j = 0; j < numOfStation; j++)
        {
            CStation *sta = map->GetStation(i, j);
            MapItem *item;
            item = PaintStation(sta, generalScene);
            sta->mapitem = item;
            CIntersite *link = sta->GetLink();
            while (link != NULL)
            {
                if (link->GetDistance() != 0)
                {
                    item = PaintGeneralLink(sta, link);
                    link->generalLink = item;
                    item = PaintStateLink(sta, link);
                    link->stateLink = item;
                }
                link = link->GetLink();
            }
        }
    }
}

//绘制站点
MapItem *MainWindow::PaintStation(CStation *sta, QGraphicsScene *scene)
{
    MapItem *staItem;
    if (!sta->GetTransfer())
    {
        staItem = new MapItem(this, MapItem::circle, sta);
    }
    else
    {
        staItem = new MapItem(this, MapItem::picture, sta);
    }
    if (!sta->GetEntrance())
        staItem->QGraphicsEllipseItem::setZValue(-10);
    else
        staItem->QGraphicsEllipseItem::setZValue(0);
    scene->addItem((QGraphicsEllipseItem *)staItem);
    return staItem;
}

//绘制普通线路
MapItem *MainWindow::PaintGeneralLink(CStation *sta, CIntersite *link)
{
    MapItem *lineItem;
    if (link->GetPassable())
        lineItem = new MapItem(this, MapItem::solidLine, sta, link);
    else
        lineItem = new MapItem(this, MapItem::dotteLine, sta, link);
    lineItem->QGraphicsLineItem::setZValue(-20);
    generalScene->addItem((QGraphicsLineItem *)lineItem);
    return lineItem;
}

//绘制拥挤度线路
MapItem *MainWindow::PaintStateLink(CStation *sta, CIntersite *link)
{
    MapItem *lineItem;
    lineItem = new MapItem(this, MapItem::arrows, sta, link);
    lineItem->p.setColor(QColor(QString("#82c29f")));
    lineItem->QGraphicsLineItem::setZValue(-20);
    stateScene->addItem((QGraphicsLineItem *)lineItem);
    return lineItem;
}

//切换到普通线路图
void MainWindow::SwitchToGeneralMap()
{
    for (int i = 0; i < map->GetNumOfLine(); i++)
    {
        int numOfStation = map->GetLine(i)->GetNumOfStation();
        for (int j = 0; j < numOfStation; j++)
        {
            generalScene->addItem((QGraphicsEllipseItem *)map->GetStation(i, j)->mapitem);
        }
    }

    generalScene->addItem((QGraphicsEllipseItem *)white);
    ui->subwayMap->setScene(generalScene);
}

//切换到拥挤度线路图
void MainWindow::SwitchToStateMap()
{
    StateChange();
    for (int i = 0; i < map->GetNumOfLine(); i++)
    {
        int numOfStation = map->GetLine(i)->GetNumOfStation();
        for (int j = 0; j < numOfStation; j++)
        {
            stateScene->addItem((QGraphicsEllipseItem *)map->GetStation(i, j)->mapitem);
        }
    }
    stateScene->addItem((QGraphicsEllipseItem *)white);
    ui->subwayMap->setScene(stateScene);
}

//重置线路图
void MainWindow::Reset()
{
    search->ui->textBrowser->setText(announcement);
    white->QGraphicsEllipseItem::setZValue(-50);
    for (int i = 0; i < map->GetNumOfLine(); i++)
    {
        for (int j = 0; j < map->GetLine(i)->GetNumOfStation(); j++)
        {
            CStation *station = map->GetStation(i, j);
            if (!station->GetEntrance())
                station->mapitem->QGraphicsEllipseItem::setZValue(-10);
            else
                station->mapitem->QGraphicsEllipseItem::setZValue(0);
            CIntersite *link = station->GetLink();
            while (link != NULL)
            {
                if (link->generalLink != NULL)
                    link->generalLink->QGraphicsLineItem::setZValue(-20);
                if (link->stateLink != NULL)
                    link->stateLink->QGraphicsLineItem::setZValue(-20);
                link = link->GetLink();
            }
        }
    }
}

//拥挤度更新
void MainWindow::StateChange()
{
    unsigned seed;
    seed = time(0);
    srand(seed);
    for (int i = 0; i < map->GetNumOfLine(); i++)
    {
        for (int j = 0; j < map->GetLine(i)->GetNumOfStation(); j++)
        {
            CStation *station = map->GetStation(i, j);
            CIntersite *link = station->GetLink();
            while (link != NULL)
            {
                if (link->stateLink != NULL)
                {
                    int k = rand() % 100;
                    if (k < 80)
                        link->stateLink->p.setColor(QString("#82c29f"));
                    else if (k < 90)
                        link->stateLink->p.setColor(QString("#f1d64b"));
                    else if (k < 95)
                        link->stateLink->p.setColor(QString("#dd8d12"));
                    else if (k < 100)
                        link->stateLink->p.setColor(QString("#d10005"));
                }
                link = link->GetLink();
            }
        }
    }
}

//切换线路
void MainWindow::SwitchLine(int line)
{
    if (isSearch)
        return;
    if (line == -1)
    {
        Reset();
        return;
    }
    Reset();
    QString firstTime = "首车时间：" + map->GetLine(line)->getFirstTime();
    QString lastTime = "末车时间：" + map->GetLine(line)->getLastTime();
    search->ui->textBrowser->setText(firstTime + "\n" + lastTime);
    qreal maxX = map->GetStation(line, 0)->mapitem->rect().x();
    qreal minX = map->GetStation(line, 0)->mapitem->rect().x();
    qreal maxY = map->GetStation(line, 0)->mapitem->rect().y();
    qreal minY = map->GetStation(line, 0)->mapitem->rect().y();
    generalScene->removeItem((QGraphicsEllipseItem *)fromItem);
    stateScene->removeItem((QGraphicsEllipseItem *)fromItem_2);
    generalScene->removeItem((QGraphicsEllipseItem *)toItem);
    stateScene->removeItem((QGraphicsEllipseItem *)toItem_2);
    generalScene->removeItem((QGraphicsEllipseItem *)posItem);
    stateScene->removeItem((QGraphicsEllipseItem *)posItem_2);
    white->QGraphicsEllipseItem::setZValue(80);
    for (int i = 0; i < map->GetLine(line)->GetNumOfStation(); i++)
    {
        CStation *sta = map->GetStation(line, i);
        MapItem *staItem = sta->mapitem;
        staItem->QGraphicsEllipseItem::setZValue(100);
        qreal x = staItem->rect().x();
        qreal y = staItem->rect().y();
        if (x > maxX)
            maxX = x;
        if (x < minX)
            minX = x;
        if (y > maxY)
            maxY = y;
        if (y < minY)
            minY = y;
        CIntersite *link = sta->GetLink();
        while (link != NULL)
        {
            if (link->generalLink != NULL)
                link->generalLink->QGraphicsLineItem::setZValue(90);
            if (link->stateLink != NULL)
                link->stateLink->QGraphicsLineItem::setZValue(90);
            link = link->GetLink();
        }
    }
    ui->subwayMap->scale(0.7 / scale, 0.7 / scale);
    scale = 0.7;
    ui->subwayMap->centerOn((maxX + minX) / 2.0, (maxY + minY) / 2.0);
    isWhite = true;
}

void MainWindow::SelectFrom(CStation *from)
{
    if (to == from)
    {
        QMessageBox box;
        box.setWindowTitle(tr("线路查询"));
        box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
        box.setIcon(QMessageBox::Critical);
        box.setText(tr("起点不能与终点相同！"));
        box.addButton(tr("确定"), QMessageBox::AcceptRole);
        if (box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
        return;
    }
    if (!from->GetEntrance())
    {
        QMessageBox box;
        box.setWindowTitle(tr("线路查询"));
        box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
        box.setIcon(QMessageBox::Critical);
        QString text(from->GetName());
        text += " 目前不可进站";
        box.setText(text);
        box.addButton(tr("确定"), QMessageBox::AcceptRole);
        if (box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
        return;
    }
    generalScene->removeItem((QGraphicsEllipseItem *)posItem);
    stateScene->removeItem((QGraphicsEllipseItem *)posItem_2);

    generalScene->removeItem((QGraphicsEllipseItem *)fromItem);
    stateScene->removeItem((QGraphicsEllipseItem *)fromItem_2);
    this->from = from;
    qreal x;
    qreal y;
    if (from->GetTransfer())
    {
        x = from->mapitem->rect().x() + 12.5 - 25;
        y = from->mapitem->rect().y() - 35;
    }
    else
    {
        x = from->mapitem->rect().x() + 11 - 25;
        y = from->mapitem->rect().y() - 35;
    }
    fromItem->setRect(x, y, 50, 50);
    fromItem->QGraphicsEllipseItem::setZValue(120);
    fromItem_2->setRect(x, y, 50, 50);
    fromItem_2->QGraphicsEllipseItem::setZValue(120);
    generalScene->addItem((QGraphicsEllipseItem *)fromItem);
    stateScene->addItem((QGraphicsEllipseItem *)fromItem_2);
    SearchPath();
}

void MainWindow::SelectTo(CStation *to)
{
    if (to == from)
    {
        QMessageBox box;
        box.setWindowTitle(tr("线路查询"));
        box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
        box.setIcon(QMessageBox::Critical);
        box.setText(tr("起点不能与终点相同！"));
        box.addButton(tr("确定"), QMessageBox::AcceptRole);
        if (box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
        return;
    }
    if (!to->GetExit())
    {
        QMessageBox box;
        box.setWindowTitle(tr("线路查询"));
        box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
        box.setIcon(QMessageBox::Critical);
        QString text(to->GetName());
        text += " 目前不可出站";
        box.setText(text);
        box.addButton(tr("确定"), QMessageBox::AcceptRole);
        if (box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
        return;
    }
    generalScene->removeItem((QGraphicsEllipseItem *)posItem);
    stateScene->removeItem((QGraphicsEllipseItem *)posItem_2);

    generalScene->removeItem((QGraphicsEllipseItem *)toItem);
    stateScene->removeItem((QGraphicsEllipseItem *)toItem_2);
    this->to = to;

    qreal x;
    qreal y;
    if (to->GetTransfer())
    {
        x = to->mapitem->rect().x() + 12.5 - 25;
        y = to->mapitem->rect().y() - 35;
    }
    else
    {
        x = to->mapitem->rect().x() + 11 - 25;
        y = to->mapitem->rect().y() - 35;
    }
    toItem->setRect(x, y, 50, 50);
    toItem->QGraphicsEllipseItem::setZValue(120);
    toItem_2->setRect(x, y, 50, 50);
    toItem_2->QGraphicsEllipseItem::setZValue(120);
    generalScene->addItem((QGraphicsEllipseItem *)toItem);
    stateScene->addItem((QGraphicsEllipseItem *)toItem_2);
    SearchPath();
}

void MainWindow::SearchPath()
{
    if (from == NULL || to == NULL)
        return;
    int type = 0;
    if (search->ui->minPath->isChecked())
        type = 1;
    else if (search->ui->minTrans->isChecked())
        type = 2;
    else if (search->ui->allPaths->isChecked())
        type = 3;
    if (type == 1)
    {
        map->Dijkstra(from, to, true);
    }
    else if (type == 2)
    {
        map->FindMinTransferPath(from, to);
    }
    else if (type == 3)
    {
        map->FindAllPaths(from, to);
        search->ui->charges->clear();
        if (allPaths.GetSize() != 0)
        {
            QString text = "共";
            text += QString::number(allPaths.GetSize());
            text += "条搜索结果";
            search->ui->charges->setText(text);
            search->ui->next->setVisible(1);
            ShowNextPaths();
        }
        else
        {
            QMessageBox box;
            box.setWindowTitle(tr("线路查询"));
            box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
            box.setIcon(QMessageBox::Critical);
            box.setText(tr("无查询结果！"));
            box.addButton(tr("确定"), QMessageBox::AcceptRole);
            generalScene->removeItem((QGraphicsEllipseItem *)fromItem);
            stateScene->removeItem((QGraphicsEllipseItem *)fromItem_2);
            generalScene->removeItem((QGraphicsEllipseItem *)toItem);
            stateScene->removeItem((QGraphicsEllipseItem *)toItem_2);
            if (box.exec() == QMessageBox::Accepted)
            {
                box.close();
            }
        }
    }
    from = NULL;
    to = NULL;
    path.Clear();
}

void MainWindow::PaintPath(CStation *sta, CIntersite *aLink)
{
    if (path.GetSize() == 0)
    {
        QMessageBox box;
        box.setWindowTitle(tr("线路查询"));
        box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
        box.setIcon(QMessageBox::Critical);
        box.setText(tr("无查询结果！"));
        box.addButton(tr("确定"), QMessageBox::AcceptRole);
        generalScene->removeItem((QGraphicsEllipseItem *)fromItem);
        stateScene->removeItem((QGraphicsEllipseItem *)fromItem_2);
        generalScene->removeItem((QGraphicsEllipseItem *)toItem);
        stateScene->removeItem((QGraphicsEllipseItem *)toItem_2);
        if (box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
        return;
    }
    path.Add(Node(0, aLink, sta));
    isSearch = true;
    search->ui->stationName->setEnabled(false);
    search->ui->search->setDisabled(true);
    qreal maxX = path[0].station->mapitem->rect().x();
    qreal minX = path[0].station->mapitem->rect().x();
    qreal maxY = path[0].station->mapitem->rect().y();
    qreal minY = path[0].station->mapitem->rect().y();
    Reset();
    white->QGraphicsEllipseItem::setZValue(80);
    QString pathText = "";
    for (int i = 0; i < path.GetSize(); i++)
    {
        CStation *sta = path[i].station;
        if (allPaths.GetSize() == 0)
        {
            pathText += map->GetLine(sta->GetLine())->GetName();
            pathText += "-";
            pathText += sta->GetName();
            if (i != path.GetSize() - 1)
                pathText += "→";
        }
        MapItem *staItem = sta->mapitem;
        staItem->QGraphicsEllipseItem::setZValue(100);
        qreal x = staItem->rect().x();
        qreal y = staItem->rect().y();
        if (x > maxX)
            maxX = x;
        if (x < minX)
            minX = x;
        if (y > maxY)
            maxY = y;
        if (y < minY)
            minY = y;
        CIntersite *link = path[i].link;
        if (link != NULL && link->GetDistance() != 0)
        {
            if (link->generalLink != NULL)
                link->generalLink->QGraphicsLineItem::setZValue(90);
            if (link->stateLink != NULL)
                link->stateLink->QGraphicsLineItem::setZValue(90);
        }
    }
    search->ui->textBrowser->setText(pathText);
    ui->subwayMap->centerOn((maxX + minX) / 2.0, (maxY + minY) / 2.0);
    isWhite = true;
    path.Clear();
}

void MainWindow::FindShortestPath(CStation *toSta, SeqList<SeqList<Node>> &path)
{
    if (path.GetSize() == 0)
        return;
    if (path[toSta->GetLine()][toSta->GetIndex()].station == NULL)
        return;
    CStation *preSta = path[toSta->GetLine()][toSta->GetIndex()].station;
    FindShortestPath(preSta, path);
    this->path.Add(path[toSta->GetLine()][toSta->GetIndex()]);
}

void MainWindow::ShowStaButton(CStation *sta)
{
    staButton->SetSta(sta);
    staButton->show();
    staButton->move(QCursor::pos());
}

void MainWindow::ShowNextPaths()
{
    CurrResult = (CurrResult + 1) % allPaths.GetSize();
    path = allPaths[CurrResult];
    PaintPath(allFrom, NULL);
}

void MainWindow::CountCharges(CIntersite *aLink)
{
    if (path.GetSize() == 0)
        return;
    path.Add(Node(0, aLink, 0));
    int charges = 0;
    int sum = 0;
    bool Separate = false;
    CIntersite *preLink = NULL;
    for (int i = 0; i < path.GetSize(); i++)
    {
        CIntersite *link = path[i].link;
        if (link != NULL && link->GetDistance() != 0)
        {
            if (link->GetCharges() == 0)
            {
                sum += link->GetDistance();
            }
            else if (link->GetCharges() == -1)
            {
                if (preLink == NULL || link->GetStation()->GetLine() != preLink->GetStation()->GetLine())
                    Separate = true;
                if (Separate)
                {
                    charges += map->GetLine(link->GetStation()->GetLine())->GetCharges();
                    Separate = false;
                }
            }
            else
                charges += link->GetCharges();
            preLink = link;
        }
    }
    charges += CountCharges(sum);
    path.Clear();
    QString text = "票价：";
    text += QString::number(charges);
    text += "元";
    search->ui->charges->setText(text);
}

int MainWindow::CountCharges(int sum)
{
    if (sum == 0)
        return 0;
    else if (sum <= 6000)
        return 3;
    else if (sum <= 12000)
        return 4;
    else if (sum <= 22000)
        return 5;
    else if (sum <= 32000)
        return 6;
    else
    {
        if ((sum - 32000) % 20000 != 0)
        {
            return (sum - 32000) / 20000 + 7;
        }
        else
            return (sum - 32000) / 20000 + 6;
    }
}

void MainWindow::SearchStation(CStation *station)
{
    if (station == NULL)
    {
        QMessageBox box;
        box.setWindowTitle(tr("站点查询"));
        box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
        box.setIcon(QMessageBox::Critical);
        box.setText(tr("无查询结果！"));
        box.addButton(tr("确定"), QMessageBox::AcceptRole);
        if (box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
        return;
    }
    Reset();
    generalScene->removeItem((QGraphicsEllipseItem *)posItem);
    stateScene->removeItem((QGraphicsEllipseItem *)posItem_2);
    qreal x;
    qreal y;
    if (station->GetTransfer())
    {
        x = station->mapitem->rect().x() + 12.5 - 25;
        y = station->mapitem->rect().y() - 35;
    }
    else
    {
        x = station->mapitem->rect().x() + 11 - 25;
        y = station->mapitem->rect().y() - 35;
    }
    posItem->setRect(x, y, 50, 50);
    posItem->QGraphicsEllipseItem::setZValue(120);
    posItem_2->setRect(x, y, 50, 50);
    posItem_2->QGraphicsEllipseItem::setZValue(120);
    generalScene->addItem((QGraphicsEllipseItem *)posItem);
    stateScene->addItem((QGraphicsEllipseItem *)posItem_2);
    ui->subwayMap->centerOn(x, y);
}

void MainWindow::AdminShow()
{
    admin->ui->tabWidget->setCurrentIndex(0);
    admin->ui->textEdit->setText(announcement);
    if (adminLogined == false)
    {
        if (adminLogin->exec() == QDialog::Accepted)
        {
            adminLogined = true;
            admin->show();
        }
    }
    else
        admin->show();
}
