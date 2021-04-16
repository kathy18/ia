#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include <GL/freeglut.h>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <utility>
#include <algorithm>
#include <queue>
#include <stack>
#include <list>
#include "button_funs.h"
#include "Algoritmos.h"
#include "time.h"


//#include <quadTree.h>
#define KEY_ESC 27

using namespace std;


void dibujar_grilla()
{
    float windowWidth = 550;
    float windowHeight = 550;
    float lineHDistance = 550/25;
    float lineVDistance = 550/25;


    glBegin(GL_LINES);
    glColor3f(0.4, 0.4, 0.4); // White (RGB)
    for( float x = -windowWidth; x < windowWidth+4; x += lineHDistance )
    {
        glVertex3f(x, -windowWidth, 0.0f);
        glVertex3f(x, float(windowHeight), 0.0f);
    }
    for( float y = -windowWidth; y < windowHeight + 4; y += lineVDistance )
    {
        glVertex3f(-windowWidth, y, 0.0f);
        glVertex3f(float(windowWidth), y, 0.0f);
    }
    glEnd();
}

vector<point> generate_points(int num){
    float windowWidth = 0;
    float windowHeight = 0;
    int a = 550/25;
    glPointSize(5.0f);
//    glColor3f(0.4,0.4,0.4);
    glColor3f(0.658824 ,0.658824 , 0.658824);
    vector<point> result(num*num);
    int j=0;
    for(int h=a; h < (num*a+1); h = h+a){
        for(int i=a; i <(num*a+1); i = i+a){
            points[j].x= windowWidth -a/2  + i;
            points[j].y= (windowHeight) -a/2  + h;
            points[j].state = 1;
            glBegin(GL_POINTS);
            glVertex2d((windowWidth) -a/2  + i, (windowHeight) -a/2  + h);
             glEnd();
             j++;
        }

    }
    return result;
}

void generate_edges()
{
    glColor3f(0.658824 ,0.658824 , 0.658824);
    int a = 550/25;
    for(int i=0; i< (25*25) ; i++)
    {
         if(points[i].state )
         {

            if(points[i].y < 550-a  && points[i].x < 550-a){
            glLineWidth(1);
            glBegin(GL_LINES);
            //glColor3f(0.4,0.4,0.4);
            if( points[i+1].state){
                glVertex2f(points[i].x, points[i].y );
                glVertex2f(points[i+1].x, points[i+1].y); //para un lado

                graph[i].push_back(i+1);
                graph[i+1].push_back(i);
            }


            if( points[i+25].state){
                glVertex2f(points[i].x, points[i].y );
                glVertex2f(points[i+25].x, points[i+25].y ); //para arriba

                graph[i].push_back(i+25);
                graph[i+25].push_back(i);
            }


            if( points[i+26].state){
                glVertex2f(points[i].x, points[i].y);// diagonal
                glVertex2f(points[i+26].x , points[i+26].y );

                graph[i].push_back(i+26);
                graph[i+26].push_back(i);
            }


            if( points[i+25].state && points[i+1].state ){
                glVertex2f(points[i+25].x, points[i+25].y );
                glVertex2f(points[i+1].x , points[i+1].y);

                graph[i+25].push_back(i+1);
                graph[i+1].push_back(i+25);
            }

            glEnd();
         }
         else
           {
               if(points[i].y > 550-a && points[i].y < 550+a && points[i].x < 500 + a ){
                   // cout<<"punto: "<<i<<endl;
                glBegin(GL_LINES);
                glVertex2f(points[i].x, points[i].y );
                glVertex2f(points[i+1].x , points[i+1].y); //para un lado
                glEnd();
                 graph[i].push_back(i+1);
                graph[i+1].push_back(i);
               }
               if(points[i].x > 550-a  && points[i].y < 500+a && points[i].x<550+a ){
                glBegin(GL_LINES);
                glVertex2f(points[i].x, points[i].y );
                glVertex2f(points[i+25].x, points[i+25].y ); //para arriba
                graph[i].push_back(i+25);
                graph[i+25].push_back(i);
                glEnd();
               }
           }
         }
        //}

    }//

}

