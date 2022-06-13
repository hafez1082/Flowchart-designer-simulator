#include "End.h"
#include <sstream>
#include "..\ApplicationManager.h"
using namespace std;

End::End(Point center)
{
	Center=center;
	StatType="END";

	height=UI.ASSGN_HI;
	width=UI.ASSGN_WDTH;

	Inlet.x = Center.x;
	Inlet.y = Center.y-(UI.ASSGN_HI/2);

}


void End::Draw(Output* pOut) const
{
	//Call Output::DrawStart_End function to draw end
	Point P;
	P.x=Center.x-(width/2);
	P.y=Center.y-(height/2);
	pOut->DrawStart_End(P,width,height,"END",Selected);

}



void End::setStatConnector(Connector *Conn,int ConnType)
{
	return;
}

Connector* End::getStatConnector(int ConnType)
{
	return NULL;
}

//Ahmed Hafez
//Move End stat.
void End::Move(Output *pOut,ApplicationManager *pManager)
{
	window* pWind=pOut->getwindow();

	pWind->SetBuffering(true); 
	bool Drag = false;

	//current point
	Point P;

	//Old positions
	int XOld = 0;
	int YOld = 0;
	char cKeyData;

	// While loop until ESC key is pressed to exit
	while(pWind->GetKeyPress(cKeyData) != ESCAPE)
	{
		pOut->PrintMessage("Move End statement anywhere in the drawing area ,press ESC to exit"); 

		// Dragging
		if(Drag == false) 
		{
			if(pWind->GetButtonState(LEFT_BUTTON, P.x, P.y) == BUTTON_DOWN) 
			{
				if(IsOnStat(P))
				{
					Drag = true; 
					XOld = P.x; 
					YOld = P.y;
				}
			}
		} 
		else 
		{
			if(pWind->GetButtonState(LEFT_BUTTON, P.x, P.y) == BUTTON_UP) 
			{
				Drag = false; 
			}
			else 
			{
				if(P.x != XOld) 
				{
					if(Center.x + (P.x - XOld)-UI.ASSGN_WDTH/2>UI.MnItWdth&&Center.x + (P.x - XOld)+UI.ASSGN_WDTH/2<UI.width)
					{
						Center.x = Center.x + (P.x - XOld);
						XOld = P.x;
					}
				}
				if(P.y != YOld) 
				{
					if(Center.y + (P.y - YOld)-UI.ASSGN_HI/2>UI.TlBrWdth&&Center.y + (P.y - YOld)+UI.ASSGN_HI/2<UI.height-UI.StBrWdth)
					{
						Center.y = Center.y + (P.y - YOld);
						YOld = P.y;
					}
				}
			}   
		}

		// Draw End	
		Inlet.x = Center.x;
		Inlet.y = Center.y-(UI.ASSGN_HI/2);

		pManager->EditConnectors(this);
		pManager->UpdateInterface(); //To update
		pOut->CreateDesignToolBar(); //Create design tool bar

		// Update the screen buffer
		pWind->UpdateBuffer();
	}
	pWind->SetBuffering(false);
}


void End::Resize(char R) 
{
	if(R=='+')
	{
		if(width>=1.5*UI.ASSGN_WDTH)
			return;
		height*=1.5;
		width*=1.5;


	}
	else if(R=='-')
	{
		if(width<=(2.0/3)*UI.ASSGN_WDTH)
			return;
		height/=1.5;
		width/=1.5;
	}



	Inlet.x = Center.x ;
	Inlet.y = Center.y - (height/2);	

}
bool End::IsOnStat(Point P)
{
	if(((float)((P.x-Center.x)*(P.x-Center.x))/((UI.ASSGN_WDTH/2)*(UI.ASSGN_WDTH/2))+((float)(P.y-Center.y)*(P.y-Center.y))/((UI.ASSGN_HI/2)*(UI.ASSGN_HI/2)))<=1)
		return true;
	else return false;
}
Point End::GetConnPoint(int Order,int connType)
{
	return Inlet;
}

void End::Edit(Output *pOut,Input *pIn)
{

	pOut->PrintMessage("The End Statement can't be edited");
}

void End::GenerateCode(ofstream &OutFile)
{if(Comment!="")

OutFile<<"return 0;\t// "<<Comment<<endl;
else

	OutFile<<"return 0;"<<endl;
OutFile<<"}"<<endl;
}

void End::Save(ofstream &OutFile)
{
	OutFile<<StatType<<"\t"<<ID<<"\t"<<Center.x<<"\t"<<Center.y<<"\t“"<<Comment<<"”"<<endl;
}

void End::Load(ifstream &Infile)
{
	Infile>>ID>>Center.x>>Center.y;
	getline(Infile,Comment);
	Comment=Comment.substr(Comment.find('“')+1);
	Comment.resize(Comment.size() -1 );
	Inlet.x = Center.x ;
	Inlet.y = Center.y - (UI.ASSGN_HI/2);
}

