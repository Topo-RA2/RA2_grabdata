#include "ra2mem.h"
#include "ui_ra2mem.h"

RA2Mem::RA2Mem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RA2Mem)
{
    schema.append(qMakePair(QString::fromUtf8("date"),      QStringLiteral("s")));
    schema.append(qMakePair(QString::fromUtf8("consume"),     QStringLiteral("金钱消耗")));
    schema.append(qMakePair(QString::fromUtf8("soliders"),  QStringLiteral("大兵")));
    schema.append(qMakePair(QString::fromUtf8("dog"),       QStringLiteral("狗狗")));
    schema.append(qMakePair(QString::fromUtf8("miner"),     QStringLiteral("矿车")));
    schema.append(qMakePair(QString::fromUtf8("tank"),      QStringLiteral("坦克")));
    schema.append(qMakePair(QString::fromUtf8("factory"),   QStringLiteral("重工")));
    schema.append(qMakePair(QString::fromUtf8("s_factory"), QStringLiteral("兵营")));
    schema.append(qMakePair(QString::fromUtf8("money"),  QStringLiteral("金钱剩余")));

    color2RGB[0] = "rgba(238,238,0,1)";//黄
    color2RGB[1] = "rgba(255,48,48,1)";//红
    color2RGB[2] = "rgba(45,85,225,1)";//蓝
    color2RGB[3] = "rgba(34,139,34,1)";//绿
    color2RGB[4] = "rgba(205,133,0,1)";//橙
    color2RGB[5] = "rgba(64,215,208,1)";//天蓝
    color2RGB[6] = "rgba(160,32,240,1)";//紫
    color2RGB[7] = "rgba(205,150,205,1)";//粉

    gameStatus = 0;
    elaspedTime = 0;
    battlePlayerCnt = 0;
    for(int t = 0; t < PLAYERNUM; ++t){
        for(int tt = 0; tt < NUM; ++tt)
            dataArray[t][0][tt] = -1;
    }

    ui->setupUi(this);
    echartIsLoaded = false;

    qsrand(QDateTime::currentDateTimeUtc().toTime_t());

    connect(ui->webview1, SIGNAL(loadFinished(bool)), this, SLOT(onResizeEcharts()));//完成加载
    on_comboBox_currentIndexChanged(0);

    startTimer();
}
RA2Mem::~RA2Mem()
{
    delete ui;
}
void RA2Mem::onResizeEcharts()
{
    echartIsLoaded = true;
    ui->label_2->setText("initialization complete");
    QJsonObject sizeData;
    sizeData.insert("width", ui->webview1->width() - 20);
    sizeData.insert("height", ui->webview1->height() - 20);
    QString sizeStr = QJsonDocument(sizeData).toJson();
    QString js = QString("setSize(%1)").arg(sizeStr);
    ui->webview1->page()->runJavaScript(js);
}

void RA2Mem::resizeEvent(QResizeEvent *event)
{
    if(echartIsLoaded)
        onResizeEcharts();
}


void RA2Mem::on_comboBox_currentIndexChanged(int index)
{
    web_index = index;
    QString url = QString("qrc:/htmlEcharts/view%1.html").arg(index+1);
    ui->webview1->load(QUrl(url));
}

void RA2Mem::on_pushButton_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("选择存档文件"),"",tr("json(*.json)"));
    if(!fileName.isNull())
    {
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly|QFile::Text))
        {
            QMessageBox::warning(this, tr("错误"),tr("文件无法打开"));
            return;
        }
        QJsonParseError jsonParserError;
        QJsonDocument   jsonDocument = QJsonDocument::fromJson( file.readAll(), &jsonParserError );
        if ( !jsonDocument.isNull() && jsonParserError.error == QJsonParseError::NoError ) {
            ui->label_2->setText("json parser success");
            //TODO:内容检查
            QString str = jsonDocument.toJson(QJsonDocument::Compact);
            str.replace(QRegExp("\""), "\\\"");
            QString js = QString("load_json(\"%1\");").arg(str);
            ui->webview1->page()->runJavaScript(js);
        }
        file.close();
    }
}
DWORD RA2Mem::readMemory(HANDLE pid, DWORD m)
{
    DWORD add_m;
    ReadProcessMemory(pid, (void *)m, &add_m, 4, 0);
    return add_m;
}

