#pragma once
#include <QString>
#include <QVariant>
#include <QSettings>

class Config
{
public:
    Config(QString filename = "");
    virtual~Config(void);

    QVariant Get(QString, QString);
    void Set(QString, QString, QVariant);
    void SetArray(QString qstrnodename, QString qstrkeyname, int i, QVector<double> x);
    void ReadArray(QString qstrnodename, QString qstrkeyname, int i, QVector<double>& x);
private:
    QSettings* m_setting;

};
