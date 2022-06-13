#include "Output.h"


Output::Output()
{
	//Initialize user interface parameters
	UI.width = 1280;
	UI.height = 700;
	UI.wx = 15;
	UI.wy =15;

	UI.AppMode = DESIGN;	//Design Mode is the default mode

	UI.StBrWdth = 50;
	UI.TlBrWdth = 50;
	UI.MnItWdth = 80;

	UI.DrawClr = BLUE;
	UI.HiClr = BLACK;
	UI.MsgClr = BLACK;

	UI.ASSGN_WDTH = 150;
	UI.ASSGN_HI = 50;

	//Create the output window
	pWind = CreateWind(UI.width, UI.height, UI.wx, UI.wy);
	//Change the title
	pWind->ChangeTitle("Programming Techniques Project");

	pWind->SetPen(BLACK,3);
	CreateDesignToolBar();
	CreateStatusBar();
}


Input* Output::CreateInput()
{
	Input* pIn = new Input(pWind);
	return pIn;
}

//======================================================================================//
//								Interface Functions										//
//======================================================================================//

window* Output::CreateWind(int wd, int h, int x, int y)
{
	return new window(wd, h, x, y);
}
//////////////////////////////////////////////////////////////////////////////////////////
void Output::CreateStatusBar()
{
	pWind->DrawLine(0, UI.height-UI.StBrWdth, UI.width, UI.height-UI.StBrWdth);

}
//////////////////////////////////////////////////////////////////////////////////////////
void Output::clearToolBar()
{
	pWind->SetPen(WHITE,5);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, 0, UI.width, UI.TlBrWdth);
	pWind->DrawRectangle(0, UI.TlBrWdth, UI.MnItWdth, UI.height-UI.StBrWdth);
	pWind->DrawLine(UI.MnItWdth, UI.TlBrWdth, UI.MnItWdth, UI.height-UI.StBrWdth);
	pWind->SetPen(BLACK,3);
	pWind->DrawLine(0, UI.height-UI.StBrWdth, UI.width, UI.height-UI.StBrWdth);
}
//////////////////////////////////////////////////////////////////////////////////////////
void Output::CreateDesignToolBar()
{
	UI.AppMode = DESIGN;	//Design Mode
	clearToolBar();
	int i=0;	
	//fill the horizontal tool bar 
	//You can draw the tool bar icons in any way you want.
	//This can be better done by storing the images name in an array then draw the toolbar using a "for" loop
	pWind->DrawImage("images\\Sim Mode.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Comment.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Copy.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Cut.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Paste.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Move.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Delete.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Resize.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Zoom Out.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Zoom In.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Redo.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Undo.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Load Design.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Save.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\New.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Exit.jpg", i++ * UI.MnItWdth, 0);

	//Draw a line under the toolbar
	pWind->DrawLine(0, UI.TlBrWdth, UI.width, UI.TlBrWdth);	

	i=1;	
	//fill the vertical tool bar 
	//You can draw the tool bar icons in any way you want.
	//This can be better done by storing the images name in an array then draw the toolbar using a "for" loop
	pWind->DrawImage("images\\Start.jpg",0, i++ * UI.TlBrWdth);
	pWind->DrawImage("images\\End.jpg",0, i++ * UI.TlBrWdth);
	pWind->DrawImage("images\\Assign Simple.JPG",0, i++ * UI.TlBrWdth);
	pWind->DrawImage("images\\Assign Var.jpg",0, i++ * UI.TlBrWdth);
	pWind->DrawImage("images\\Assign Operator.jpg",0, i++ * UI.TlBrWdth);
	pWind->DrawImage("images\\Condition.jpg",0, i++ * UI.TlBrWdth);
	pWind->DrawImage("images\\Read.jpg",0, i++ * UI.TlBrWdth);
	pWind->DrawImage("images\\Write.jpg",0, i++ * UI.TlBrWdth);
	pWind->DrawImage("images\\Edit Statement.jpg",0, i++ * UI.TlBrWdth);
	pWind->DrawImage("images\\Connector.jpg",0, i++ * UI.TlBrWdth);
	pWind->DrawImage("images\\Edit Connector.jpg",0, i++ * UI.TlBrWdth);


	//Draw a line beside the toolbar
	pWind->DrawLine(UI.MnItWdth, UI.TlBrWdth, UI.MnItWdth, UI.height-UI.StBrWdth);


}


