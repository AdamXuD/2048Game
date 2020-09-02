#ifndef GAME_H
#define GAME_H

#include "common.h"

class Game
{
    bool isJustMove = false;

    GameData data;
    GameFlag flag;
    std::stack<GameData> history;

    std::function<void()> printer;
    std::function<void()> overHandler;

public:
    Game(); //清空脏内存
    ~Game();

    void start();

    void setPrinter(std::function<void()> func); //注册打印函数
    void setOverHandler(std::function<void()> func); //注册游戏结束函数
    void setUploaded();
    int getMaxNum();
    bool isOver();
    GameData getGameData();    //游戏数据的getter方法
    GameFlag getGameFlag();

    void moveOperation(Key key); //游戏操作的接口
    void undo(); //撤回的接口

private:
    void randNum(); //在地图上随机一个空位置生成 2 或 4

    void compact(int line[]); //向左压缩
    void merge(int line[]); //合并相同项
    void rotateMap(int count); //旋转地图， round 圈数
    void move();

    void onUpKey();
    void onDownKey();
    void onLeftKey();
    void onRightKey();

    void recordMap();

};


#endif // GAMEBASIC_H
