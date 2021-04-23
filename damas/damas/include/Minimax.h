#ifndef MINIMAX_H_INCLUDED
#define MINIMAX_H_INCLUDED
#include <vector>
#include <utility>
#include <iostream>
#include "Ficha.h"
using namespace std;


bool turno = 0; /// 0 rojo, 1 negro;
int Jugador1=0;
int Jugador2=0;

vector<Ficha *> fichas;
vector<Tablero> pos_movs(Tablero t, int x, int y, bool player){
    vector<Tablero> result;
    Tablero current;
    if(player){
        if(x-1>=0&&y-1>=0){
            if(t[x-1][y-1]=='0'){
                current=t;
                current[x][y]='0';
                current[x-1][y-1]='2';
                result.push_back(current);
            }
        }
        if(x-1>=0&&y+1<=7){
            if(t[x-1][y+1]=='0'){
                current=t;
                current[x][y]='0';
                current[x-1][y+1]='2';
                result.push_back(current);
            }
        }
    }
    else{
        if(x+1<=7&&y-1>=0){
            if(t[x+1][y-1]=='0'){
                current=t;
                current[x][y]='0';
                current[x+1][y-1]='1';
                result.push_back(current);
            }
        }
        if(x+1<=7&&y+1<=7){
            if(t[x+1][y+1]=='0'){
                current=t;
                current[x][y]='0';
                current[x+1][y+1]='1';
                result.push_back(current);
            }
        }
    }

    return result;
}

vector<Tablero> pos_mov_comer(Tablero t, int x, int y, bool player){
    vector<Tablero> result;
    Tablero current;
    if(player){
        if(x-2>=0&&y-2>=0){
            if(t[x-1][y-1]=='1'&&t[x-2][y-2]=='0'){
                current=t;
                current[x][y]='0';
                current[x-1][y-1]='0';
                current[x-2][y-2]='2';
                result.push_back(current);
            }
        }
        if(x-2>=0&&y+2<=7){
            if(t[x-1][y+1]=='1'&&t[x-2][y+2]=='0'){
                current=t;
                current[x][y]='0';
                current[x-1][y+1]='0';
                current[x-2][y+2]='2';
                result.push_back(current);
            }
        }
    }
    else{
        if(x+2<=7&&y-2>=0){
            if(t[x+1][y-1]=='2'&&t[x+2][y-2]=='0'){
                current=t;
                current[x][y]='0';
                current[x+1][y-1]='0';
                current[x+2][y-2]='1';
                result.push_back(current);
            }
        }
        if(x+2<=7&&y+2<=7){
            if(t[x+1][y+1]=='2'&&t[x+2][y+2]=='0'){
                current=t;
                current[x][y]='0';
                current[x+1][y+1]='0';
                current[x+2][y+2]='1';
                result.push_back(current);
            }
        }
    }

    return result;
}


class Minimax
{
    int Prof, maxProf, val;
    bool Maximizador;
    Tablero tab;
    vector<Minimax*> movimientos;
public:
    Minimax(Tablero b, int prof, int m_prof, bool player){
        this->tab = b;
        this->Prof=prof;
        this->Maximizador=player;
        this->maxProf = m_prof;
        vector<Tablero> posib1,posib2,posib;
        vector<Tablero> current;
        if(Prof<maxProf){
            for(int i=0;i<tab.size();++i){
                for(int j=0; j<tab[i].size();++j){
                    if(Maximizador){
                        if(tab[i][j]=='2'){
                            current=pos_movs(tab,i,j,Maximizador);
                            for(size_t i=0;i<current.size();++i)
                                posib1.push_back(current[i]);
                            current=pos_mov_comer(tab,i,j,Maximizador);
                            for(size_t i=0;i<current.size();++i)
                                posib2.push_back(current[i]);
                        }
                    }
                    else{
                        if(tab[i][j]=='1'){
                            current=pos_movs(tab,i,j,Maximizador);
                            for(size_t i=0;i<current.size();++i)
                                posib1.push_back(current[i]);
                            current=pos_mov_comer(tab,i,j,Maximizador);
                            for(size_t i=0;i<current.size();++i)
                                posib2.push_back(current[i]);
                        }
                    }
                }
            }
        }
        if(posib2.size()!=0)
            posib=posib2;
        else
            posib=posib1;
        for(size_t i=0;i<posib.size();++i){
            Minimax* m = new Minimax(posib[i],Prof+1,maxProf,!Maximizador);
            movimientos.push_back(m);
        }
    }
    void evaluacion()
    {
           if(movimientos.size()==0){
            int result=0;
            for(size_t i=0;i<tab.size();++i){
                for(size_t j=0;j<tab[i].size();++j){
                    if(tab[i][j]=='1')
                        --result;
                    if(tab[i][j]=='2')
                        ++result;
                }
            }
            val=result;
        }
        else{
            for(size_t i=0;i<movimientos.size();++i)
                movimientos[i]->evaluacion();
            if(Maximizador){
                int m=-10000,current;
                for(size_t i=0;i<movimientos.size();++i){
                    current=movimientos[i]->val;
                    if(current>m)
                        m=current;
                }
                val=m;
            }
            else{
                int m=100000,current;
                for(size_t i=0;i<movimientos.size();++i){
                    current=movimientos[i]->val;
                    if(current<m)
                        m=current;
                }
                val=m;
            }
        }
    }
    Tablero jugar()
    {
           Tablero result;
            int current=-10000;
            for(size_t i=0;i<movimientos.size();++i){
                if(movimientos[i]->val>current){
                    current=movimientos[i]->val;
                    result=movimientos[i]->tab;
                }
            }
            return result;
    }
    virtual ~Minimax(){}
};

void fin_partida()
{
    if(Jugador1==12)
    {
        cout<<"FIN, GANO EL JUGADOR 1"<<endl;
    }
    if(Jugador2 == 12)
    {
        cout<<"FIN, GANO EL JUGADOR 2"<<endl;
    }
}
#endif