void Output::CreateSimulationToolBar()
{
	UI.AppMode = SIMULATION;	//Simulation Mode
	clearToolBar();
	int i=0;
	pWind->DrawImage("images\\Design Mode.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Run.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Step.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Generate.jpg", i++ * UI.MnItWdth, 0);
	pWind->DrawImage("images\\Exit.jpg", i++ * UI.MnItWdth, 0);

	//Draw a line under the toolbar
	pWind->DrawLine(0, UI.TlBrWdth, UI.width, UI.TlBrWdth);

	///TODO: add code to create the simulation tool bar
}
//////////////////////////////////////////////////////////////////////////////////////////
void Output::ClearStatusBar()
{
	//Clear Status bar by drawing a filled white rectangle
	pWind->SetPen(BLACK, 1);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, UI.height - UI.StBrWdth, UI.width, UI.height);
}
//////////////////////////////////////////////////////////////////////////////////////////
void Output::ClearDrawArea()
{
	pWind->SetPen(WHITE, 1);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(UI.MnItWdth, UI.TlBrWdth, UI.width, UI.height - UI.StBrWdth);

}
//////////////////////////////////////////////////////////////////////////////////////////
void Output::PrintMessage(string msg)	//Prints a message on status bar
{
	ClearStatusBar();	//First clear the status bar

	pWind->SetPen(UI.MsgClr, 50);
	pWind->SetFont(20, BOLD , BY_NAME, "Arial");   
	pWind->DrawString(10, UI.height - UI.StBrWdth/1.5, msg);
}

//======================================================================================//
//								Statements Drawing Functions								//
//======================================================================================//

//Draw assignment statement and write the "Text" on it
void Output::DrawAssign(Point Left, int width, int height, string Text, bool Selected)
{
	if(Selected)	//if stat is selected, it should be highlighted
		pWind->SetPen(UI.HiClr,3);	//use highlighting color
	else
		pWind->SetPen(UI.DrawClr,3);	//use normal color

	//Draw the statement block rectangle
	pWind->DrawRectangle(Left.x, Left.y, Left.x + width, Left.y + height);

	//Write statement text
	pWind->SetPen(BLACK, 2);
	pWind->DrawString(Left.x+5, Left.y + height/4, Text);

}


////////////////////////////////////////////////////////////////////////////////////////////
//Draw start or end statement 
void Output::DrawStart_End(Point Left,int width,int height,string type,bool Selected)
{
	if(Selected)	//if stat is selected, it should be highlighted
		pWind->SetPen(UI.HiClr,3);	//use highlighting color
	else
		pWind->SetPen(UI.DrawClr,3);	//use normal color

	pWind->DrawEllipse(Left.x,Left.y,Left.x+width,Left.y+height);

	pWind->SetPen(BLACK, 2);
	pWind->DrawString(Left.x+width/2-21, Left.y+height/2-10, type);

}


////////////////////////////////////////////////////////////////////////////////////////////
//Draw read or write statement
void Output::DrawRead_Write(Point Left,int width, int height,string type, string Text, bool Selected)
{if(Selected)	//if stat is selected, it should be highlighted
pWind->SetPen(UI.HiClr,3);	//use highlighting color
else
	pWind->SetPen(UI.DrawClr,3);	//use normal color
int x[4],y[4];
x[0]=Left.x;     x[1]=x[0]+width;          x[2]=x[1]-50;           x[3]=x[0]-50;
y[0]=Left.y;                      y[1]=y[0];                                   y[2]=y[0]+height;      y[3]=y[2];
pWind->DrawPolygon(x,y,4);
pWind->SetPen(BLACK, 2);
if(type=="READ")
	pWind->DrawString(x[0], y[0]+15, "Read "+Text);
else if(type=="WRITE")
	pWind->DrawString(x[0], y[0]+15, "Write "+Text);

}


