#ifndef ECHART_H
#define ECHART_H

#include "constant.h"

struct data_struct{
    int id;
    int elaspedTime;
    int consume;
    int soliders;
    int dog;
    int miner;
    int mainTank;
    int zz;
    int warFactory;
    int soliderFactory;
    int cash;
};
enum dataArraySet {consume,soliders,dog,miner,mainTank,zz,warFactory,soliderFactory,cash};

class Echart{
private:
    QJsonDocument jsonDoc;
    QJsonArray playerlistArray;
    QJsonArray totalFrame;
    QList<QPair<QString,QString>> schema;
    QMap<int,QString> color2RGB;
public:
    Echart();
    ~Echart();
    std::mutex data_mtx;
    std::queue<struct data_struct> data_queue;
    int dataArray[PLAYERNUM][CLASS][TIME_LIMIT_2];
    int isJudge = 0;
    int currTime = 0;
    int playerCount = 0;
    int battlePlayerCnt = 0;
    std::vector<int> battle_player_vec;
    std::vector<QString> battlePlayerNameVec;
    std::vector<int> battlePlayerColorVec;

    void revDataArray();
    std::thread revThread();
    void revFunction();
    std::vector<int> judgeOb();
    void resetAllEchart();
    int set_playerCount(int);
    int set_battlePlayerNameVec(std::vector<QString>);
    int set_battlePlayerColorVec(std::vector<int>);
    QJsonArray generateEchartSchema();
    QJsonArray generateEchartOptionSeries(int);
    QJsonObject generateEchartOptionLegend();
    QJsonObject generateDataJson();
    QJsonDocument generateAll();
};

#endif // ECHART_H
