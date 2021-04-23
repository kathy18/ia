#ifndef FICHA_H_INCLUDED
#define FICHA_H_INCLUDED

#include <vector>
#include <utility>
#include <iostream>
using namespace std;
typedef vector<vector<char> > Tablero;
class Ficha
{
public:
    int posX , posY;
    char id; /// 0 rojo, 1 negro;
    bool dama = 0;
    Ficha (int x,int y,bool col)
    {
        this->posX = x;
        this->posY = y;
        this->id = col;
    }
    Ficha (int x,int y,bool col, bool dama)
    {
        this->posX = x;
        this->posY = y;
        this->id = col;
        this->dama = dama;
    }
    ~Ficha(){};
};

#endif
