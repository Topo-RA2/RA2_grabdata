#pragma once

#include <QMap>
#include <QFile>
#include <QPair>
#include <QList>
#include <QtMath>
#include <QDebug>
#include <QTimer>
#include <QWidget>
#include <QDateTime>
#include <QJsonArray>
#include <QMessageBox>
#include <QFileDialog>
#include <QVariantMap>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QResizeEvent>

#include <qt_windows.h>
#include <psapi.h>

#include "config.h"

const int CLASS = 8;
const int NUM = 1500;
const int PLAYERNUM = 8;

namespace Ui {
class RA2Mem;
}

class RA2Mem: public QWidget
{
    Q_OBJECT
public:
    explicit RA2Mem(QWidget *parent = nullptr);
    ~RA2Mem();

signals:
    void EmitStatusCode(int);

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_clicked();
    void onResizeEcharts();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    Ui::RA2Mem *ui;
    bool echartIsLoaded;

public:
    enum dataArraySet {consume,soliders,dog,miner,mainTank,warFactory,soliderFactory,cash};

    DWORD PID = 0;
    QTimer* checkTimer;
    QTimer* checkTimer2;
    int gameStatus;
    int elaspedTime;
    QString qGameFile;//保存游戏运行的文件路径
    QFile* gameData;//保存游戏数据

    QString playerName[PLAYERNUM];
    int playerCount = 0;
    int battlePlayerCnt = 0;
    int playerColor[PLAYERNUM] = { 0 };
    int playerCountry[PLAYERNUM];
    int isSpectatorPlayer[PLAYERNUM];
    QList<QString> playerNameList;
    QList<int> playerColorList;
    QList<QString> otherlist = { "other1","other2", "other3", "other4", "other5", "other6", "other7", };
    QString localName;

    int dataArray[PLAYERNUM][CLASS][NUM];
    QMap<int,QString> color2RGB;
    QJsonDocument jsonDoc;
    QJsonArray playerlistArray;
    QJsonArray totalFrame;
    QList<QPair<QString,QString>> schema;
    int web_index = 0;

    void startTimer();
    void switchStatusCode(int);
    QJsonArray generateEchartSchema();
    QJsonArray generateEchartOptionSeries(int);
    QJsonObject generateEchartOptionLegend();
    QJsonObject generateDataJson();
    DWORD readMemory(HANDLE, DWORD);
    DWORD readMemory(HANDLE, DWORD, DWORD);
    DWORD readMemory(HANDLE, DWORD, DWORD, DWORD);
    DWORD readMemory(HANDLE, DWORD, DWORD, DWORD, DWORD, DWORD);

};
