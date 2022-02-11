#include "echart.h"

std::thread rev_thread;
int cashWarning = 280;

Echart::Echart() {

    schema.append(qMakePair(QString::fromUtf8("date"),      QStringLiteral("s")));
    schema.append(qMakePair(QString::fromUtf8("consume"),   QStringLiteral("金钱消耗")));
    schema.append(qMakePair(QString::fromUtf8("soliders"),  QStringLiteral("大兵")));
    schema.append(qMakePair(QString::fromUtf8("dog"),       QStringLiteral("狗狗")));
    schema.append(qMakePair(QString::fromUtf8("miner"),     QStringLiteral("矿车")));
    schema.append(qMakePair(QString::fromUtf8("tank"),      QStringLiteral("坦克")));
    schema.append(qMakePair(QString::fromUtf8("zz"),        QStringLiteral("蜘蛛")));
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
    rev_thread.detach();

}
Echart::~Echart() {

}

void Echart::revFunction() {
    while(1) {
        data_mtx.lock();
        while (!data_queue.empty())
        {
            struct data_struct tmp = data_queue.front();
            data_queue.pop();
            //qDebug() << "tmp.id:" << tmp.id << "tmp.elaspedTime:" << tmp.elaspedTime << "tmp.soliderFactory:" << tmp.soliderFactory;
            if(-1 != tmp.elaspedTime)
            {
                currTime = tmp.elaspedTime;
                dataArray[tmp.id][consume][currTime]         = tmp.consume;
                dataArray[tmp.id][soliders][currTime]        = tmp.soliders;
                dataArray[tmp.id][dog][currTime]             = tmp.dog;
                dataArray[tmp.id][miner][currTime]           = tmp.miner;
                dataArray[tmp.id][mainTank][currTime]        = tmp.mainTank;
                dataArray[tmp.id][zz][currTime]              = tmp.zz;
                dataArray[tmp.id][warFactory][currTime]      = tmp.warFactory;
                dataArray[tmp.id][soliderFactory][currTime]  = tmp.soliderFactory;
                dataArray[tmp.id][cash][currTime]            = tmp.cash;
            }
        }
        data_mtx.unlock();
        _sleep(500);
    }
}

std::thread Echart::revThread() {
    return std::thread(&Echart::revFunction, this);
}

std::vector<int> Echart::judgeOb() {

    int tmp_array[8] = {0};
    for(int _id = 0; _id < playerCount; ++_id) {
        for(int _time = 20; _time < TIME_LIMIT_1; ++_time) {
            if(dataArray[_id][consume][TIME_LIMIT_1 - 1] > 0) {
                qDebug() << "dataArray[_id][consume][TIME_LIMIT_1]: " << dataArray[_id][consume][TIME_LIMIT_1 - 10];
                tmp_array[_id] = 1;
                break;
            }
        }
    }
    for(int i = 0; i < playerCount; ++i) {
        qDebug() << "tmp_array:" << tmp_array[i];
        if(1 == tmp_array[i]) {
            battle_player_vec.emplace_back(i);
            ++battlePlayerCnt;
        }
    }
    return battle_player_vec;
}
int Echart::set_playerCount(int p) {
    playerCount = p;
    return 1;
}
int Echart::set_battlePlayerNameVec(std::vector<QString> n) {
    battlePlayerNameVec = n;
    return 1;
}
int Echart::set_battlePlayerColorVec(std::vector<int> c) {
    battlePlayerColorVec = c;
    return 1;
}

void Echart::resetAllEchart() {
    isJudge = 0;
    playerCount = 0;
    battlePlayerCnt = 0;
    currTime = 0;
    for(int _id = 0; _id < PLAYERNUM; ++_id) {
        for(int _class = 0; _class < CLASS; ++_class) {
            for(int _time = 0; _time < TIME_LIMIT_2; ++_time) {
                dataArray[_id][_class][_time] = -1;
            }
        }
    }
    battlePlayerNameVec.clear();
    battlePlayerNameVec.shrink_to_fit();
    battlePlayerColorVec.clear();
    battlePlayerColorVec.shrink_to_fit();
    battle_player_vec.clear();
    battle_player_vec.shrink_to_fit();
}