DWORD RA2Mem::readMemory(HANDLE pid, DWORD m, DWORD n)
{
    DWORD add_m;
    ReadProcessMemory(pid, (void *)m, &add_m, 4, 0);
    m = add_m + n;
    ReadProcessMemory(pid, (void *)m, &add_m, 4, 0);
    return add_m;
}

DWORD RA2Mem::readMemory(HANDLE pid, DWORD m, DWORD n, DWORD o)
{
    DWORD add_m;
    ReadProcessMemory(pid, (void *)m, &add_m, 4, 0);
    m = add_m + n;
    ReadProcessMemory(pid, (void *)m, &add_m, 4, 0);
    m = add_m + o;
    ReadProcessMemory(pid, (void *)m, &add_m, 4, 0);
    return add_m;
}

DWORD RA2Mem::readMemory(HANDLE pid, DWORD m, DWORD n, DWORD o, DWORD p, DWORD q)
{
    DWORD add_m;
    ReadProcessMemory(pid, (void *)m, &add_m, 4, 0);
    m = add_m + n;
    ReadProcessMemory(pid, (void *)m, &add_m, 4, 0);
    m = add_m + o;
    ReadProcessMemory(pid, (void *)m, &add_m, 4, 0);
    m = add_m + p;
    ReadProcessMemory(pid, (void *)m, &add_m, 4, 0);
    m = add_m + q;
    ReadProcessMemory(pid, (void *)m, &add_m, 4, 0);
    return add_m;
}

