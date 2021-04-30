#include <iostream>
#include <fstream>
#include <random>
#include <time.h>
//#include <Windows.h> milisegundos y temperatura
#include <vector>
#include <algorithm>

using namespace std;

#define seti(x,x1,x2) x.push_back(x1); x.push_back(x2)       // Set Individuo
#define setd2(x11) x11=int(x11*cifras); x11/=cifras          // Set cifras en un numero
#define setd(x11,x12,x21,x22) x11=int(x11*cifras); x11/=cifras; x12=int(x12*cifras); x12/=cifras; x21=int(x21*cifras); x21/=cifras; x22=int(x22*cifras); x22/=cifras   // Set cifras en min max de x y.
#define f(i,N,M) for(int i = N; i< M; i++)                   // Simplificación de for

//Variables 
int
D = 2,         //Func: Rastring.D Dimensiones
P = 20,        //Población
max_it = 200,  //Cantidad de poblaciones
A = 10,        //Func: Rastring.A
xmin = -10,    //Minimo en x
xmax = 10,     //Maximo en x
mostrar_rango = 1;

double
pi = 3.1415926535,
cifra = 1,         //Decimales [0-n]
m = 1,             //Mutacion [10-1]

//Derivados
cifras = pow(10, cifra),   //Decimales
M = m / cifras * 2;         //Mutacion 

struct Individuo { vector<double> x; double f;};

double fun(vector<double> x) {    // Rastring

    double aux = 0;
    f(i, 0, D) 
        aux += x[i] * x[i] - A * cos(2 * pi * x[i]);
    return A * D + aux;

}
double mod(double a, double b) {   // Mod para los rangos (mod modificado para funcionar con numeros negativos)

    if (b == 0) return 0;
    return a - b * int(a / b) + b * ((int(a) * int(b)) < 0);

}

Individuo generarIndividuo() {     // Retorna un individuo con los rangos iniciales

    Individuo aux;
    seti(
        aux.x, 
        xmin + mod(double(rand()) / cifras, xmax - xmin),
        xmin + mod(double(rand()) / cifras, xmax - xmin)
    );
    return aux;

}
vector<Individuo> generarPoblacion() {    // Retorna una población de individuos iniciales

    vector<Individuo>poblacion;
    f(i, 0, P) { 
        poblacion.push_back(generarIndividuo());
        poblacion.back().f = fun(poblacion.back().x);
    }
    return poblacion;

}

struct cmp { bool operator()(const Individuo& a, const Individuo& b) { return a.f < b.f; } }comp; // Comparador para usar el sort

struct Seleccion_Reproduccion {

    Seleccion_Reproduccion(int P) { min.f = INT_MAX; porcentaje = 100 / P; }

    vector<Individuo> cruze(int,double,double,double,double);
    void generarSelecionR();
    void generarNuevaPoblR();
    void MinSR(int);
    void funpaint();

