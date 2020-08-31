#ifndef GAME_H
#define GAME_H

#include "common.h"

class Game
{
    friend class MainWindow;

    bool isJustMove = false;
    struct GameData
    {
        int map[4][4];
        int score;
        unsigned int step = 0;

//        GameData() { memset(this, 0, sizeof(GameData)); }
    };

    GameData data;
    std::stack<GameData> history;

public:
    Game(); //清空脏内存
    ~Game();

    void start();

private:
    void randNum(); //在地图上随机一个空位置生成 2 或 4
    bool isOver();

    void compact(int line[]); //向左压缩
    void merge(int line[]); //合并相同项
    void rotateMap(int count); //旋转地图， round 圈数
    void move();

    void onUpKey();
    void onDownKey();
    void onLeftKey();
    void onRightKey();

    void undo();
    void recordMap();

};


#endif // GAMEBASIC_H
