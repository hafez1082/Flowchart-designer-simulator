#include "Input.h"
#include "Output.h"
#include <cstdlib>
#include <sstream>

Input::Input(window* pW)
{
	pWind = pW; //point to the passed window
}

void Input::GetPointClicked(Point &P) const
{

	pWind->WaitMouseClick(P.x, P.y);	//Wait for mouse click
}


string Input::GetString(Output *pO,string s) const 
{

	string Str=s;
	if (Str!="")
		pO->PrintMessage(Str);
	char Key;
	while(1)
	{
		pWind->WaitKeyPress(Key);
		if(Key == 27 )	//ESCAPE key is pressed
			return "";	//returns nothing as user has cancelled label
		if(Key == 13 )	//ENTER key is pressed
			return Str;
		if(Key == 8 )	//BackSpace is pressed
		{if(Str.length()!=0) 
		Str.resize(Str.size() -1 );}			
		else
			Str += Key;
		pO->PrintMessage(Str);
	}

}


double Input::GetValue(Output* pO,double s) const	// Reads a double value from the user 
{
	///TODO: add code to read a double value from the user and assign it to D
	double D=s;
	string Str;
	bool exit=false;
	int sign=1;
	while(!exit){

		Str=GetString(pO,to_string(long double(D)));

		int i = 0;	//First element in the string

		if (Str[0] == '-'){		// If the input number is negative (first char = '-')

			sign = -1;	//Set sign to negative

			for (i=1; i < Str.length() ; i++){	//Shift string backwards and delete '-'
				Str[i-1] = Str [i];
			}

			Str.resize(Str.size() -1 );
		}





		int count=0;
		for(i=0;i<Str.length();i++)
			if(Str[i]=='.')
				count++;
		if(count>1)
		{	pO->PrintMessage("Invalid value ,please enter numbers only ,press any key to retry ");
		char x;        
		pWind->WaitKeyPress(x);    
		exit=false;
		pO->PrintMessage("Please enter a value");
		continue;
		}


		for(i=0;i<Str.length();i++)
		{if(((int)Str[i]<48||(int)Str[i]>57)&&Str[i]!='.')
		{	pO->PrintMessage("Invalid value ,please enter numbers only ,press any key to retry ");
		char x;        
		pWind->WaitKeyPress(x);    
		exit=false;   
		pO->PrintMessage("Please enter a value");
		break;
		}
		exit=true;
		}}
	D=stod(Str);

	//Read a double value from the user

	return D*sign;
}


