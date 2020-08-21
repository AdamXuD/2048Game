#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <ctime>
#include <cstring>

#include <conio.h>

class Game
{
    friend class MainWindow;
    bool isJustMove = false;
    int map[4][4];
    int score = 0;

public:
    Game(); //清空脏内存
    ~Game();

    void start();

private:
    void randNum(); //在地图上随机一个空位置生成 2 或 4
    void printMap();
    bool isOver();

    void compact(int line[]); //向左压缩
    void merge(int line[]); //合并相同项
    void rotateMap(int count); //旋转地图， round 圈数
    void move();

    void onUpKey();
    void onDownKey();
    void onLeftKey();
    void onRightKey();

    void waitForControl();

};


#endif // GAMEBASIC_H
