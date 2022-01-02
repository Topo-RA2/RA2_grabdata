#include "echart.h"

std::thread rev_thread;

Echart::Echart() {

    schema.append(qMakePair(QString::fromUtf8("date"),      QStringLiteral("s")));
    schema.append(qMakePair(QString::fromUtf8("consume"),   QStringLiteral("金钱消耗")));
    schema.append(qMakePair(QString::fromUtf8("soliders"),  QStringLiteral("大兵")));
    schema.append(qMakePair(QString::fromUtf8("dog"),       QStringLiteral("狗狗")));
    schema.append(qMakePair(QString::fromUtf8("miner"),     QStringLiteral("矿车")));
    schema.append(qMakePair(QString::fromUtf8("tank"),      QStringLiteral("坦克")));
    schema.append(qMakePair(QString::fromUtf8("factory"),   QStringLiteral("重工")));
    schema.append(qMakePair(QString::fromUtf8("s_factory"), QStringLiteral("兵营")));
    schema.append(qMakePair(QString::fromUtf8("money"),     QStringLiteral("金钱剩余")));

    color2RGB[0] = "rgba(238,238,0,1)";//黄
    color2RGB[1] = "rgba(255,48,48,1)";//红
    color2RGB[2] = "rgba(45,85,225,1)";//蓝
    color2RGB[3] = "rgba(34,139,34,1)";//绿
    color2RGB[4] = "rgba(205,133,0,1)";//橙
    color2RGB[5] = "rgba(64,215,208,1)";//天蓝
    color2RGB[6] = "rgba(160,32,240,1)";//紫
    color2RGB[7] = "rgba(205,150,205,1)";//粉

    color2RGB[8] = "rgba(238,238,0,0.1)";//黄
    color2RGB[9] = "rgba(255,48,48,0.1)";//红
    color2RGB[10] = "rgba(45,85,225,0.1)";//蓝
    color2RGB[11] = "rgba(34,139,34,0.1)";//绿
    color2RGB[12] = "rgba(205,133,0,0.1)";//橙
    color2RGB[13] = "rgba(64,215,208,0.1)";//天蓝
    color2RGB[14] = "rgba(160,32,240,0.1)";//紫
    color2RGB[15] = "rgba(205,150,205,0.1)";//粉

    rev_thread = revThread();
    rev_thread.join();

}
Echart::~Echart() {

}

void Echart::revFunction() {
    data_mtx.lock();
    while (!data_queue.empty())
    {
        struct data_struct tmp = data_queue.front();
        data_queue.pop();
        //dataArray[tmp.id]
    }
    data_mtx.unlock();


}

std::thread Echart::revThread() {
    return std::thread(&Echart::revFunction, this);
}

void Echart::judgeOb() {

    int p_count = 0;
    for(int _id = 0; _id < PLAYERNUM; ++_id) {
        if(-1 != dataArray[_id][0][TIME_LIMIT_1/2])
            ++p_count;
    }
    for(int _id = 0; _id < PLAYERNUM; ++_id) {
        for(int _class = 0; _class < CLASS; ++_class) {
            for(int _time = 0; _time < TIME_LIMIT_2; ++_time) {

                dataArray[_id][_class][_time] =
            }
        }
    }
}

void Echart::resetAllEchart() {
    isJudge = 0;
    for(int _id = 0; _id < PLAYERNUM; ++_id) {
        for(int _class = 0; _class < CLASS; ++_class) {
            for(int _time = 0; _time < TIME_LIMIT_2; ++_time) {
                dataArray[_id][_class][_time] = -1;
            }
        }
    }
}

