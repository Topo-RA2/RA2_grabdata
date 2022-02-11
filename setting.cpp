#include "setting.h"
#include "ui_setting.h"
extern int cashWarning;
Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
    ui->textEdit->setText(QString::number(cashWarning));
}

Setting::~Setting()
{
    delete ui;
}

void Setting::on_pushButton_clicked()
{
    QString text = ui->textEdit->toPlainText();
    bool ok;
    int tmp = text.toInt(&ok);
    if(ok)
    {
        cashWarning = tmp;
        QMessageBox::information(NULL, NULL, "success", QMessageBox::Yes);
        this->close();
    }
    else
    {
        QMessageBox::information(NULL, NULL, "fail", QMessageBox::Yes);
    }
}


void Setting::on_pushButton_2_clicked()
{
    this->close();
}

