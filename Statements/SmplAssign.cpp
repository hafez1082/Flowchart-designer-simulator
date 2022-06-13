#include "SmplAssign.h"
#include <sstream>
#include "..\ApplicationManager.h"
using namespace std;

SmplAssign::SmplAssign(Point Lcorner, string LeftHS, double RightHS)
{
	StatType="SMPL";

	LHS = LeftHS;
	RHS = RightHS;

	UpdateStatementText();

	height=UI.ASSGN_HI;
	width=UI.ASSGN_WDTH;
	LeftCorner = Lcorner;

	pConn = NULL;	//No connectors yet

	Inlet.x = LeftCorner.x + width /2;
	Inlet.y = LeftCorner.y;

	Outlet.x = Inlet.x;
	Outlet.y = LeftCorner.y + height;	
}

void SmplAssign::setLHS(const string &L)
{
	LHS = L;

	UpdateStatementText();
}

void SmplAssign::setRHS(double R)
{
	RHS = R;
	UpdateStatementText();
}


void SmplAssign::setStatConnector(Connector *Conn,int ConnType)
{
	pConn=Conn;
}

Connector* SmplAssign::getStatConnector(int ConnType)
{
	return pConn;
}



void SmplAssign::Draw(Output* pOut) const
{
	//Call Output::DrawAssign function to draw assignment statement 	
	pOut->DrawAssign(LeftCorner, width, height, Text, Selected);

}


//This function should be called when LHS or RHS changes
void SmplAssign::UpdateStatementText()
{
	if(LHS=="")	//No left handside ==>statement have no valid text yet
		Text = "    = ";
	else
	{
		//Build the statement text: Left handside then equals then right handside
		ostringstream T;
		T<<LHS<<" = "<<RHS;	
		Text = T.str();	 
	}
}

void SmplAssign::Resize(char R) 
{
	if(R=='+')
	{if(width>=1.5*UI.ASSGN_WDTH)
	return;
	LeftCorner.x=LeftCorner.x-(0.25*width);
	LeftCorner.y=LeftCorner.y-(0.25*height);
	height*=1.5;
	width*=1.5;


	}
	else if(R=='-') {
		if(width<=(2.0/3)*UI.ASSGN_WDTH)
			return;
		LeftCorner.x=LeftCorner.x+((1.0/6)*width);
		LeftCorner.y=LeftCorner.y+((1.0/6)*height);
		height/=1.5;
		width/=1.5;
	}



	Inlet.x = LeftCorner.x + width /2;
	Inlet.y = LeftCorner.y;

	Outlet.x = Inlet.x;
	Outlet.y = LeftCorner.y + height;


	if(pConn!=NULL)
		pConn->setStartPoint(Outlet);


}

void SmplAssign::Edit(Output *pOut,Input *pIn)
{
	string S;

	while(1)
	{   pOut->PrintMessage("enter the < LeftHand Side > ");
	S=pIn->GetString(pOut,LHS);
	if(CheckVar(S))
	{setLHS(S);	break;}}
	pOut->PrintMessage("enter the < RightHand Side > ");

	setRHS(pIn->GetValue(pOut,RHS));

	pOut->ClearStatusBar();

	Var->name=LHS;
	Var->value=RHS;
}

Point SmplAssign::GetConnPoint(int Order,int connType)
{
	if(Order==1)
		return Outlet;
	else return Inlet;

}

bool SmplAssign::IsOnStat(Point p)
{
	if(p.x>=LeftCorner.x&&p.x<=(LeftCorner.x+width))
		if(p.y>=LeftCorner.y&&p.y<=(LeftCorner.y+height))
			return true;

	return false;

}

Statement* SmplAssign::Copy(Point P)
{
	Point New;
	New.x=P.x-width/2;
	New.y=P.y;
	SmplAssign *pSmpl=new SmplAssign(New,LHS,RHS);
	return pSmpl;

}

//Ahmed Hafez
//Move Simpl Assign stat.
void SmplAssign::Move(Output *pOut,ApplicationManager *pManager)
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
		pOut->PrintMessage("Move Simple Assign statement anywhere in the drawing area ,press ESC to exit"); 

		// Dragging
		if(Drag == false) 
		{
			if(pWind->GetButtonState(LEFT_BUTTON, P.x, P.y) == BUTTON_DOWN) {
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
					if(LeftCorner.x + (P.x - XOld)>UI.MnItWdth&&LeftCorner.x + (P.x - XOld)+width<UI.width)
					{
						LeftCorner.x = LeftCorner.x + (P.x - XOld);
						XOld = P.x;
					}
				}
				if(P.y != YOld) 
				{
					if(LeftCorner.y + (P.y - YOld)>UI.TlBrWdth&&LeftCorner.y + (P.y - YOld)+height<UI.height-UI.StBrWdth)
					{
						LeftCorner.y = LeftCorner.y + (P.y - YOld);
						YOld = P.y;
					}
				}
			}
		}

		// Draw SmplAssign
		Inlet.x = LeftCorner.x + width /2;
		Inlet.y = LeftCorner.y;

		Outlet.x = Inlet.x;
		Outlet.y = LeftCorner.y + height;	

		if(pConn!=NULL) //Check connector
			pConn->setStartPoint(Outlet); //Set start point
		pManager->EditConnectors(this); //Edit Connector
		pManager->UpdateInterface(); //Update
		pOut->CreateDesignToolBar(); //Create design tool bar

		// Update the screen buffer
		pWind->UpdateBuffer();
	}
	pWind->SetBuffering(false);
}


void SmplAssign::Save(ofstream &OutFile)
{

	OutFile<<StatType<<"\t"<<ID<<"\t"<<LeftCorner.x<<"\t"<<LeftCorner.y<<"\t"<<LHS<<"\t"<<RHS<<"\t“"<<Comment<<"”"<<endl;


}

void SmplAssign::Load(ifstream &Infile)
{

	string a,line,l;
	if (getline( Infile, line ))
	{
		istringstream liness( line );
		getline( liness, l, '\t' );
		getline( liness, a, '\t' );	
		istringstream A(a);
		A >> ID;
		getline( liness, a, '\t' );
		istringstream B(a);
		B >> LeftCorner.x;
		getline( liness, a, '\t' );
		istringstream C(a);
		C >> LeftCorner.y;
		getline( liness, a, '\t' );
		istringstream D(a);
		D >> LHS;
		getline( liness, a, '\t' );
		istringstream E(a);
		E >> RHS;
		Comment=line.substr(line.find('“')+1);
		Comment.resize(Comment.size() -1 );
	}

	Var->name=LHS;
	Inlet.x = LeftCorner.x + width /2;
	Inlet.y = LeftCorner.y;

	Outlet.x = Inlet.x;
	Outlet.y = LeftCorner.y + height;	


	UpdateStatementText();

}

void SmplAssign::GenerateCode(ofstream &OutFile)
{if(LHS!="")
if(Comment!="")
	OutFile<<LHS<<" = "<<RHS<<";	//"<<Comment<<endl;
else OutFile<<LHS<<" = "<<RHS<<";\n";

}

void SmplAssign::Simulate(Output *pOut,Input *pIn)
{
	if(Var->name!="")
		Var->value=RHS;
}