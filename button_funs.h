#ifndef BUTTON_FUNS_H_INCLUDED
#define BUTTON_FUNS_H_INCLUDED

#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sstream>
using namespace std;

/* colores
1.0 0.0 0.0 red, duh
0.0 1.0 0.0 green, duh
0.0 0.0 1.0 blue, duh
1.0 1.0 0.0 yellow
1.0 0.0 1.0 purple
0.0 1.0 1.0 cyan
1.0 1.0 1.0 white
1.0 0.5 0.0 orange
0.5 1.0 0.0 greenish yellow
…
0.5f, 0.35f, 0.05f brown
0.5 1.0 0.5 light green
0.0 0.5 0.0 dark green
*/

std::string to_string(int val) {
  std::stringstream ss;
  ss << val;
  return ss.str();
}
std::string to_string(float val) {
  std::stringstream ss;
  ss << val;
  return ss.str();
}

struct Mouse
{
	int x;		/*	the x coordinate of the mouse cursor	*/
	int y;		/*	the y coordinate of the mouse cursor	*/
	int lmb;	/*	is the left button pressed?		*/
	int mmb;	/*	is the middle button pressed?	*/
	int rmb;	/*	is the right button pressed?	*/
	int xpress; /*	stores the x-coord of when the first button press occurred	*/
	int ypress; /*	stores the y-coord of when the first button press occurred	*/
};

typedef struct Mouse Mouse;

Mouse TheMouse = {0,0,0,0,0};

int winw = 600;
int winh = 600;

typedef void (*ButtonCallback)();
struct Button
{
	int   x;							/* top left x coord of the button */
	int   y;							/* top left y coord of the button */
	int   w;							/* the width of the button */
	int   h;							/* the height of the button */
	int	  state;						/* the state, 1 if pressed, 0 otherwise */
	int	  highlighted;					/* is the mouse cursor over the control? */
	char* label;						/* the text label of the button */
	ButtonCallback callbackFunction;	/* A pointer to a function to call if the button is pressed */

	int id;								/* A unique ID to represent this Button */

	struct Button* next;
};
typedef struct Button Button;

int check=0;
void bfs_Callback()
{
	check = 1;
}
void dfs_Callback()
{
	check = 2;
}
void best_Callback()
{
	check = 3;
}
void a_Callback()
{
	check = 4;
}

//Button MyButtonBFS = {560,100, 30,30, 0,0, "BFS", bfs_Callback };
//Button MyButtonDFS = {560,140, 30,30, 0,0, "DFS", dfs_Callback };
//Button MyButtonA = {560,180, 30,30, 0,0, "A*", a_Callback };
Button* pButtonList = NULL;

int GlobalRef=0;

/*
 *
 */
int CreateButton(char *label,ButtonCallback cb,int x,int y,int w,int h)
{
	Button* p = (Button*)malloc( sizeof(Button) );
	assert(p);
	memset(p,0,sizeof(Button));
	p->x = x;
	p->y = y;
	p->w = w;
	p->h = h;
	p->callbackFunction = cb;
	p->label = (char*)malloc( strlen(label)+1 );
	if(p->label)
		sprintf(p->label,label);

	p->next = pButtonList;
	pButtonList = p;

	return p->id = ++GlobalRef;
}

int DeleteButtonByName(char *label)
{
	Button* previous=NULL,*curr=pButtonList;
	while (curr!=NULL) {
		if (strcmp(label,curr->label)==0) {
			if(previous)
				previous->next = curr->next;
			else
				pButtonList = curr->next;

			if (curr->label) {

				free(curr->label);
			}
			free(curr);
			return 1;
		}
		previous = curr;
		curr = curr->next;
	}
	return 0;
}

int DeleteButtonById(int id)
{
	Button	*previous = NULL       ,
			*curr     = pButtonList;

	while (curr!=NULL)
	{
		if ( id == curr->id )
		{
			if(previous)
				previous->next = curr->next;
			else
				pButtonList = curr->next;

			if (curr->label) {

				free(curr->label);
			}
			free(curr);
			return 1;
		}
		previous = curr;
		curr = curr->next;
	}
	return 0;
}


/*----------------------------------------------------------------------------------------
 *	This is an example callback function. Notice that it's type is the same
 *	an the ButtonCallback type. We can assign a pointer to this function which
 *	we can store and later call.
 */
static void TheButtonCallback()
{
	printf("I have been called\n");
}




/*----------------------------------------------------------------------------------------
 *	\brief	This function draws a text string to the screen using glut bitmap fonts.
 *	\param	font	-	the font to use. it can be one of the following :
 *
 *					GLUT_BITMAP_9_BY_15
 *					GLUT_BITMAP_8_BY_13
 *					GLUT_BITMAP_TIMES_ROMAN_10
 *					GLUT_BITMAP_TIMES_ROMAN_24
 *					GLUT_BITMAP_HELVETICA_10
 *					GLUT_BITMAP_HELVETICA_12
 *					GLUT_BITMAP_HELVETICA_18
 *
 *	\param	text	-	the text string to output
 *	\param	x		-	the x co-ordinate
 *	\param	y		-	the y co-ordinate
 */
