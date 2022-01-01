#include "player.h"

Player::Player() {
    reset_alldata();
}
Player::~Player() {

}
int Player::set_gameid(int id) {
    gameid = id;
    return 1;
}
int Player::get_gameid() {
    return gameid;
}
int Player::set_name(QString n) {
    name = n;
    return 1;
}
QString Player::get_name() {
    return name;
}
int Player::set_color(int c) {
    color = c;
    return 1;
}
int Player::get_color() {
    return color;
}
int Player::set_ob(int b) {
    ob = b;
    return 1;
}
int Player::get_ob() {
    return ob;
}
int Player::set_gameData(int classs,int timee,int data) {
    gameData[classs][timee] = data;
    return 1;
}
int Player::get_gameData(int classs,int timee) {
    return gameData[classs][timee];
}
int Player::set_side(int s) {
    side = s;
    return 1;
}
int Player::get_side() {
    return side;
}
int Player::reset_alldata() {
    gameid = -1;
    name = "";
    color = -1;
    ob = 0;
    side = -1;
    for(int i = 0; i< CLASS; ++i)
        for(int j = 0; j< TIME_LIMIT_1; ++j)
            gameData[i][j] = -1;
    return 1;
}
