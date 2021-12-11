#include "AddLine.h"
#include "ui_AddLine.h"
#include "Admin.h"
#include <QColorDialog>
#include <QMessageBox>

//构造函数
AddLine::AddLine(QWidget *parent) : QDialog(parent),
                                    ui(new Ui::AddLine)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("线路管理"));
    setWindowIcon(QIcon(":/icon/icon/logo.png"));
    setWindowModality(Qt::ApplicationModal);
    Qt::WindowFlags flags = this->windowFlags();
    setWindowFlags(flags & ~Qt::WindowContextHelpButtonHint);

    lineColor = QColor(QString("#000000"));
    ui->charges->setReadOnly(true);

    ui->addLine->setFocus();
    ui->addLine->setDefault(true);

    ui->addLine->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;padding:2px 4px;}"
                               "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                               "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->setColor->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;}"
                                "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->deleteLine->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:2px groove rgb(0,78,162);border-radius:10px;padding:2px 4px;}"
                                  "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                  "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
}

//析构函数
AddLine::~AddLine()
{
    delete ui;
}

//添加线路按钮
void AddLine::on_addLine_pressed()
{
    if (ui->addLine->text() == "添加线路")
        admin->AddALine();
    else
        admin->ModifyLine();
}

//拾取颜色按钮
void AddLine::on_setColor_pressed()
{
    QColorDialog colorDialog;
    colorDialog.setOptions(QColorDialog::ShowAlphaChannel);
    colorDialog.exec();
    lineColor = colorDialog.currentColor();
}

//单独收费按钮
void AddLine::on_charges3_toggled(bool checked)
{
    ShowCharges();
}

//显示单独收费
void AddLine::ShowCharges()
{
    if (ui->charges3->isChecked())
    {
        ui->charges->setReadOnly(false);
    }
    else
        ui->charges->setReadOnly(true);
}

//删除线路按钮
void AddLine::on_deleteLine_pressed()
{
    QMessageBox box;
    box.setWindowTitle("删除线路");
    box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
    box.setIcon(QMessageBox::Warning);
    box.setText("确定要删除吗？");
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Ok);
    if (box.exec() == QMessageBox::Cancel)
    {
        return;
    }
    admin->DeleteLine();
    QMessageBox box_2;
    box_2.setWindowTitle(tr("删除线路"));
    box_2.setWindowIcon(QIcon(":/icon/icon/logo.png"));
    box_2.setIcon(QMessageBox::Information);
    box_2.setText(tr("删除成功！"));
    if (box_2.exec() == QMessageBox::Accepted)
    {
        box_2.close();
    }
    this->close();
}
