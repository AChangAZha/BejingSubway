#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>

namespace Ui
{
    class Search;
}
class MainWindow;
class Search : public QWidget
{
    Q_OBJECT

public:
    Search(MainWindow *mainWindow, QWidget *parent = 0);
    ~Search();

private slots:
    void on_allLine_pressed(const QModelIndex &index);

    void paintEvent(QPaintEvent *event);

    void on_next_pressed();

    void on_search_pressed();

private:
    Ui::Search *ui;
    MainWindow *mainWindow;
    friend class MainWindow;
    friend class MapItem;
    friend class Admin;
};

#endif // SEARCH_H
