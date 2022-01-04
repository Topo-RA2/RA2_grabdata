#include "ra2mem.h"
#include "ui_ra2mem.h"

RA2Mem::RA2Mem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RA2Mem)
{
    gameStatus = 0;

    reset_all_tmp_data();

    ui->setupUi(this);
    echartIsLoaded = false;

    qsrand(QDateTime::currentDateTimeUtc().toTime_t());

    connect(ui->webview1, SIGNAL(loadFinished(bool)), this, SLOT(onResizeEcharts()));//完成加载
    QString url = QString("qrc:/htmlEcharts/view1.html");
    ui->webview1->load(QUrl(url));

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

            //reset
            QString reset_js = QString("reset_echart();");
            ui->webview1->page()->runJavaScript(reset_js);

            QString str = jsonDocument.toJson(QJsonDocument::Compact);
            str.replace(QRegExp("\""), "\\\"");
            QString load_js = QString("load_json(\"%1\");").arg(str);
            ui->webview1->page()->runJavaScript(load_js);
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
        QString reset_js = QString("reset_echart();");
        ui->webview1->page()->runJavaScript(reset_js);
        Config ini(qGameFile + "\\spawn.ini");

        playerCount = ini.Get("Settings", "PlayerCount").toInt();
        echart.set_playerCount(playerCount);
        for (int i = 0; i < playerCount; ++i) {
            player_id_vec.emplace_back(i);
            m_player[i].set_name(   ini.Get(fieldlist.at(i), "Name").   toString());
            m_player[i].set_color(  ini.Get(fieldlist.at(i), "Color").  toInt());
            m_player[i].set_side(   ini.Get(fieldlist.at(i), "Side").   toInt());
            if (ini.Get(fieldlist.at(i), "IsSpectator") == "True") { //是观察者
                m_player[i].set_ob(1);
                spectatorCount++;
            }
            else
                m_player[i].set_ob(0);

        }
        localName = m_player[0].get_name();
        std::sort(m_player, m_player + playerCount);//排序数组从小到大

        for (int i = 0; i < playerCount; ++i) {
            if(0 == m_player[i].get_ob()) {
                battlePlayerNameVec.emplace_back(m_player[i].get_name());
                battlePlayerColorVec.emplace_back(m_player[i].get_color());
            }

            qDebug()    << "playerName:"        << m_player[i].get_name()
                        << "playerColor:"       << m_player[i].get_color()
                        << "isSpectatorPlayer:" << m_player[i].get_ob();
        }
        //for(auto i : battlePlayerNameVec)
        //   qDebug() << "battlePlayerNameVec:" << i;
        echart.set_battlePlayerNameVec(battlePlayerNameVec);
        echart.set_battlePlayerColorVec(battlePlayerColorVec);

        emit EmitStatusCode(2);
    }

    else if(code == 2) { // read memory
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

        connect(checkTimer2, &QTimer::timeout, [&]()->void {
            gameProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
            if (gameProcess == NULL)
                qDebug() << "Open Process Failed";


            if (elaspedTime < TIME_LIMIT_1) {
                for (int i: player_id_vec) {
                    echart.data_mtx.lock();
                    echart.data_queue.push( get_one_player_data(i) );
                    echart.data_mtx.unlock();
                }
            }
            else {
                if(TIME_LIMIT_1 == elaspedTime) {
                    battle_player_id_vec = echart.judgeOb();
                    int cnt = 0;
                    for(int i = 0; i < playerCount; ++i)
                        m_player[i].set_name("");
                    if(battle_player_id_vec.size() == battlePlayerNameVec.size())
                        for(int i : battle_player_id_vec) {
                            m_player[i].set_name(battlePlayerNameVec[cnt++]);
                        }
                    qDebug() << "battle_player_id_vec:" << battle_player_id_vec;
                }
                for (int i : battle_player_id_vec) {
                    echart.data_mtx.lock();
                    echart.data_queue.push( get_one_player_data(i) );
                    echart.data_mtx.unlock();
                }
            }
            elaspedTime++;
        });
    }

    else if(code == 0)//没有游戏
    {
        ui->pushButton->setEnabled(true);
        qDebug() << "come in 0";
        checkTimer2->stop();
        ui->label_2->setText("generated");

        QJsonDocument tmp_jsonDoc = echart.generateAll();

        gameData->write(tmp_jsonDoc.toJson());
        gameData->close();

        QString str = tmp_jsonDoc.toJson(QJsonDocument::Compact);
        str.replace(QRegExp("\""), "\\\"");
        QString js = QString("load_json(\"%1\");").arg(str);

        ui->webview1->page()->runJavaScript(js);
    }
}

struct data_struct RA2Mem::get_one_player_data(int i) {

    int soliderFactoryAddress,  soliderFactoryCount;
    int warFactoryAddress,      warFactoryCount;
    int cashAddress,            cashCount;
    int consumeAddress,         consumeCount;

