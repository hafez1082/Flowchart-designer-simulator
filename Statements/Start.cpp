#include "Start.h"
#include <sstream>
#include "..\ApplicationManager.h"
using namespace std;

Start::Start(Point center)
{

	Center=center;
	StatType="STRT";

	pConn = NULL;	//No connectors yet
	height=UI.ASSGN_HI;
	width=UI.ASSGN_WDTH;

	Outlet.x = Center.x ;
	Outlet.y = Center.y + (UI.ASSGN_HI/2);	
}


void Start::setStatConnector(Connector *Conn,int ConnType)
{
	pConn=Conn;
}

Connector* Start::getStatConnector(int ConnType)
{
	return pConn;
}


void Start::Draw(Output* pOut) const
{
	//Call Output::DrawStart_End function to draw start 	
	Point P;
	P.x=Center.x-(width/2);
	P.y=Center.y-(height/2);
	pOut->DrawStart_End(P,width,height,"START",Selected);

}

//Ahmed Hafez
//Move Start stat.
void Start::Move(Output *pOut,ApplicationManager *pManager)
{
	window* pWind=pOut->getwindow();

	pWind->SetBuffering(true);    
	bool Drag = false;

	//Declaring a point
	Point P;

	//Old positions
	int XOld = 0;
	int YOld = 0;
	char cKeyData;

	// While loop until ESC key is pressed to exit
	while(pWind->GetKeyPress(cKeyData) != ESCAPE)
	{
		pOut->PrintMessage("Move Start statement anywhere in the drawing area ,press ESC to exit"); 

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

		// Draw start
		Outlet.x = Center.x ;
		Outlet.y = Center.y + (UI.ASSGN_HI/2);

		if(pConn!=NULL) //Check connector
			pConn->setStartPoint(Outlet); //set start point
		pManager->EditConnectors(this); //Edit
		pManager->UpdateInterface(); //Update
		pOut->CreateDesignToolBar(); //Create desgin tool bar

		// Update the screen buffer
		pWind->UpdateBuffer();

	}
	pWind->SetBuffering(false);
}


void Start::Resize(char R) 
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



	Outlet.x = Center.x ;
	Outlet.y = Center.y + (height/2);	

	if(pConn!=NULL)
		pConn->setStartPoint(Outlet);


}



bool Start::IsOnStat(Point P)
{
	if(((float)((P.x-Center.x)*(P.x-Center.x))/((UI.ASSGN_WDTH/2)*(UI.ASSGN_WDTH/2))+((float)(P.y-Center.y)*(P.y-Center.y))/((UI.ASSGN_HI/2)*(UI.ASSGN_HI/2)))<=1)
		return true;
	else return false;
}
Point Start::GetConnPoint(int Order,int connType)
{
	return Outlet;
}

void Start::Edit(Output *pOut,Input *pIn)
{

	pOut->PrintMessage("The Start Statement can't be edited");
}

void Start::GenerateCode(ofstream &OutFile)
{
	if(Comment!="")
		OutFile<<"#include <iostream>\t// "<<Comment<<endl;
	else 
		OutFile<<"#include <iostream>"<<endl;

	OutFile<<"using namespace std;"<<endl;
	OutFile<<"int main()"<<endl;
	OutFile<<"{"<<endl;
}

void Start::Save(ofstream &OutFile)
{
	OutFile<<StatType<<"\t"<<ID<<"\t"<<Center.x<<"\t"<<Center.y<<"\t“"<<Comment<<"”"<<endl;
}

void Start::Load(ifstream &Infile)
{
	Infile>>ID>>Center.x>>Center.y;
	getline(Infile,Comment);
	Comment=Comment.substr(Comment.find('“')+1);
	Comment.resize(Comment.size() -1 );
	Outlet.x = Center.x ;
	Outlet.y = Center.y + (UI.ASSGN_HI/2);
}