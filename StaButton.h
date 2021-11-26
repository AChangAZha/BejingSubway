#ifndef STABUTTON_H
#define STABUTTON_H

#include <QDialog>

namespace Ui
{
    class StaButton;
}
class CStation;
class MainWindow;
class StaButton : public QDialog
{
    Q_OBJECT

public:
    StaButton(MainWindow *mainWindow, QWidget *parent = 0);
    ~StaButton();
    void paintEvent(QPaintEvent *event);

    void SetSta(CStation *value);

private slots:
    void on_from_pressed();

    void on_to_pressed();

private:
    Ui::StaButton *ui;
    CStation *sta;
    MainWindow *mainWindow;
};
#endif // STABUTTON_H
