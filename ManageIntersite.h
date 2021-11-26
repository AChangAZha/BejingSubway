#ifndef MANAGEINTERSITE_H
#define MANAGEINTERSITE_H

#include <QDialog>

namespace Ui
{
    class ManageIntersite;
}

class AddIntersite;
class AddStation;
class ManageIntersite : public QDialog
{
    Q_OBJECT

public:
    ManageIntersite(QWidget *parent = 0);
    ~ManageIntersite();

private slots:
    void on_addIntersite_pressed();

    void on_intersiteTable_cellDoubleClicked(int row, int column);

private:
    Ui::ManageIntersite *ui;
    AddIntersite *addIntersite;
    AddStation *addStation;
    friend class AddStation;
    friend class AddIntersite;
    friend class Admin;
};

#endif // MANAGEINTERSITE_H
