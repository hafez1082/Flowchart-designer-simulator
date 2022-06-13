#ifndef OUPTUT_H
#define OUPTUT_H

#include "Input.h"

class Output	//The application manager should have a pointer to this class
{
private:	
	window* pWind;	//Pointer to the Graphics Window
	void clearToolBar(); //is used only in switching between design & simulation mode
public:
	Output();	

	window* CreateWind(int, int, int , int);
	void CreateDesignToolBar();	//Tool bar of the design mode
	void CreateSimulationToolBar();//Tool bar of the simulation mode

	void CreateStatusBar();

	Input* CreateInput(); //creates a pointer to the Input object	
	void ClearStatusBar();	//Clears the status bar
	void ClearDrawArea();	//Clears the drawing area

	// -- Statements Drawing Functions
	void DrawAssign(Point Left, int width, int height, string Text, bool Selected=false);
	void DrawStart_End(Point p,int width,int height,string type,bool Selected=false);
	void DrawRead_Write(Point p,int width, int height,string type, string Text, bool Selected=false);
	void DrawCondition(Point top,int width, int height, string Text, bool Selected=false);
	void DrawConnector(Point start,Point end,bool Selected=false);
	void DrawExitMsg();
	void DrawCondMsg();
	///Make similar functions for drawing all other statements.

	window* getwindow();
	void PrintMessage(string msg);	//Print a message on Status bar
	void DrawImage(image *img, Point p1, Point p2);
	~Output();
};

#endif