void RA2Mem::startTimer(){
    checkTimer = new QTimer();
    checkTimer->start(2500);

    connect(checkTimer, &QTimer::timeout, [&]()->void {

        DWORD aProcesses[1024], cbNeeded;// 获取进程标识符列表。
        if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))//接收进程标识符PID的数组 数组的大小 数组返回的字节数
        {
            emit EmitStatusCode(-1);
            return;
        }

        unsigned int cProcesses = cbNeeded / sizeof(DWORD);//进程个数

        for (unsigned int i = 0; i < cProcesses; ++i)//遍历每个进程的名称和进程标识符
        {
            DWORD processID = aProcesses[i];
            if (processID == 0)
                continue;
            char buff[255];
            WCHAR gameFile[255];
            ZeroMemory(gameFile, 255);
            // 打开现有的本地过程对象
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
            //if (hProcess == NULL)
                //continue;

            // 获取进程名称。
            HMODULE hMod;
            DWORD cbNeeded;

            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                GetModuleBaseNameA(hProcess, hMod, (LPSTR)&buff, DWORD(sizeof(buff) / sizeof(char)));

            QString s = QString::fromLocal8Bit(buff);
            if (s == "gamemd-spawn.exe" || s == "gamemd.exe") {
                //获取进程的文件路径
                GetModuleFileNameExW(hProcess, hMod, gameFile, 1024);
                qGameFile = QString::fromWCharArray(gameFile);
                if (s == "gamemd-spawn.exe")
                    qGameFile.remove("\\gamemd-spawn.exe");
                else
                    qGameFile.remove("\\gamemd.exe");
                PID = processID;
                if (gameStatus != 1)//第一次发现进程
                {
                    ui->label_2->setText("gaming");
                    emit EmitStatusCode(1);//读spawnini
                }
                gameStatus = 1;
                break;
            }

            if (i == cProcesses - 1 && gameStatus == 1)//找了个遍还是没找到
            {
                PID = 0;
                gameStatus = 0;
                ui->label_2->setText("generating");
                emit EmitStatusCode(0);//游戏关闭
            }
            CloseHandle(hProcess);
        }
        qDebug() << PID << gameStatus;
    });
    connect(this, &RA2Mem::EmitStatusCode, this, &RA2Mem::switchStatusCode);
}
void RA2Mem::switchStatusCode(int code){

    if(code == 1) // read spawnini
    {
        for(int i=0; i < playerlistArray.size(); ++i)//清空数组
             playerlistArray.removeFirst();

        elaspedTime = 0;
        battlePlayerCnt = 0;
        for(int t = 0; t < PLAYERNUM; ++t){
            for(int tt = 0; tt < NUM; ++tt)
                dataArray[t][0][tt] = -1;
        }
        playerNameList.clear();
        playerColorList.clear();

        ui->pushButton->setDisabled(true);
        ui->comboBox->setDisabled(true);
        QString js = QString("reset_echart();");
        ui->webview1->page()->runJavaScript(js);
        Config ini(qGameFile + "\\spawn.ini");

        if (ini.Get("Settings", "IsSpectator") == "True")//自己是观察者
            isSpectatorPlayer[0] = 1;
        else
            isSpectatorPlayer[0] = 0;

        playerName[0] = ini.Get("Settings", "Name").toString();
        localName = playerName[0];
        playerColor[0] = ini.Get("Settings", "Color").toInt();
        playerCountry[0] = ini.Get("Settings", "Side").toInt();
        playerCount = ini.Get("Settings", "PlayerCount").toInt();

        for (int i = 1; i < playerCount; i++){
            playerName[i] = ini.Get(otherlist.at(i - 1), "Name").toString();
            playerColor[i] = ini.Get(otherlist.at(i - 1), "Color").toInt();
            playerCountry[i] = ini.Get(otherlist.at(i - 1), "Side").toInt();
            //是观察者
            if (ini.Get(otherlist.at(i - 1), "IsSpectator") == "True")
                isSpectatorPlayer[i] = 1;
            else
                isSpectatorPlayer[i] = 0;

        }
        //排序数组从小到大
        for (int i = 0; i < playerCount - 1; ++i){
            for (int j = 0; j < playerCount - i; ++j){
                if (playerColor[i] > playerColor[i + 1]){
                    std::swap(playerColor[i + 1],playerColor[i]);
                    swap(playerName[i + 1],playerName[i]);
                    std::swap(playerCountry[i + 1],playerCountry[i]);
                    std::swap(isSpectatorPlayer[i + 1],isSpectatorPlayer[i]);
                }
            }
        }
        for (int i = 0; i < playerCount; ++i)
            qDebug() << playerName[i] << "\t" << playerColor[i];

        for (int i = 0; i < playerCount; ++i){
            if (!isSpectatorPlayer[i])//不是观察者则显示名字
            {
                playerNameList.append(playerName[i]);
                playerColorList.append(playerColor[i]);
            }
        }
        emit EmitStatusCode(2);
    }

    else if(code == 2) // read memory
    {
        //准备修改文件名为当前时间
        QString currentDate = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm");
        gameData = new QFile(currentDate + ".json");
        if(!gameData->open(QIODevice::ReadWrite)) {
            qDebug() << "gameData open error";
        } else {
            qDebug() <<"gameData open!";
        }
        checkTimer2 = new QTimer;
        checkTimer2->start(1000);

        //i是第几个玩家 j是第几个活人
        for (int i = 0; i < playerCount; ++i)
        {
            if (isSpectatorPlayer[i] != 1)
            {
                QJsonObject jsonObject;
                jsonObject.insert("Name", playerName[i]);
                jsonObject.insert("Color", playerColor[i]);
                jsonObject.insert("Country", playerCountry[i]);
                playerlistArray.append(jsonObject);
                ++battlePlayerCnt;
            }
        }
        connect(checkTimer2, &QTimer::timeout, [&]()->void {
            HANDLE gameProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
            if (gameProcess == NULL)
                qDebug() << "Open Process Failed";;

            for (int i = 0, j = 0, k = 0; i < playerCount; i++)
            {
                int soliderFactoryAddress, soliderFactoryCount;
                int solidersAddress, solidersCount;
                int dogAddress, dogCount;
                int minerAddress, minerCount;
                int cashAddress, cashCount;
                int consumeAddress, consumeCount;
                int mainTankAddress, mainTankCount;
                int warFactoryAddress, warFactoryCount;
                int isAddressValid;

                //如果本机是观察者 则不会写入884b94那段内存
                if (isSpectatorPlayer[i] == 1 && playerName[i] == localName)
                    k = 1;
                //i代表第几个玩家 j代表经过了多少个不是观察者的玩家
                if (!isSpectatorPlayer[i])
                {
                    qDebug() << "playerName" << playerName[i] << "no spectator";
                    qDebug() << "i: " << i << "\tj: " << j;


                    int playerAddress = readMemory(gameProcess, 0x884b94 + (i - k) * 4);
                    int playerInfantryAddress = readMemory(gameProcess, 0x884b94 + (i - k) * 4, 0x557c);
                    int playerTankAddress = readMemory(gameProcess, 0x884b94 + (i - k) * 4, 0x5568);

                    if(1 == playerCount) { // offline
                        playerAddress = readMemory(gameProcess, 0xA8022C, i * 4);
                        playerInfantryAddress = readMemory(gameProcess, 0xA8022C, i * 4, 0x557c);
                        playerTankAddress = readMemory(gameProcess, 0xA8022C, i * 4, 0x5568);
                    }

                    //cash
                    cashAddress = playerAddress;
                    cashAddress += 0x30c;
                    isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(cashAddress), &cashCount, sizeof(minerAddress), 0);
                    if (isAddressValid == 0)
                        cashCount = 0;
                    qDebug() << "cashCount: " << cashCount << "elaspedTime: " << elaspedTime;
                    if((elaspedTime <= 5) && (cashCount > 30000 || cashCount <= 0))
                    {
                        qDebug() << "break";
                        break;
                    }
                    consumeAddress = playerAddress;
                    consumeAddress += 0x2DC;
                    isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(consumeAddress), &consumeCount, sizeof(consumeAddress), 0);
                    if (isAddressValid == 0)
                        consumeCount = 0;

                    if (playerCountry[i] >= 5)
                    {
                        //动员兵
                        solidersAddress = playerInfantryAddress + 4;
                        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(solidersAddress), &solidersCount, sizeof(solidersAddress), 0);
                        if (isAddressValid == 0)
                            solidersCount = 0;

                        //狗狗
                        dogAddress = playerInfantryAddress + 0x24;
                        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(dogAddress), &dogCount, sizeof(dogAddress), 0);
                        if (isAddressValid == 0)
                            dogCount = 0;

                        //矿车
                        minerAddress = playerTankAddress + 4;
                        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(minerAddress), &minerCount, sizeof(minerAddress), 0);
                        if (isAddressValid == 0)
                            minerCount = 0;

                        //犀牛坦克
                        mainTankAddress = playerTankAddress + 0xc;
                        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(mainTankAddress), &mainTankCount, sizeof(mainTankAddress), 0);
                        if (isAddressValid == 0)
                            mainTankCount = 0;

                        //重工
                        warFactoryAddress = playerAddress + 0x5380;
                        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(warFactoryAddress), &warFactoryCount, sizeof(warFactoryCount), 0);
                        if (isAddressValid == 0)
                            warFactoryCount = 0;

                        //检测兵营是否存在
                        soliderFactoryAddress = playerAddress + 0x537c;
                        ReadProcessMemory(gameProcess, (LPCVOID)(soliderFactoryAddress), &soliderFactoryCount, sizeof(soliderFactoryCount), 0);
                    }
                    else
                    {
                        //盟军
                        //大兵
                        solidersAddress = playerInfantryAddress + 0;
                        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(solidersAddress), &solidersCount, sizeof(solidersAddress), 0);
                        if (isAddressValid == 0)
                            solidersCount = 0;

                        //狗狗
                        dogAddress = playerInfantryAddress + 0x70;;
                        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(dogAddress), &dogCount, sizeof(dogAddress), 0);
                        if (isAddressValid == 0)
                            dogCount = 0;

                        //矿车
                        minerAddress = playerTankAddress + 0x84;
                        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(minerAddress), &minerCount, sizeof(minerAddress), 0);
                        if (isAddressValid == 0)
                            minerCount = 0;

                        //主战坦克
                        mainTankAddress = playerTankAddress + 0x24;;
                        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(mainTankAddress), &mainTankCount, sizeof(mainTankAddress), 0);
                        if (isAddressValid == 0)
                            mainTankCount = 0;

                        //重工
                        warFactoryAddress = playerAddress + 0x5380;
                        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(warFactoryAddress), &warFactoryCount, sizeof(warFactoryCount), 0);
                        if (isAddressValid == 0)
                            warFactoryCount = 0;

                        //检测兵营是否存在
                        soliderFactoryAddress = playerAddress + 0x537c;
                        ReadProcessMemory(gameProcess, (LPCVOID)(soliderFactoryAddress), &soliderFactoryCount, sizeof(soliderFactoryCount), 0);

                    }//else end

                    dataArray[j][consume][elaspedTime] = consumeCount;
                    dataArray[j][soliders][elaspedTime] = solidersCount;
                    dataArray[j][dog][elaspedTime] = dogCount;
                    dataArray[j][miner][elaspedTime] = minerCount;
                    dataArray[j][mainTank][elaspedTime] = mainTankCount;
                    dataArray[j][warFactory][elaspedTime] = warFactoryCount;
                    dataArray[j][soliderFactory][elaspedTime] = soliderFactoryCount;
                    dataArray[j][cash][elaspedTime] = cashCount;
                    j++;
                }
            }
            elaspedTime++;
        });
    }
    ///*
    else if(code == 0)//没有游戏
    {
        ui->pushButton->setEnabled(true);
        ui->comboBox->setEnabled(true);
        qDebug() << "come in 0";
        checkTimer2->stop();
        ui->label_2->setText("generated");
        QJsonObject jsonDocObj;
        jsonDocObj.insert("playerlist", playerlistArray);

        QJsonArray schema = generateEchartSchema();
        jsonDocObj.insert("schema", schema);

        QJsonArray option_series = generateEchartOptionSeries(0);
        jsonDocObj.insert("series", option_series);

        QJsonObject option_legend = generateEchartOptionLegend();
        jsonDocObj.insert("legend", option_legend);

        jsonDoc.setObject(jsonDocObj);
        qDebug()<<jsonDoc.toJson();
        gameData->write(jsonDoc.toJson());
        gameData->close();

        QString str = jsonDoc.toJson(QJsonDocument::Compact);
        str.replace(QRegExp("\""), "\\\"");
        QString js = QString("load_json(\"%1\");").arg(str);

        ui->webview1->page()->runJavaScript(js);
    }
    //*/
}

