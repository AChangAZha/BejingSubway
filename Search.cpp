#include "Search.h"
#include "ui_Search.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CSubwayMap.h"
#include "CLine.h"
#include "CStation.h"
#include "MapItem.h"
#include "QtMath"
Search::Search(MainWindow *mainWindow, QWidget *parent) : QWidget(parent),
                                                          ui(new Ui::Search)
{
    ui->setupUi(this);
    this->mainWindow = mainWindow;
    ui->allLine->addItem("全部线路");
    for (int i = 0; i < mainWindow->map->GetNumOfLine(); i++)
    {
        ui->allLine->addItem(mainWindow->map->GetLine(i)->GetName());
    }
    ui->minPath->setChecked(1);
    if (mainWindow->map->GetNumOfLine() > 10)
        ui->allPaths->setCheckable(false);
    ui->search->setFocus();
    ui->search->setShortcut(QKeySequence::InsertParagraphSeparator);
    ui->search->setShortcut(Qt::Key_Enter);
    ui->search->setShortcut(Qt::Key_Return);
    ui->search->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;padding:2px 4px;}"
                              "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                              "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->next->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;padding:2px 4px;}"
                            "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                            "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
}

Search::~Search()
{
    delete ui;
}

void Search::on_allLine_pressed(const QModelIndex &index)
{
    mainWindow->SwitchLine(index.row() - 1);
}

void Search::on_next_pressed()
{
    mainWindow->ShowNextPaths();
}

void Search::paintEvent(QPaintEvent *event)
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

void Search::on_search_pressed()
{
    mainWindow->SearchStation(mainWindow->map->SearchStation(ui->stationName->text()));
}