    vector<Individuo> poblacion, paint;
    double porcentaje;         // La seleccion por reproduccion utiliza un porcentaje para la seleccion de individuos
    Individuo min;             // Se guarda el mejor individuo para retornarlo al final
    Individuo media;
};
vector<Individuo> Seleccion_Reproduccion::cruze(int P, double x11, double x12, double x21, double x22) {   // Se genera una poblacion con los individuos seleccionados
    Individuo aux;
    vector<Individuo>poblacion;
    double x, y;
    f(i, 0, P) {

        //Generamos x, y en los rangos de forma aleatoria
        x = x11 + mod(double(rand()) / cifras, (x12 - x11));
        y = x21 + mod(double(rand()) / cifras, (x22 - x21));

        //x, y a la cantidad de cifras necesarias, se carga el individuo
        setd2(x); setd2(y);
        seti(aux.x, x, y);
        poblacion.push_back(aux);

        // Se encuentra la funcion para los x, y seleccionados
        poblacion.back().f = fun(poblacion.back().x);
        aux.x.clear();
    }
    return poblacion;

}
void Seleccion_Reproduccion::generarNuevaPoblR() {

    int porc;
    vector<Individuo> aux;
    double min1 = INT_MAX, min2 = INT_MAX, max1 = INT_MIN, max2 = INT_MIN;

    //Encontramos los mejores parametros de entre los seleccionados (el menor y el mayor, dado que no se sabe a que direccion ir)
    f(i, 0, poblacion.size()) {
        if (min1 > poblacion[i].x[0]) min1 = poblacion[i].x[0];
        if (min2 > poblacion[i].x[1]) min2 = poblacion[i].x[1];
        if (max1 < poblacion[i].x[0]) max1 = poblacion[i].x[0];
        if (max2 < poblacion[i].x[1]) max2 = poblacion[i].x[1];
    }
    setd(min1, max1, min2, max2); // set valores con las cifras necesarias 

    //En el caso de que la población comienze a crear clones generamos un rango de mutación
    if (min1 == max1) min1 -= M / 2; max1 += M / 2;
    if (min2 == max2) min2 -= M / 2; max2 += M / 2;

    //Si se desea ver el rango 
    if (mostrar_rango) cout << " [" << min1 << " - " << max1 << "] " << " [" << min2 << " - " << max2 << "] ";

    //Generamos la población, un porcentaje con rangos de los seleccionados y los demás con los iniciales
    porc = P / (-2 * porcentaje / (1 - porcentaje));
    aux = cruze(porc, min1, max1, min2, max2);
    while (!aux.empty()) {
        poblacion.push_back(aux.back());
        aux.pop_back();
    }

    porc = (P / (-porcentaje / (1 - porcentaje))) - porc;
    aux = cruze(porc, xmin, xmax, xmin, xmax);
    while (!aux.empty()) {
        poblacion.push_back(aux.back());
        aux.pop_back();
    }
    double auxdou = 0;
    for (int i = 0; i < poblacion.size()-1; i++) {
        auxdou += poblacion[i].f;
    }
    auxdou /= poblacion.size()-1;
    cout << auxdou << endl;

}
void Seleccion_Reproduccion::generarSelecionR() {

    // Se ordena la población para seleccionar los menores
    sort(poblacion.begin(), poblacion.end(), comp);

    // Extraemos los mejores en el porcentaje indicado
    double i = 0, aux = P - (P / porcentaje);
    while (++i < aux) poblacion.pop_back();

}
void Seleccion_Reproduccion::MinSR(int in) { // Encontramos el menor y lo guardamos si es menor al menor de todos los tiempos
    
    f(i, 0, P) {
        if (poblacion[i].f < min.f) {
            min = poblacion[i];
            paint.push_back(min);
            cout << in ;
            cout << " [" << min.x[0] << " " << min.x[1] << "] " << min.f<<endl;
        }
    }
}
void Seleccion_Reproduccion::funpaint() {

    bool a = false;
    f(i, -10, 11) {
        cout << i;
        if (i != 10 && i != -10) cout << " ";
        if (i > -1) cout << " ";
        f (j,-10,11) {
            for (int k = paint.size() - 1; k > 0; k--)
                if (int(paint[k].x[0]) == j && int(paint[k].x[1]) == i) {
                    if (k < 10)cout << " ";
                    cout << k;
                    a = true;
                    break;
                }
            if (a) a = false;
            else cout << " -";
        }
        cout << endl;
    }

    cout << "  ";
    f(i, -10, 11) {
        cout << i;
        if (i > -2) cout << " ";
    }

    cout << endl << endl << " Los minimos son: ";
    f(i, 0, paint.size())
        cout << " [" << paint[i].x[0] << " " << paint[i].x[1] << "] " << paint[i].f;
    cout << endl;

}

Individuo S_Reproduccion(int porcentaje) { 

    Seleccion_Reproduccion R(porcentaje); // Se crea la clase con el porcentaje dado
    R.poblacion = generarPoblacion();     // Se genera una poblacion general 
    //f(i, 0, max_it) {                     // Usando el maximo de poblaciones
    for (int i = 0; R.min.f != 0; i++) {  // Siempre converge y usa la parada f(x,y) = 0
        R.generarSelecionR();             // Seleccionamos el porcentaje de poblacion 
        R.generarNuevaPoblR();            // Generamos nuevos individuos con los seleccionados y generales
        R.MinSR(i);                        // Revisamos si existe un optimo
      //  if (R.min.f == 0) return R.min;
        
//    int aux; cin >> aux;
//    system("cls");
    }
  
    R.funpaint();
    return R.min;

}

int main()
{

    srand(time(NULL));
    Individuo aux = S_Reproduccion(30);

}