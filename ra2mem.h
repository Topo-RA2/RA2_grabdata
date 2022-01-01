#pragma once

#include <QMap>
#include <QFile>
#include <QPair>
#include <QtMath>
#include <QDebug>
#include <QTimer>
#include <QWidget>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QVariantMap>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QResizeEvent>

#include <qt_windows.h>
#include <psapi.h>
#include <stdarg.h>

#include "config.h"
#include "constant.h"
#include "player.h"
#include "echart.h"

enum dataArraySet {consume,soliders,dog,miner,mainTank,warFactory,soliderFactory,cash};

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
    QTimer *checkTimer, *checkTimer2;

public:
    DWORD PID = 0;
    int gameStatus;                     //游戏状态
    int elaspedTime;                    //游戏启动时间（每次进timer+1）
    QString qGameFile;                  //保存游戏运行的文件路径
    QFile* gameData;                    //保存游戏数据

    Player m_player[8];
    QString localName;                  //本机name
    QString playerName[PLAYERNUM];      //玩家name数组
    int playerCount = 0;                //A:总玩家数
    int battlePlayerCnt = 0;            //B:除去观战的玩家数
    int spectatorCount = 0;             //C = A - B
    int playerColor[PLAYERNUM] = { 0 }; //玩家颜色数组
    int playerCountry[PLAYERNUM];       //玩家国家数组
    QList<QString> otherlist = { "other1","other2", "other3", "other4", "other5", "other6", "other7", };

    int dataArray[PLAYERNUM][CLASS][TIME_LIMIT_1];


    int web_index = 0;

    void startTimer();
    void switchStatusCode(int);
    void reset_all_tmp_data();

    DWORD readMemory(HANDLE, int, ...);

};
