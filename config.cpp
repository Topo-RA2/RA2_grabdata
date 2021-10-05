#include "config.h"

Config::Config(QString fileName)
{
    m_setting = new QSettings(fileName, QSettings::IniFormat);
    m_setting->setIniCodec("UTF-8");
}

Config::~Config()
{
    delete m_setting;
    m_setting = 0;
}

void Config::Set(QString qstrnodename, QString qstrkeyname, QVariant qstrvalue)
{
    m_setting->setValue(QString("%1/%2").arg(qstrnodename).arg(qstrkeyname), qstrvalue);
}

QVariant Config::Get(QString qstrnodename, QString qstrkeyname)
{
    QVariant value = m_setting->value(QString("%1/%2").arg(qstrnodename).arg(qstrkeyname));
    return value;
}

void Config::SetArray(QString qstrnodename, QString qstrkeyname, int i, QVector<double> x)
{
    m_setting->beginWriteArray(qstrnodename);
    m_setting->setArrayIndex(i);
    m_setting->setValue(qstrkeyname, x[i]);
    m_setting->endArray();
}

void Config::ReadArray(QString qstrnodename, QString qstrkeyname, int arraySize, QVector<double>& x)
{
    m_setting->beginReadArray(qstrnodename);
    for (int i = 0; i < arraySize; i++)
    {
        m_setting->setArrayIndex(i);
        x[i] = m_setting->value(qstrkeyname).toInt();
    }

    m_setting->endArray();
}