////////////////////////////////////////////////////////////////////////////////////////////
//Draw condition statement
void Output::DrawCondition(Point Top,int width, int height, string Text, bool Selected)
{if(Selected)	//if stat is selected, it should be highlighted
pWind->SetPen(UI.HiClr,3);	//use highlighting color
else
	pWind->SetPen(UI.DrawClr,3);	//use normal color

int x[4],y[4];
x[0]=Top.x;            x[1]=Top.x-width/2;             x[2]=x[0];               x[3]=Top.x+width/2;
y[0]=Top.y;     y[1]=Top.y+height/2;                                       y[2]=y[1]+height/2;        y[3]=y[1];
pWind->DrawPolygon(x,y,4);
pWind->SetPen(BLACK, 2);
pWind->DrawString(x[1]+50, y[1]-5, Text);
pWind->DrawString(x[1]-10, y[1]-20, "F");
pWind->DrawString(x[3]+5, y[3]-20, "T");



}


//Draw connector
//////////////////////////////////////////////////////////////////////////////////////////
void Output::DrawConnector(Point start,Point end,bool Selected)
{if(Selected)	//if stat is selected, it should be highlighted
pWind->SetPen(UI.HiClr,3);	//use highlighting color
else
	pWind->SetPen(UI.DrawClr,3);	//use normal color

if(end.y>start.y)
{
	pWind->DrawLine(start.x,start.y,start.x,start.y+10);
	pWind->DrawLine(start.x,start.y+10,end.x,start.y+10);
	pWind->DrawLine(end.x,start.y+10,end.x,end.y);
	pWind->DrawLine(end.x,end.y,end.x+10,end.y-10);
	pWind->DrawLine(end.x,end.y,end.x-10,end.y-10);
}
else if(end.y<start.y)
{
	if(end.x>start.x)
	{
		pWind->DrawLine(start.x,start.y,start.x,start.y+10);
		pWind->DrawLine(start.x,start.y+10,start.x-150,start.y+10);
		pWind->DrawLine(start.x-150,start.y+10,start.x-150,end.y);
		pWind->DrawLine(start.x-150,end.y,end.x,end.y);
		pWind->DrawLine(end.x,end.y,end.x-10,end.y+10);
		pWind->DrawLine(end.x,end.y,end.x-10,end.y-10);
	}
	else
	{
		pWind->DrawLine(start.x,start.y,start.x,start.y+10);
		pWind->DrawLine(start.x,start.y+10,start.x+150,start.y+10);
		pWind->DrawLine(start.x+150,start.y+10,start.x+150,end.y);
		pWind->DrawLine(start.x+150,end.y,end.x,end.y);
		pWind->DrawLine(end.x,end.y,end.x+10,end.y+10);
		pWind->DrawLine(end.x,end.y,end.x+10,end.y-10);
	}
}
else
{
	if(end.x>start.x)
	{
		pWind->DrawLine(start.x,start.y,end.x,end.y);
		pWind->DrawLine(end.x,end.y,end.x-10,end.y+10);
		pWind->DrawLine(end.x,end.y,end.x-10,end.y-10);
	}
	else
	{
		pWind->DrawLine(start.x,start.y,end.x,end.y);
		pWind->DrawLine(end.x,end.y,end.x+10,end.y+10);
		pWind->DrawLine(end.x,end.y,end.x+10,end.y-10);
	}
}
}

void Output::DrawExitMsg()
{

	pWind->DrawImage("images\\ExitMsg.jpg", UI.width/2-150, UI.height/2-75);

}


void Output::DrawCondMsg()
{

	pWind->DrawImage("images\\CondMsg.jpg", UI.width/2-150, UI.height/2-75);

}


window* Output::getwindow()
{
	return pWind;
}

///////////////////////////////////////////////////////////////////////////////////////////
Output::~Output()
{
	delete pWind;
}


void Output::DrawImage(image *img,Point p1, Point p2)
{
	pWind->DrawImage(img,p1.x,p1.y,p2.x,p2.y);
}