void imprimir_ventana(GLdouble x, GLdouble y, const std::string &string)
{
    glColor3d(0.0, 0.0, 0.0);
    glRasterPos2d(x, y);
    for (int n=0; n<string.size(); ++n) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[n]);
    }
}
void printBFS()
{
    int prev[num_points], dist[num_points];

    clock_t ti;
    ti = clock();

    if (BFS(prev, dist) == false)
    {
        cout << "No hay camino"<<endl;
        return;
    }

    ti = clock() - ti;


    vector<int> path;
    int temp = fin;
    path.push_back(temp);
    while (prev[temp] != -1) {
        path.push_back(prev[temp]);
        temp = prev[temp];
    }

  ///imprimir
  glColor3f(0,0,0);
  string t = "tiempo: " +  to_string(((float)ti) / CLOCKS_PER_SEC );
  string d= "Tamanio del camino mas corto : " + to_string(dist[fin]);
  imprimir_ventana(10,565,t);
  imprimir_ventana(10, 578, d);
  string camino= "Camino: " + to_string(path[path.size() - 1]) +", ";
    ///

    for (int i = path.size()-2 ; i >= 0; i--){
        camino = camino + to_string(path[i]) + ", ";
        glLineWidth(3);
        glBegin(GL_LINES);
        glColor3f(0.0, 0.3, 1.0);
        glVertex2f(points[path[i+1]].x, points[path[i+1]].y);
        glVertex2f(points[path[i]].x, points[path[i]].y);
        glEnd();
    }
    imprimir_ventana(10, 591, camino);
}
void printDFS()
{
    int prev[num_points], dist[num_points];

     clock_t ti;
    ti = clock();
    if (DFS(prev, dist) == false)
    {
        cout << "No hay camino";
        return;
    }
    ti = clock() - ti;

    vector<int> path;
    int temp = fin;
    path.push_back(temp);
    while (prev[temp] != -1) {
        path.push_back(prev[temp]);
        temp = prev[temp];
    }
  //  cout << "Tamaño del camino mas corto : "<< dist[fin]<<endl;
    string t = "tiempo: " +  to_string(((float)ti) / CLOCKS_PER_SEC );
    string d= "Tamanio del camino mas corto : " + to_string(dist[fin]);
    imprimir_ventana(10,565,t);
    imprimir_ventana(10, 578, d);
    string camino= "Camino: " + to_string(path[path.size() - 1]) +", ";
    ///
    cout<<path[path.size() - 1]<<" ";
    for (int i = path.size()-2 ; i >= 0; i--){
        camino = camino + to_string(path[i]) + ", ";
        glLineWidth(3);
        glBegin(GL_LINES);
        glColor3f(0.0, 0.3, 1.0);
        glVertex2f(points[path[i+1]].x, points[path[i+1]].y);
        glVertex2f(points[path[i]].x, points[path[i]].y);
        glEnd();

    }
    imprimir_ventana(10, 591, camino);
}


void printBest()
{
    int prev[num_points], dist[num_points];

     clock_t ti;
    ti = clock();
    bool bf = best_first(prev, dist);
    ti = clock() - ti;

    if (bf == false)
    {
        cout << "No hay camino"<<endl;
        return;
    }
    vector<int> path;
    int temp = fin;
    path.push_back(temp);
    while (prev[temp] != -1) {
        path.push_back(prev[temp]);
        temp = prev[temp];
    }

  ///imprimir
  glColor3f(0,0,0);
  string t = "tiempo: " +  to_string(((float)ti) / CLOCKS_PER_SEC );
  string d= "Tamanio del camino mas corto : " + to_string(dist[fin]);
  imprimir_ventana(10,565,t);
  imprimir_ventana(10, 578, d);
  string camino= "Camino: " + to_string(path[path.size() - 1]) +", ";
    ///

    for (int i = path.size()-2 ; i >= 0; i--){
        camino = camino + to_string(path[i]) + ", ";
        glLineWidth(3);
        glBegin(GL_LINES);
        glColor3f(0.0, 0.3, 1.0);
        glVertex2f(points[path[i+1]].x, points[path[i+1]].y);
        glVertex2f(points[path[i]].x, points[path[i]].y);
        glEnd();
    }
    imprimir_ventana(10, 591, camino);
}

void eliminar()
{
    for(int i=0; i<ptsDel.size(); i++)
    {
        points[ptsDel[i]].state = 0;
        glPointSize(20.0f);
        glColor3f(0,0,0);
        glDisable(GL_POINT_SMOOTH);
        glBegin(GL_POINTS);
        glVertex2f(points[ptsDel[i]].x, points[ptsDel[i]].y);
        glEnd();
    }
    generate_edges();
}


