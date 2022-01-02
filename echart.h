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
    int warFactory;
    int soliderFactory;
    int cash;
};

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


    void revDataArray();
    std::thread revThread();
    void revFunction();
    void judgeOb();
    void resetAllEchart();
    QJsonArray generateEchartSchema();
    QJsonArray generateEchartOptionSeries(int);
    QJsonObject generateEchartOptionLegend();
    QJsonObject generateDataJson();
};

#endif // ECHART_H
