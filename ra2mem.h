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
#include <QThread>

#include <qt_windows.h>
#include <psapi.h>
#include <stdarg.h>

#include "config.h"
#include "constant.h"
#include "player.h"
#include "echart.h"
#include "setting.h"

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
    void on_pushButton_clicked();
    void on_settingButton_clicked();
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
    int gameFrame;                    //游戏启动时间（每次进timer+1）
    QString qGameFile;                  //保存游戏运行的文件路径
    QFile* gameData;                    //保存游戏数据
    HANDLE gameProcess;

    Player m_player[PLAYERNUM];
    QString localName;                  //本机name
    int playerCount = 0;                //A:总玩家数
    int battlePlayerCnt = 0;            //B:除去观战的玩家数
    int spectatorCount = 0;             //C = A - B
    int lastGameFrame[PLAYERNUM] = {0};
    int judgeGameCount[PLAYERNUM] = {0};
    bool isGameOver = false;
    std::vector<QString> battlePlayerNameVec;   //游戏玩家name数组
    std::vector<int> battlePlayerColorVec;      //游戏玩家color数组
    QList<QString> fieldlist = { "Settings", "other1", "other2", "other3", "other4", "other5", "other6", "other7", };
    std::vector<int> player_id_vec;
    std::vector<int> battle_player_id_vec;
    Echart echart;
    Setting *setting;

    struct data_struct lastFrameData[PLAYERNUM];

    void startTimer();
    void switchStatusCode(int);
    void reset_all_tmp_data();
    struct data_struct get_one_player_data(int);
    std::vector<global_unit_data_struct> get_all_unit(int);

    DWORD readMemory(HANDLE, DWORD);
    DWORD readMemory(HANDLE, DWORD, DWORD);
    DWORD readMemory(HANDLE, DWORD, DWORD, DWORD);
    DWORD readMemory(HANDLE, DWORD, DWORD, DWORD, DWORD, DWORD);

};