void DrawButtons()
{
    ButtonDraw();
}
void displayGizmo()
{
    dibujar_grilla();
    generate_points(25);
   // eliminar(214);
   eliminar();
    generate_edges();
    if(check==1) printBFS();
    if(check==2) printDFS();
    if(check == 3) printBest();
    //if(check == 4) printA();
    glColor3f(0.5 ,1.0, 0.5 );
    //glPointSize(4.5);
    ///para dibujar los puntos de inicio y final
    glPointSize(15.0f);
    glDisable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glVertex2f(points[inicio].x, points[inicio].y);
    glColor3f(1.0 ,1.0, 0.0);
    glVertex2d(points[fin].x, points[fin].y);
    glEnd();

    glColor3f(0,0,0);
    Font(GLUT_BITMAP_8_BY_13, "I", points[inicio].x-2, points[inicio].y + 5);
    Font(GLUT_BITMAP_8_BY_13, "F", points[fin].x-2, points[fin].y + 5);


}

int click_point(int x, int y)
{
    for(int i=0; i<points.size(); i++)
    {
        if(x < points[i].x + 5 && x > points[i].x - 5 && y < points[i].y + 5 && y > points[i].y - 5){
            return i;
        }
    }
    return 0;
}
int pt;
void MouseButton(int button,int state,int x, int y)
{
	TheMouse.x = x;
	TheMouse.y = y;

	if (state == GLUT_DOWN)
	{
		//if ( !(TheMouse.lmb || TheMouse.mmb || TheMouse.rmb) ) {
			TheMouse.xpress = x;
			TheMouse.ypress = y;
		//}

		switch(button)
		{

		case GLUT_LEFT_BUTTON:
			TheMouse.lmb = 1;
			pt=click_point(x,y);
			if(pt!= 0) inicio = pt;
			ButtonPress(x,y);
			break;
		case GLUT_MIDDLE_BUTTON:
			TheMouse.mmb = 1;
			pt=click_point(x,y);
			if(pt!= 0) ptsDel.push_back(pt);
			break;
		case GLUT_RIGHT_BUTTON:
			TheMouse.rmb = 1;
			pt=click_point(x,y);
			if(pt!= 0) fin = pt;
			break;
		}
	}
	else
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			TheMouse.lmb = 0;
			ButtonRelease(x,y);
			break;
		case GLUT_MIDDLE_BUTTON:
			TheMouse.mmb = 0;
			break;
		case GLUT_RIGHT_BUTTON:
			TheMouse.rmb = 0;
			break;
		}
	}


	glutPostRedisplay();
}


void MouseMotion(int x, int y)
{
	/*
	 *	Calculate how much the mouse actually moved
	 */
	int dx = x - TheMouse.x;
	int dy = y - TheMouse.y;

	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;


	/*
	 *	Check MyButton to see if we should highlight it cos the mouse is over it
	 */
	ButtonPassive(x,y);

	/*
	 *	Force a redraw of the screen
	 */
	glutPostRedisplay();
}


void MousePassiveMotion(int x, int y)
{
	/*
	 *	Calculate how much the mouse actually moved
	 */
	int dx = x - TheMouse.x;
	int dy = y - TheMouse.y;

	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;

	/*
	 *	Check MyButton to see if we should highlight it cos the mouse is over it
	 */
	ButtonPassive(x,y);


}


//
//inicializacion de OpenGL
//
void init_GL(void) {
    //Color del fondo de la escena
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
    //modo projeccion

    CreateButton( "BFS", bfs_Callback,560, 100,30,30);
	CreateButton( "DFS", dfs_Callback,560, 140,30,30);
	CreateButton( "Best", best_Callback,560, 180,30,30);
	//CreateButton( "A*", a_Callback,560, 220,30,30);
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,600,600,0, -1.0f, 1.0f);
    // todas la informaciones previas se aplican al la matrice del ModelView
    glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(unsigned char key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
            TheMouse.mmb = 1;
			pt=click_point(x,y);
			if(pt!= 0) ptsDel.push_back(pt);
			break;
    case KEY_ESC:
        exit(0);
        break;

    default:
        break;
    }

}