ActionType Input::GetUserAction(Point &P) const
{	
	//This function reads the position where the user clicks to determine the desired action

	int x,y;
	pWind->WaitMouseClick(x, y);	//Get the coordinates of the user click
	P.x=x;
	P.y=y;
	if(UI.AppMode == DESIGN )	//application is in design mode
	{
		//[1] If user clicks on the horizontal Toolbar
		if ( y >= 0 && y < UI.TlBrWdth)
		{	//Check whick Menu item was clicked
			if( x >= 0 && x <  UI.MnItWdth  ) 
				return SIM_MODE;
			if( x >= UI.MnItWdth && x < 2* UI.MnItWdth  ) 
				return COMMENT;
			if( x >= 2*UI.MnItWdth && x < 3* UI.MnItWdth  ) 
				return COPY;
			if( x >= 3*UI.MnItWdth && x < 4* UI.MnItWdth  ) 
				return CUT;
			if( x >= 4*UI.MnItWdth && x < 5* UI.MnItWdth  ) 
				return PASTE;
			if( x >= 5*UI.MnItWdth && x < 6* UI.MnItWdth  ) 
				return MOVE;
			if( x >= 6*UI.MnItWdth && x < 7* UI.MnItWdth  ) 
				return DEL;
			if( x >= 7*UI.MnItWdth && x < 8* UI.MnItWdth  ) 
				return RESIZE;
			if( x >= 8*UI.MnItWdth && x < 9* UI.MnItWdth  ) 
				return ZOOMOUT;
			if( x >= 9*UI.MnItWdth && x < 10* UI.MnItWdth  ) 
				return ZOOMIN;
			if( x >= 10*UI.MnItWdth && x < 11* UI.MnItWdth  ) 
				return REDO;
			if( x >= 11*UI.MnItWdth && x < 12* UI.MnItWdth  ) 
				return UNDO;
			if( x >= 12*UI.MnItWdth && x < 13* UI.MnItWdth  ) 
				return LOAD;
			if( x >= 13*UI.MnItWdth && x < 14* UI.MnItWdth  ) 
				return SAVE;
			if( x >= 14*UI.MnItWdth && x < 15* UI.MnItWdth  )
				return NEW;
			if( x >= 15*UI.MnItWdth && x < 16* UI.MnItWdth  )
				return EXIT;

			return DSN_TOOL;	//a click on empty part of the tool bar

		}
		//[2] If user clicks on the vertical Toolbar
		if ( y >= UI.TlBrWdth && y < UI.height - UI.StBrWdth && x < UI.MnItWdth)
		{
			if( y >= UI.TlBrWdth && y < 2*UI.TlBrWdth  )
				return ADD_START;
			if( y >= 2*UI.TlBrWdth && y < 3*UI.TlBrWdth  )		
				return ADD_END;
			if( y >= 3*UI.TlBrWdth && y < 4*UI.TlBrWdth  )		
				return ADD_SMPL_ASSIGN;
			if( y >= 4*UI.TlBrWdth && y < 5*UI.TlBrWdth  )		
				return ADD_VAR_ASSIGN;
			if( y >= 5*UI.TlBrWdth && y < 6*UI.TlBrWdth  )
				return ADD_SINGLE_OPERATOR_ASSIGN;
			if( y >= 6*UI.TlBrWdth && y < 7*UI.TlBrWdth  )
				return ADD_CONDITION;
			if( y >= 7*UI.TlBrWdth && y < 8*UI.TlBrWdth  )
				return  ADD_READ_STAT;
			if( y >= 8*UI.TlBrWdth && y < 9*UI.TlBrWdth  )
				return  ADD_WRITE_STAT;
			if( y >= 9*UI.TlBrWdth && y < 10*UI.TlBrWdth  )
				return EDIT_STAT;
			if( y >= 10*UI.TlBrWdth && y < 11*UI.TlBrWdth  )
				return ADD_CONNECTOR;
			if( y >= 11*UI.TlBrWdth && y < 12*UI.TlBrWdth  ) 
				return EDIT_CONNECTOR;

			return DSN_TOOL;	//a click on empty part of the tool bar
		}
		//[3] User clicks on the drawing area
		if ( y >= UI.TlBrWdth && y < UI.height - UI.StBrWdth && x >= UI.MnItWdth)
		{
			return SELECT;	//user want unselect a statement in the flowchart
		}

		//[4] User clicks on the status bar
		return STATUS;
	}
	else	//Application is in Simulation mode
	{   //[1] If user clicks on the Toolbar
		if ( y >= 0 && y < UI.TlBrWdth)
		{
			if( x >= 0 && x < UI.MnItWdth  )
				return DSN_MODE;
			if( x >= UI.MnItWdth && x < 2* UI.MnItWdth  )		
				return RUN;
			if( x >= 2*UI.MnItWdth && x < 3* UI.MnItWdth  )		
				return STEP;
			if( x >= 3*UI.MnItWdth && x < 4* UI.MnItWdth  )		
				return GENERATE;
			if( x >= 4*UI.MnItWdth && x < 5* UI.MnItWdth  )
				return EXIT;



			return SIM_TOOL;
		}

		//[2] User clicks on the status bar
		if(y < UI.height && y >= UI.height-UI.StBrWdth)
			return STATUS;
	}
}

Input::~Input()
{
}


image Input::StoreImage(image img, Point p1, Point p2) const
{
	pWind->StoreImage(img,p1.x,p1.y,p2.x,p2.y);
	pWind->SetPen(WHITE, 1);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(UI.MnItWdth, UI.TlBrWdth, UI.width, UI.height - UI.StBrWdth);
	return img;
}