QJsonArray Echart::generateEchartSchema() {
    /*
    QJsonArray schemaArray;
    for (int i = 0; i < schema.length(); ++i) {
        QJsonObject jsonObject;
        jsonObject.insert("name", schema[i].first);
        jsonObject.insert("index", i);
        jsonObject.insert("text", schema[i].second);
        schemaArray.append(jsonObject);
    }
    return schemaArray;
    */
    return QJsonArray{};
}
QJsonArray Echart::generateEchartOptionSeries(int opt){//opt=1,data=[]
    /*
    QJsonArray seriesJsonArray;

    for(int p_cnt = 0; p_cnt < battlePlayerCnt; ++p_cnt){//每个人
        //QJsonArray playerArray{QJsonArray{-2,0,0,0,0,0,0},QJsonArray{-1,0,0,0,0,0,0}};
        QJsonArray playerArray;
        QJsonArray playerMarkPoint;
        QJsonArray playerMarkArea;
        int init_miner_num = 0, init_factory_num = 0;

        if(opt != 1)
        {
            //int jumpFlag = 0;
            for(int p_time = 0; p_time < elaspedTime; ++p_time){//每个时间
                if(-1 == dataArray[p_cnt][cash][p_time])
                    continue;

                int sum = 0;
                for(int p_class = 0; p_class < CLASS; ++p_class){
                    sum += dataArray[p_cnt][p_class][p_time];
                }
                //if(sum >= 600000)
                //    break;
                if(p_time >= 10)
                {
                    if(dataArray[p_cnt][cash][p_time] >= 0 && dataArray[p_cnt][cash][p_time] <= 280)
                    {
                        playerMarkArea.append(QJsonArray{
                                                     QJsonObject{{"xAxis",p_time-1}},
                                                     QJsonObject{{"xAxis",p_time}}
                                                 });
                    }
                }

                QJsonArray frameArray;
                frameArray.append(p_time);//把时间放进去

                for(int p_class = 0; p_class < CLASS; ++p_class){//有什么单位
                    if(dataArray[p_cnt][p_class][p_time] >= 3000000)
                        frameArray.append(0);
                    else
                        frameArray.append(dataArray[p_cnt][p_class][p_time]);
                }
                playerArray.append(frameArray);

                QJsonObject miner_change, factory_change;
                if(init_miner_num != dataArray[p_cnt][miner][p_time]){

                    init_miner_num = dataArray[p_cnt][miner][p_time];
                    miner_change.insert("coord",
                                        QJsonArray{p_time,dataArray[p_cnt][consume][p_time]+10});
                    miner_change.insert("value",
                                        QStringLiteral("%1牛").arg(init_miner_num));
                    playerMarkPoint.append(miner_change);
                }
                if(init_factory_num != dataArray[p_cnt][warFactory][p_time]){
                    init_factory_num = dataArray[p_cnt][warFactory][p_time];
                    factory_change.insert("coord",
                                        QJsonArray{p_time,dataArray[p_cnt][consume][p_time]+10});
                    factory_change.insert("value",
                                        QStringLiteral("%1重").arg(init_factory_num));
                    playerMarkPoint.append(factory_change);
                }
            }
        }

        QJsonObject seriesJsonObj {
            {"name", playerNameList[p_cnt]},
            {"type", "line"},
            {"smooth", true},
            {"showSymbol", false},
            {"lineStyle", QJsonObject{
                    {"normal", QJsonObject{
                            {"width", 3}
                        }},
                }},
            {"itemStyle",QJsonObject{
                    {"normal", QJsonObject{
                            {"color", color2RGB[playerColorList[p_cnt]]}
                        }},
                    {"emphasis", QJsonObject{
                            {"color", "rgb(99,250,235)"},
                            {"borderColor", "rgba(99,250,235,0.2)"},
                            {"extraCssText", "box-shadow: 8px 8px 8px rgba(0,0,0,1);"},
                            {"borderWidth", 10},
                        }}
                }},
            {"data", playerArray},
            {"markPoint", QJsonObject{{"data", playerMarkPoint}}},
            {"markLine", QJsonObject{{"data", QJsonArray{
                                    QJsonObject{{"type", "average"},
                                        {"name", "Avg"}}
                            }}}},
            {"markArea", QJsonObject{{"data", playerMarkArea},
                                     {"itemStyle",QJsonObject{
                                                    {{"color", color2RGB[playerColorList[p_cnt] + 8]}}
                                                  }
                                     }
                         }
            }
        };
        seriesJsonArray.append(seriesJsonObj);
    }
    return seriesJsonArray;
    */
    return QJsonArray{};
}

QJsonObject Echart::generateEchartOptionLegend() {
/*
    QJsonArray legendDataArray;
    for(int p_cnt = 0; p_cnt < battlePlayerCnt; ++p_cnt){//每个人
        legendDataArray.append(playerNameList[p_cnt]);
    }
    QJsonObject seriesJsonObj {
        {"icon", "rect"},
        {"itemWidth", 14},
        {"itemHeight", 5},
        {"itemGap", 13},
        {"right", "6%"},
        {"textStyle", QJsonObject{
                {"fontSize", 16},
                {"color", "#F1F1F3"},
            }},
        {"data", legendDataArray},
    };
    return seriesJsonObj;
    */
    return QJsonObject{};
}