    int solidersAddress,        solidersCount1, solidersCount2, solidersCnt;
    int dogAddress,             dogCount1, dogCount2, dogCnt;
    int minerAddress,           minerCount1, minerCount2, minerCnt;
    int mainTankAddress,        mainTankCount1, mainTankCount2, mainTankCnt;
    int zzAddress,              zzCount = 0;

    int isAddressValid;

    qDebug() << "playerName:" << m_player[i].get_name();
    int playerAddress = readMemory(gameProcess, 0x884b94 + i * 4);
    int playerInfantryAddress = readMemory(gameProcess, 0x884b94 + i * 4, 0x557c);
    int playerTankAddress = readMemory(gameProcess, 0x884b94 + i * 4, 0x5568);
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
    if((elaspedTime <= 5) && (cashCount > 30000 || cashCount <= 0)) {
        cashCount = -1;
    }
    consumeAddress = playerAddress;
    consumeAddress += 0x2DC;
    isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(consumeAddress), &consumeCount, sizeof(consumeAddress), 0);
    if (isAddressValid == 0)
        consumeCount = 0;
    qDebug() << "m_player[i].get_side():" << m_player[i].get_side() << "consumeCount: " << consumeCount << "elaspedTime: " << elaspedTime;

    {
        //动员兵、美国大兵
        solidersAddress = playerInfantryAddress + 4;
        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(solidersAddress), &solidersCount1, sizeof(solidersAddress), 0);
        if (isAddressValid == 0)
            solidersCount1 = 0;
        solidersAddress = playerInfantryAddress + 0;
        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(solidersAddress), &solidersCount2, sizeof(solidersAddress), 0);
        if (isAddressValid == 0)
            solidersCount2 = 0;

        //狗狗
        dogAddress = playerInfantryAddress + 0x24;
        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(dogAddress), &dogCount1, sizeof(dogAddress), 0);
        if (isAddressValid == 0 || dogCount1 > 150)
            dogCount1 = 0;
        dogAddress = playerInfantryAddress + 0x70;
        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(dogAddress), &dogCount2, sizeof(dogAddress), 0);
        if (isAddressValid == 0 || dogCount2 > 150)
            dogCount2 = 0;

        //矿车
        minerAddress = playerTankAddress + 4;
        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(minerAddress), &minerCount1, sizeof(minerAddress), 0);
        if (isAddressValid == 0)
            minerCount1 = 0;
        minerAddress = playerTankAddress + 0x84;
        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(minerAddress), &minerCount2, sizeof(minerAddress), 0);
        if (isAddressValid == 0)
            minerCount2 = 0;

        //犀牛坦克、灰熊坦克
        mainTankAddress = playerTankAddress + 0xc;
        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(mainTankAddress), &mainTankCount1, sizeof(mainTankAddress), 0);
        if (isAddressValid == 0)
            mainTankCount1 = 0;
        mainTankAddress = playerTankAddress + 0x24;
        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(mainTankAddress), &mainTankCount2, sizeof(mainTankAddress), 0);
        if (isAddressValid == 0)
            mainTankCount2 = 0;

        solidersCnt = solidersCount1 + solidersCount1;
        dogCnt = dogCount1 + dogCount2;
        minerCnt = minerCount1 + minerCount2;
        mainTankCnt = mainTankCount1 + mainTankCount2;

        //zz
        zzAddress = playerTankAddress + 0x40;
        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(zzAddress), &zzCount, sizeof(zzAddress), 0);
        if (isAddressValid == 0)
            zzCount = 0;

        //重工
        warFactoryAddress = playerAddress + 0x5380;
        isAddressValid = ReadProcessMemory(gameProcess, (LPCVOID)(warFactoryAddress), &warFactoryCount, sizeof(warFactoryCount), 0);
        if (isAddressValid == 0)
            warFactoryCount = 0;

        //检测兵营是否存在
        soliderFactoryAddress = playerAddress + 0x537c;
        ReadProcessMemory(gameProcess, (LPCVOID)(soliderFactoryAddress), &soliderFactoryCount, sizeof(soliderFactoryCount), 0);
    }
    qDebug() << "soliderFactoryCount: " << soliderFactoryCount << "dogCnt:" << dogCnt<< "minerCnt:" << minerCnt << "mainTankCnt: " << mainTankCnt;
    struct data_struct tmp = {
        i,
        elaspedTime,
        consumeCount,
        solidersCnt,
        dogCnt,
        minerCnt,
        mainTankCnt,
        zzCount,
        warFactoryCount,
        soliderFactoryCount,
        cashCount
    };
    return tmp;
}

void RA2Mem::reset_all_tmp_data() {
    elaspedTime = 0;
    battlePlayerCnt = 0;
    spectatorCount = 0;
    gameProcess = NULL;
    battlePlayerNameVec.clear();
    battlePlayerNameVec.shrink_to_fit();
    battlePlayerColorVec.clear();
    battlePlayerColorVec.shrink_to_fit();
    player_id_vec.clear();
    player_id_vec.shrink_to_fit();
    echart.resetAllEchart();
}
