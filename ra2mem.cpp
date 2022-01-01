#include "ra2mem.h"
#include "ui_ra2mem.h"

RA2Mem::RA2Mem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RA2Mem)
{
    gameStatus = 0;
    elaspedTime = 0;
    battlePlayerCnt = 0;
    for(int t = 0; t < PLAYERNUM; ++t){
        for(int tt = 0; tt < TIME_LIMIT_1; ++tt)
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

DWORD RA2Mem::readMemory(HANDLE pid, int num_args, ...)
{
    DWORD m, next, add_m;
    va_list args;
    int i;

    va_start(args, num_args);
    for(i = 0; i < num_args; ++i) {
        if(0 == i) {
            m = va_arg(args, DWORD);
        } else {
            next = va_arg(args, DWORD);
            m = add_m + next;
        }
        ReadProcessMemory(pid, (void *)m, &add_m, 4, 0);
    }
    va_end(args);
    return add_m;
}

void RA2Mem::startTimer() {
    checkTimer = new QTimer();
    checkTimer->start(2500);
    connect(checkTimer, &QTimer::timeout, [&]()->void {
        DWORD aProcesses[1024], cbNeeded;
        if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
            emit EmitStatusCode(-1);
            return;
        }
        unsigned int cProcesses = cbNeeded / sizeof(DWORD);
        for (unsigned int i = 0; i < cProcesses; ++i)//遍历每个进程的名称和进程标识符
        {
            DWORD processID = aProcesses[i];
            if (processID == 0)
                continue;
            char buff[255];
            WCHAR gameFile[255];
            ZeroMemory(gameFile, 255);
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
            HMODULE hMod;
            DWORD cbNeeded;
            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                GetModuleBaseNameA(hProcess, hMod, (LPSTR)&buff, DWORD(sizeof(buff) / sizeof(char)));
            QString s = QString::fromLocal8Bit(buff);
            if (s == "gamemd-spawn.exe" || s == "gamemd.exe") {
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
void RA2Mem::switchStatusCode(int code) {

    if(code == 1) { // read spawnini
        reset_all_tmp_data();

        ui->pushButton->setDisabled(true);
        ui->comboBox->setDisabled(true);
        QString js = QString("reset_echart();");
        ui->webview1->page()->runJavaScript(js);
        Config ini(qGameFile + "\\spawn.ini");

        m_player[0].set_name(ini.Get("Settings", "Name").toString());
        localName = m_player[0].get_name();
        m_player[0].set_color(ini.Get("Settings", "Color").toInt());
        m_player[0].set_side(ini.Get("Settings", "Side").toInt());
        if (ini.Get("Settings", "IsSpectator") == "True") { //是观察者
            m_player[0].set_ob(1);
            spectatorCount++;
        }

        playerCount = ini.Get("Settings", "PlayerCount").toInt();
        for (int i = 1; i < playerCount; i++) {

            m_player[i].set_name(ini.Get(otherlist.at(i - 1), "Name").toString());
            m_player[i].set_color(ini.Get(otherlist.at(i - 1), "Color").toInt());
            m_player[i].set_side(ini.Get(otherlist.at(i - 1), "Side").toInt());
            if (ini.Get(otherlist.at(i - 1), "IsSpectator") == "True") { //是观察者
                m_player[i].set_ob(1);
                spectatorCount++;
            }
            else
                m_player[i].set_ob(0);

        }
        std::sort(m_player, m_player + playerCount);//排序数组从小到大

        for (int i = 0; i < playerCount; ++i)
            qDebug()    << "playerName:"     << m_player[i].get_name()
                        << "playerColor:"       << m_player[i].get_color()
                        << "isSpectatorPlayer:" << m_player[i].get_ob();
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
        /*
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
        */

        connect(checkTimer2, &QTimer::timeout, [&]()->void {
            HANDLE gameProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
            if (gameProcess == NULL)
                qDebug() << "Open Process Failed";
            //用于判断是否是观战
            if (elaspedTime <= 80) {

            }
            else {

            }

            elaspedTime++;
        });
    }

    else if(code == 0)//没有游戏
    {
        ui->pushButton->setEnabled(true);
        ui->comboBox->setEnabled(true);
        qDebug() << "come in 0";
        checkTimer2->stop();
        ui->label_2->setText("generated");
        /*
        QJsonObject jsonDocObj;
        jsonDocObj.insert("playerlist", playerlistArray);

        QJsonArray schema = generateEchartSchema();
        jsonDocObj.insert("schema", schema);

        QJsonArray option_series = generateEchartOptionSeries(0);
        jsonDocObj.insert("series", option_series);

        QJsonObject option_legend = generateEchartOptionLegend();
        jsonDocObj.insert("legend", option_legend);

        jsonDoc.setObject(jsonDocObj);
        gameData->write(jsonDoc.toJson());
        gameData->close();

        QString str = jsonDoc.toJson(QJsonDocument::Compact);
        str.replace(QRegExp("\""), "\\\"");
        QString js = QString("load_json(\"%1\");").arg(str);

        ui->webview1->page()->runJavaScript(js);
        */
    }
}

void RA2Mem::reset_all_tmp_data() {
    elaspedTime = 0;
    battlePlayerCnt = 0;
    spectatorCount = 0;
    for(int t = 0; t < PLAYERNUM; ++t){
        for(int tt = 0; tt < TIME_LIMIT_1; ++tt)
            dataArray[t][0][tt] = -1;
    }
}