void Font(void *font,char *text,int x,int y)
{
	glRasterPos2i(x, y);

	while( *text != '\0' )
	{
		glutBitmapCharacter( font, *text );
		++text;
	}
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is used to see if a mouse click or event is within a button
 *			client area.
 *	\param	b	-	a pointer to the button to test
 *	\param	x	-	the x coord to test
 *	\param	y	-	the y-coord to test
 */
int ButtonClickTest(Button* b,int x,int y)
{
	if( b)
	{
		/*
		 *	If clicked within button area, then return true
		 */
	    if( x > b->x      &&
			x < b->x+b->w &&
			y > b->y      &&
			y < b->y+b->h ) {
				return 1;
		}
	}

	/*
	 *	otherwise false.
	 */
	return 0;
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonRelease(int x,int y)
{
	Button* b = pButtonList;
	while(b)
	{
		/*
		 *	If the mouse button was pressed within the button area
		 *	as well as being released on the button.....
		 */
		if( ButtonClickTest(b,TheMouse.xpress,TheMouse.ypress) &&
			ButtonClickTest(b,x,y) )
		{
			/*
			 *	Then if a callback function has been set, call it.
			 */
			if (b->callbackFunction) {
				b->callbackFunction();
			}
		}

		/*
		 *	Set state back to zero.
		 */
		b->state = 0;

		b=b->next;
	}
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonPress(int x,int y)
{
	Button* b = pButtonList;
	while(b)
	{
		/*
		 *	if the mouse click was within the buttons client area,
		 *	set the state to true.
		 */
		if( ButtonClickTest(b,x,y) )
		{
			b->state = 1;
		}
		b=b->next;
	}
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonPassive(int x,int y)
{
	int needRedraw = 0;
	Button* b = pButtonList;
	while(b)
	{
		/*
		 *	if the mouse moved over the control
		 */
		if( ButtonClickTest(b,x,y) )
		{
			/*
			 *	If the cursor has just arrived over the control, set the highlighted flag
			 *	and force a redraw. The screen will not be redrawn again until the mouse
			 *	is no longer over this control
			 */
			if( b->highlighted == 0 ) {
				b->highlighted = 1;
				needRedraw=1;
			}
		}
		else

		/*
		 *	If the cursor is no longer over the control, then if the control
		 *	is highlighted (ie, the mouse has JUST moved off the control) then
		 *	we set the highlighting back to false, and force a redraw.
		 */
		if( b->highlighted == 1 )
		{
			b->highlighted = 0;
			needRedraw=1;
		}
		b=b->next;
	}
	if (needRedraw) {
		glutPostRedisplay();
	}
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 */
void ButtonDraw()
{
	int fontx;
	int fonty;

	Button* b = pButtonList;
	while(b)
	{
		/*
		 *	We will indicate that the mouse cursor is over the button by changing its
		 *	colour.
		 */
		if (b->highlighted)
			glColor3f(0.7f,0.7f,0.8f);
		else
			glColor3f(0.6f,0.6f,0.6f);

		/*
		 *	draw background for the button.
		 */
		glBegin(GL_QUADS);
			glVertex2i( b->x     , b->y      );
			glVertex2i( b->x     , b->y+b->h );
			glVertex2i( b->x+b->w, b->y+b->h );
			glVertex2i( b->x+b->w, b->y      );
		glEnd();

		/*
		 *	Draw an outline around the button with width 3
		 */
		glLineWidth(3);

		/*
		 *	The colours for the outline are reversed when the button.
		 */
		if (b->state)
			glColor3f(0.4f,0.4f,0.4f);
		else
			glColor3f(0.8f,0.8f,0.8f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x+b->w, b->y      );
			glVertex2i( b->x     , b->y      );
			glVertex2i( b->x     , b->y+b->h );
		glEnd();

		if (b->state)
			glColor3f(0.8f,0.8f,0.8f);
		else
			glColor3f(0.4f,0.4f,0.4f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x     , b->y+b->h );
			glVertex2i( b->x+b->w, b->y+b->h );
			glVertex2i( b->x+b->w, b->y      );
		glEnd();

		glLineWidth(1);


		/*
		 *	Calculate the x and y coords for the text string in order to center it.
		 */
		fontx = b->x + (b->w - 20) / 2 ;///cuaxk
		fonty = b->y + (b->h+10)/2;

		/*
		 *	if the button is pressed, make it look as though the string has been pushed
		 *	down. It's just a visual thing to help with the overall look....
		 */
		if (b->state) {
			fontx+=2;
			fonty+=2;
		}

		/*
		 *	If the cursor is currently over the button we offset the text string and draw a shadow
		 */
		if(b->highlighted)
		{
			glColor3f(0,0,0);
			Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);
			fontx--;
			fonty--;
		}

		glColor3f(1,1,1);
		Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);

		b = b->next;
	}
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is called to initialise opengl.
 */
 #endif