QJsonArray RA2Mem::generateEchartSchema(){
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
QJsonArray RA2Mem::generateEchartOptionSeries(int opt){//opt=1,data=[]
    QJsonArray seriesJsonArray;

    for(int p_cnt = 0; p_cnt < battlePlayerCnt; ++p_cnt){//每个人
        //QJsonArray playerArray{QJsonArray{-2,0,0,0,0,0,0},QJsonArray{-1,0,0,0,0,0,0}};
        QJsonArray playerArray;
        QJsonArray playerMarkPoint;
        int init_miner_num = 0, init_factory_num = 0;

        if(opt != 1)
        {
            //int jumpFlag = 0;
            for(int p_time = 0; p_time < elaspedTime; ++p_time){//每个时间
                if(-1 == dataArray[p_cnt][0][p_time])
                {
                    continue;
                }
                int sum = 0;
                for(int p_class = 0; p_class < CLASS; ++p_class){
                    sum += dataArray[p_cnt][p_class][p_time];
                }
                //if(sum >= 600000)
                //    break;

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
                                        QStringLiteral("%1矿车").arg(init_miner_num));
                    playerMarkPoint.append(miner_change);
                }
                if(init_factory_num != dataArray[p_cnt][warFactory][p_time]){
                    init_factory_num = dataArray[p_cnt][warFactory][p_time];
                    factory_change.insert("coord",
                                        QJsonArray{p_time,dataArray[p_cnt][consume][p_time]+10});
                    factory_change.insert("value",
                                        QStringLiteral("%1重工").arg(init_factory_num));
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
                            }}}
            }
        };
        seriesJsonArray.append(seriesJsonObj);
    }
    return seriesJsonArray;
}

QJsonObject RA2Mem::generateEchartOptionLegend(){

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
}