void glPaint(void) {

    //El fondo de la escena al color initial
    //glClearColor(1.0f, 1.0f, 1.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
    	glClear( GL_COLOR_BUFFER_BIT |
			 GL_DEPTH_BUFFER_BIT );

    glLoadIdentity();


    //dibuja lo normal
    displayGizmo();


	DrawButtons();

    //doble buffer, mantener esta instruccion al fin de la funcion
    glutSwapBuffers();
}

//
//el programa principal
//
int main(int argc, char** argv) {

    //Inicializacion de la GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600); //tamaño de la ventana
    glutInitWindowPosition(100, 100); //posicion de la ventana
    glutCreateWindow("IA"); //titulo de la ventana
    init_GL(); //funcion de inicializacion de OpenGL


    glutDisplayFunc(glPaint);
    glutReshapeFunc(&window_redraw);
    // Callback del teclado
    glutKeyboardFunc(&window_key);
    glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);

    glutMainLoop(); //bucle de rendering
     init_GL();
    return 0;
}





//#ifdef __APPLE__
//#include <GLUT/glut.h>
//#else
//#include <GL/glut.h>
//#endif
//
//#include <stdlib.h>
//
//static int slices = 16;
//static int stacks = 16;
//
// GLUT callback Handlers
//
//static void resize(int width, int height)
//{
//    const float ar = (float) width / (float) height;
//
//    glViewport(0, 0, width, height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity() ;
//}
//
//static void display(void)
//{
//    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
//    const double a = t*90.0;
//
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glColor3d(1,0,0);
//
//    glPushMatrix();
//        glTranslated(-2.4,1.2,-6);
//        glRotated(60,1,0,0);
//        glRotated(a,0,0,1);
//        glutSolidSphere(1,slices,stacks);
//    glPopMatrix();
//
//    glPushMatrix();
//        glTranslated(0,1.2,-6);
//        glRotated(60,1,0,0);
//        glRotated(a,0,0,1);
//        glutSolidCone(1,1,slices,stacks);
//    glPopMatrix();
//
//    glPushMatrix();
//        glTranslated(2.4,1.2,-6);
//        glRotated(60,1,0,0);
//        glRotated(a,0,0,1);
//        glutSolidTorus(0.2,0.8,slices,stacks);
//    glPopMatrix();
//
//    glPushMatrix();
//        glTranslated(-2.4,-1.2,-6);
//        glRotated(60,1,0,0);
//        glRotated(a,0,0,1);
//        glutWireSphere(1,slices,stacks);
//    glPopMatrix();
//
//    glPushMatrix();
//        glTranslated(0,-1.2,-6);
//        glRotated(60,1,0,0);
//        glRotated(a,0,0,1);
//        glutWireCone(1,1,slices,stacks);
//    glPopMatrix();
//
//    glPushMatrix();
//        glTranslated(2.4,-1.2,-6);
//        glRotated(60,1,0,0);
//        glRotated(a,0,0,1);
//        glutWireTorus(0.2,0.8,slices,stacks);
//    glPopMatrix();
//
//    glutSwapBuffers();
//}
//
//
//static void key(unsigned char key, int x, int y)
//{
//    switch (key)
//    {
//        case 27 :
//        case 'q':
//            exit(0);
//            break;
//
//        case '+':
//            slices++;
//            stacks++;
//            break;
//
//        case '-':
//            if (slices>3 && stacks>3)
//            {
//                slices--;
//                stacks--;
//            }
//            break;
//    }
//
//    glutPostRedisplay();
//}
//
//static void idle(void)
//{
//    glutPostRedisplay();
//}
//
//const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
//const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
//const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };
//
//const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
//const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
//const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
//const GLfloat high_shininess[] = { 100.0f };
//
// Program entry point
//
//int main(int argc, char *argv[])
//{
//    glutInit(&argc, argv);
//    glutInitWindowSize(640,480);
//    glutInitWindowPosition(10,10);
//    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//
//    glutCreateWindow("GLUT Shapes");
//
//    glutReshapeFunc(resize);
//    glutDisplayFunc(display);
//    glutKeyboardFunc(key);
//    glutIdleFunc(idle);
//
//    glClearColor(1,1,1,1);
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
//
//    glEnable(GL_LIGHT0);
//    glEnable(GL_NORMALIZE);
//    glEnable(GL_COLOR_MATERIAL);
//    glEnable(GL_LIGHTING);
//
//    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
//    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//
//    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
//    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
//    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
//
//    glutMainLoop();
//
//    return EXIT_SUCCESS;
//}
