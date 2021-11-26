#ifndef ADDINTERSITE_H
#define ADDINTERSITE_H

#include <QDialog>

namespace Ui
{
    class AddIntersite;
}

class CIntersite;
class ManageIntersite;
class AddIntersite : public QDialog
{
    Q_OBJECT

public:
    explicit AddIntersite(QWidget *parent = 0);
    ~AddIntersite();
    void loadLine();

private slots:
    void on_line_currentIndexChanged(int index);

    void on_addIntersite_pressed();

    void on_deleteIntersite_pressed();

private:
    Ui::AddIntersite *ui;
    ManageIntersite *manageIntersite;
    CIntersite *link;
    friend class ManageIntersite;
    friend class Admin;
};

#endif // ADDINTERSITE_H
