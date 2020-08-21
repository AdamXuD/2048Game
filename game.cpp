#include "game.h"

Game::Game() { srand((unsigned int)time(NULL)); }
Game::~Game() { }

void Game::start()
{
    memset(map, 0, sizeof(int[4][4]));
    randNum();
    randNum();
    while (true)
    {
        printMap();
        waitForControl();
        if (isOver()) return;
        if(isJustMove) randNum();
        isJustMove = false;
    }

    while(true)
    {
        std::cout << "Game Over. Your score is " << score << "." << std::endl;
        std::cout << "Would you like to restart? (y/n)" << std::endl;
        int key = _getch();
        if(key == 'N' ||key == 'n')
        {
            system("pause");
            break;
        }
        else if(key == 'Y' || key == 'y')
        {
            start();
            break;
        }
    }
}

void Game::randNum() //在地图上随机一个空位置生成 2 或 4
{
    int point[2];
    do {
        point[0] = rand() % 4;
        point[1] = rand() % 4;
    } while (this->map[point[0]][point[1]] != 0);
    this->map[point[0]][point[1]] = rand() % 2 ? 2 : 4;
}

void Game::printMap()
{
    system("cls");
    for (int row = 0; row < 4; row++)
    {
        printf("|");
        for (int col = 0; col < 4; col++)
            if (this->map[row][col] != 0)
                printf("%5d|", this->map[row][col]);
            else
                printf("%5c|", ' ');
        printf("\n-------------------------\n");
    }
    std::cout << "SCORE = " << this->score << std::endl;
}


bool Game::isOver()
{
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            if (map[row][col] == 0) return false; //如果有为 0 的格子
    //如果格子已满 则判断每一格能否与相邻格合并
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            if(map[row][col] == (row == 3 ? 0 : map[row + 1][col]) || map[row][col] == (col == 3 ? 0 : map[row][col + 1]))
                return false; //只要存在一个可合并的未结束

    return true; //否则即结束
}

void Game::compact(int line[]) //向左压缩
{
    int count = 0;
    for (int i = 0; i < 4; i++)
        if (line[i] != 0)
        {
            if (count != i)
            {
                line[count] = line[i];
                isJustMove = true;
            }
            count++;
        }
    for (; count < 4; count++) line[count] = 0;
}

void Game::merge(int line[]) //合并相同项
{
    for (int i = 0; i < 3; i++)
        if (line[i] == line[i + 1] && line[i] != 0)
        {
            line[i] *= 2;
            line[i + 1] = 0;

            score += line[i];
            isJustMove = true;
        }
}

void Game::rotateMap(int count) //旋转地图， round 圈数
{
    for (int i = 0; i < count; i++)
    {
        int tmp[4][4];
        memcpy(tmp, this->map, sizeof(int[4][4]));
        for (int row = 0; row < 4; row++)
            for (int col = 0; col < 4; col++)
                this->map[col][3 - row] = tmp[row][col];
    }
}

void Game::move()
{
    for (int i = 0; i < 4; i++)
    {
        compact(this->map[i]);
        merge(this->map[i]);
        compact(this->map[i]);
    }
}

void Game::onUpKey()
{
    rotateMap(3);
    move();
    rotateMap(1);
}

void Game::onDownKey()
{
    rotateMap(1);
    move();
    rotateMap(3);
}

void Game::onLeftKey()
{
    move();
}

void Game::onRightKey()
{
    rotateMap(2);
    move();
    rotateMap(2);
}

void Game::waitForControl()
{
    while (true)
    {
        int key = _getch();
        if (key == 'W' || key == 'w' || key == 'S' || key == 's' || key == 'A' || key == 'a' || key == 'D' || key == 'd')
        {
            switch (toupper(key))
            {
            case 'W': onUpKey(); break;
            case 'S': onDownKey(); break;
            case 'A': onLeftKey(); break;
            case 'D': onRightKey(); break;
            }
            break;
        }
    }
}

