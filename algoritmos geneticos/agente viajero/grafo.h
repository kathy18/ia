#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>	//dibujar fig basicas
#include <fstream>


#define RATE 1
#define GENERACIONES 100

using namespace std;

unsigned int max_poblacion = 20;

int random(int ini, int size){
    random_device device;
    mt19937 generador(device());
    uniform_int_distribution<> distribucion(ini, size);

    return distribucion(generador);
}

void invertir(vector<int> &v, int ini, int fin)
{
	for(; ini < fin ;ini++, fin--){
        int temp = v[fin];
        v[fin] = v[ini];
        v[ini] = temp;
    }
}

float dist_euclideana(pair<int, int> A, pair<int, int> B)
{
	return sqrt( pow(B.first - A.first, 2) + pow(B.second - A.second, 2) );
}


class TSP{
	public:
		TSP(){}
		vector<pair<int, int>> ciudades;
		vector< vector<float> >distancias;			// dist entre cada nodo
		vector< vector<int> > descendencia;	// aleatoriamente

		vector<int> camino_corto;
		vector<float> mejores_valores;
		
		
		
		float distancia_total(vector<int>);
		void agregar_ciudad(pair<int, int>);
		void generar_poblacion();
		void crear_grafo_distancias();
		void dibujar_grafo();
		void mutacion(vector<int>&);
		void algoritmo_genetico();
		vector<int> PMX(vector<int>, vector<int>);
		void dibujar_mejor_camino();
		void dibujar_grafico();
		
};

float TSP::distancia_total(vector<int> camino)
{
	float d = 0;
	for(unsigned int i = 0;i < camino.size()-1 ;i++)
		d += distancias[camino[i]][camino[i+1]];
	
	return d;
}
void TSP::agregar_ciudad(pair<int, int> p)	{  ciudades.push_back(p);  }


void TSP::generar_poblacion()
{
	vector<int> orden;
	vector< vector<int> > permutaciones;
	
	for(unsigned int i = 0;i < ciudades.size();i++) orden.push_back(i);
	
	do{
		permutaciones.push_back(orden);					//permutaciones de ciudades segun sus indices
		
	} while( next_permutation(orden.begin(), orden.end()) );

	for(unsigned int i = 0;i < max_poblacion;i++)		// genera poblacion inicial aleatoria
	{
		int idx_pos = random(0, permutaciones.size() - 1);
		descendencia.push_back( permutaciones[idx_pos]);
		permutaciones.erase(permutaciones.begin()+idx_pos);
	}	
}


void TSP::crear_grafo_distancias()
{
	vector<float> temp(ciudades.size());
	distancias.assign(ciudades.size(), temp);
	
	for(unsigned int i = 0;i < ciudades.size();i++)
	{
		for(unsigned int j = i;j < ciudades.size();j++)
		{
			if( i == j ) distancias[i][j] = 0;
			else{
				float d = dist_euclideana(ciudades[i], ciudades[j]) * -1;
				distancias[i][j] = d;
				distancias[j][i] = d;
			}
		}
	}
}

void TSP::dibujar_grafo()
{
	for(unsigned int i = 0;i < ciudades.size();i++)
	{
		al_draw_filled_circle(ciudades[i].first, ciudades[i].second, 2, al_map_rgb(255,0,0));
		for(unsigned int j = i+1;j < ciudades.size();j++)
		{
			// desde i hasta j
			al_draw_line(ciudades[i].first, ciudades[i].second, ciudades[j].first, ciudades[j].second, al_map_rgb(255,255,255), 1.2 );
		}
	}
}




void TSP::mutacion(vector<int> &hijo)
{	
	if( (random(0, 10)) < RATE )
	{
		int p1 = random(0, hijo.size()/2);
		int p2 = random(p1+1, hijo.size()-1);
		
		invertir(hijo, p1, p2);
	}
}

void TSP::algoritmo_genetico()
{
	generar_poblacion();
	crear_grafo_distancias();
	unsigned int pos_camino_max;
	float mejor_camino = -100000;
	
	for(unsigned int i = 0;i < GENERACIONES; i++)
	{
		float sumatoria = 0;
		
		vector<float> tam_recorrido;	// recorrido de cada camino
		vector< vector<int> >nueva_descendencia;	// caminos de la siguiente generacion
		
		
		for(unsigned int it = 0;it < descendencia.size();it++)	//recorrer todas las descendencias
		{
			sumatoria += distancia_total(descendencia[it]); 
			tam_recorrido.push_back(distancia_total(descendencia[it]));
		}

		
		pos_camino_max = max_element(tam_recorrido.begin(), tam_recorrido.end()) - tam_recorrido.begin();
		
		int media = sumatoria / descendencia.size();
		
		
		for(unsigned int it = 0;it < descendencia.size();it++)	//recorrer todas las descendencias
		{
			float prob = tam_recorrido[it] / float(media);
			
			if(prob < 0.12)
			{
				descendencia.erase(descendencia.begin()+it);
			}
			else{
				unsigned int rd;
				do{
					rd = random(0, descendencia.size()-1);
				} while(rd == it);
				
				vector<int> orden = PMX(descendencia[it], descendencia[rd]);
				
				mutacion(orden);
				nueva_descendencia.push_back(orden);
			}
			
			
		}
		mejores_valores.push_back(tam_recorrido[pos_camino_max]);
		if(tam_recorrido[pos_camino_max] > mejor_camino ){
			mejor_camino = tam_recorrido[pos_camino_max];
			camino_corto = descendencia[pos_camino_max];
		}
		
		descendencia = nueva_descendencia;
	}
}

vector<int> TSP::PMX( vector<int> a, vector<int> b)
{
	int l = a.size();
	int left = random(0, l-1), right = random(0, l-1);
	if(left > right){
		int tmp = left; left = right; right = tmp;
	}
	bool done0,done1;
	vector<int> c = a, d = b;
	int tmp;
	for(int i = left;i<=right;i++)
	{
		done0 = done1 = false;
		for(int j = 0; j<l; j++)
		{
			if((done0 = (c[j] == b[i]))) c[j] = c[i]; //Mapping selected region
			if((done1 = (d[j] == a[i]))) d[j] = d[i];
			if(done0 && done1) break;
		}
	}
	for(int i = 0 ; i<l ; i++)
	{
		if((i>=left) && (i<=right)) {
			tmp = a[i];
			a[i] = b[i];
			b[i] = tmp;
		}else{
			a[i] = c[i];
			b[i] = d[i];
		}
	}
	return a;
}

void TSP::dibujar_mejor_camino()
{
	unsigned int i;
	for(i = 0;i < camino_corto.size()-1;i++)
	{
		al_draw_line(ciudades[camino_corto[i]].first, ciudades[camino_corto[i]].second, ciudades[camino_corto[i+1]].first, ciudades[camino_corto[i+1]].second, al_map_rgb(255,0,0), 2 );
	}
	
	al_draw_line(ciudades[camino_corto[i]].first, ciudades[camino_corto[i]].second, ciudades[camino_corto[0]].first, ciudades[camino_corto[0]].second, al_map_rgb(255,0,0), 2 );
}