QJsonArray Echart::generateEchartSchema() {

    QJsonArray schemaArray;
    for (int i = 0; i < schema.length(); ++i) {
        QJsonObject jsonObject;
        jsonObject.insert("name", schema[i].first);
        jsonObject.insert("index", i);
        jsonObject.insert("text", schema[i].second);
        schemaArray.append(jsonObject);
    }
    return schemaArray;
}
QJsonArray Echart::generateEchartOptionSeries(int opt){//opt=1,data=[]

    QJsonArray seriesJsonArray;
    qDebug() << "battlePlayerCnt:" << battlePlayerCnt << "currTime:" << currTime;
    for(int p_cnt = 0; p_cnt < battlePlayerCnt; ++p_cnt){//每个人
        QJsonArray playerArray;
        QJsonArray playerMarkPoint;
        QJsonArray playerMarkArea;
        int init_miner_num = 0, init_factory_num = 0;

        if(opt != 1) {
            //int jumpFlag = 0;
            for(int p_time = 0; p_time < currTime; ++p_time) {//每个时间
                if(p_time >= 10) {
                    if(dataArray[battle_player_vec[p_cnt]][cash][p_time] >= 0 && dataArray[battle_player_vec[p_cnt]][cash][p_time] <= cashWarning) {
                        playerMarkArea.append(QJsonArray{
                                                     QJsonObject{{"xAxis",p_time-1}},
                                                     QJsonObject{{"xAxis",p_time}}
                                                 });
                    }
                }

                QJsonArray frameArray;
                frameArray.append(p_time);//把时间放进去

                for(int p_class = 0; p_class < CLASS; ++p_class) {//有什么单位
                    if(dataArray[battle_player_vec[p_cnt]][consume][p_time] == 0 &&
                       dataArray[battle_player_vec[p_cnt]][p_class][p_time] >= 300000)
                        frameArray.append(0);
                    else
                        frameArray.append(dataArray[battle_player_vec[p_cnt]][p_class][p_time]);
                }
                playerArray.append(frameArray);
                if(p_time > 15 && p_time < currTime - 3)
                {
                    QJsonObject miner_change, factory_change;
                    if(init_miner_num != dataArray[battle_player_vec[p_cnt]][miner][p_time]){

                        init_miner_num = dataArray[battle_player_vec[p_cnt]][miner][p_time];
                        miner_change.insert("coord",
                                            QJsonArray{p_time,dataArray[battle_player_vec[p_cnt]][consume][p_time]+10});
                        miner_change.insert("value",
                                            QStringLiteral("%1牛").arg(init_miner_num));
                        playerMarkPoint.append(miner_change);
                    }
                    if(init_factory_num != dataArray[battle_player_vec[p_cnt]][warFactory][p_time]){
                        init_factory_num = dataArray[battle_player_vec[p_cnt]][warFactory][p_time];
                        factory_change.insert("coord",
                                            QJsonArray{p_time,dataArray[battle_player_vec[p_cnt]][consume][p_time]+10});
                        factory_change.insert("value",
                                            QStringLiteral("%1重").arg(init_factory_num));
                        playerMarkPoint.append(factory_change);
                    }
                }
            }
        }

        QJsonObject seriesJsonObj {
            {"name", battlePlayerNameVec[p_cnt]},
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
                            {"color", color2RGB[battlePlayerColorVec[p_cnt]]}
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
                                                    {{"color", color2RGB[battlePlayerColorVec[p_cnt] + 8]}}
                                                  }
                                     }
                         }
            }
        };
        seriesJsonArray.append(seriesJsonObj);
    }
    return seriesJsonArray;
}

QJsonObject Echart::generateEchartOptionLegend() {

    QJsonArray legendDataArray;
    QJsonObject selectedObj;
    //selectedObj.insert("delta",true);
    //legendDataArray.append("delta");
    for(int p_cnt = 0; p_cnt < battlePlayerCnt; ++p_cnt){//每个人
        legendDataArray.append(battlePlayerNameVec[p_cnt]);
        //selectedObj.insert(battlePlayerNameVec[p_cnt],false);
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
        {"selected", selectedObj}
    };
    return seriesJsonObj;
}

QJsonDocument Echart::generateAll() {
    QJsonObject jsonDocObj;

    for (int i = 0; i < battlePlayerCnt; ++i)
    {
        QJsonObject jsonObject;
        jsonObject.insert("Name", battlePlayerNameVec[i]);
        jsonObject.insert("Color", battlePlayerColorVec[i]);
        //jsonObject.insert("Country", playerCountry[i]);
        playerlistArray.append(jsonObject);
    }
    jsonDocObj.insert("playerlist", playerlistArray);

    QJsonArray schema = generateEchartSchema();
    jsonDocObj.insert("schema", schema);

    QJsonArray option_series = generateEchartOptionSeries(0);
    jsonDocObj.insert("series", option_series);

    QJsonObject option_legend = generateEchartOptionLegend();
    jsonDocObj.insert("legend", option_legend);

    jsonDoc.setObject(jsonDocObj);
    //qDebug() << "jsonDoc:" << jsonDoc;
    return jsonDoc;
}

