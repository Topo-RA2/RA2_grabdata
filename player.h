#ifndef PLAYER_H
#define PLAYER_H

#include "constant.h"

class Player {
private:

    int gameid;
    QString name;
    int color;
    int ob;//0:No,1:Yes
    int gameData[CLASS][TIME_LIMIT_2];
    int side;

public:
    Player();
    ~Player();

    int set_gameid(int);
    int get_gameid();
    int set_name(QString);
    QString get_name();
    int set_color(int);
    int get_color();
    int set_ob(int);
    int get_ob();
    int set_gameData(int,int,int);
    int get_gameData(int,int);
    int set_side(int);
    int get_side();
    int reset_alldata();
    bool operator < (const Player &other) const {
        return color < other.color;
    }
    bool operator > (const Player &other) const {
        return color > other.color;
    }
};

#endif // PLAYER_H
