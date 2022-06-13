#ifndef INPUT_H
#define INPUT_H

#include "..\DEFS.h"
#include "UI_Info.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"

class Output;
class Input		//The application manager should have a pointer to this class
{
private:
	window *pWind;	//Pointer to the Graphics Window
public:
	Input(window *pW);		//Consturctor
	void GetPointClicked(Point &P) const;//Get coordinate where user clicks

	double GetValue(Output* pO,double s=0) const;	// Reads a double value from the user 

	string GetString(Output* pO,string s="") const ; //Returns a string entered by the user

	ActionType GetUserAction(Point &P) const; //Read the user click and map it to an action
	image StoreImage(image img, Point p1, Point p2) const;
	~Input();
};

#endif