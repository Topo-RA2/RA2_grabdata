#ifndef ECHART_H
#define ECHART_H

#include "constant.h"

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
    QJsonArray generateEchartSchema();
    QJsonArray generateEchartOptionSeries(int);
    QJsonObject generateEchartOptionLegend();
    QJsonObject generateDataJson();
};

#endif // ECHART_H
