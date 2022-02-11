#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Setting *ui;
};

#endif // SETTING_H
