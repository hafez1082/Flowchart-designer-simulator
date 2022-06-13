#include "VarAssign.h"
#include <sstream>
#include "..\ApplicationManager.h"
using namespace std;

VarAssign::VarAssign(Point Lcorner, string LeftHS, string RightHS)
{
	Var2=new Variable;
	Var2->name="";
	Var2->value=0;
	Var2->repeat=1;
	LHS = LeftHS;
	RHS = RightHS;
	StatType="VAR";
	UpdateStatementText();

	LeftCorner = Lcorner;
	height=UI.ASSGN_HI;
	width=UI.ASSGN_WDTH;

	pConn = NULL;	//No connectors yet

	Inlet.x = LeftCorner.x + width /2;
	Inlet.y = LeftCorner.y;

	Outlet.x = Inlet.x;
	Outlet.y = LeftCorner.y + height;	
}

void VarAssign::setLHS(const string &L)
{
	LHS = L;
	UpdateStatementText();
}

void VarAssign::setRHS(const string &R)
{
	RHS = R;
	UpdateStatementText();
}


void VarAssign::setStatConnector(Connector *Conn,int ConnType)
{
	pConn=Conn;
}

Connector* VarAssign::getStatConnector(int ConnType)
{
	return pConn;
}



void VarAssign::Draw(Output* pOut) const
{
	//Call Output::DrawAssign function to draw assignment statement 	
	pOut->DrawAssign(LeftCorner, width, height, Text, Selected);

}


//This function should be called when LHS or RHS changes
void VarAssign::UpdateStatementText()
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

void VarAssign::Resize(char R) 
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

void VarAssign::Edit(Output *pOut,Input *pIn)
{
	string S;

	while(1)
	{   pOut->PrintMessage("enter the < LeftHand Side > ");
	S=pIn->GetString(pOut,LHS);
	if(CheckVar(S))
	{setLHS(S);	break;}}


	while(1)
	{   pOut->PrintMessage("enter the < rightHand Side > ");
	S=pIn->GetString(pOut,RHS);
	if(CheckVar(S))
	{setRHS(S);	break;}}

	pOut->ClearStatusBar();
	Var->name=LHS;
	Var2->name=RHS;

}

Point VarAssign::GetConnPoint(int Order,int connType)
{
	if(Order==1)
		return Outlet;
	else return Inlet;

}

bool VarAssign::IsOnStat(Point p)
{
	if(p.x>=LeftCorner.x&&p.x<=(LeftCorner.x+width))
		if(p.y>=LeftCorner.y&&p.y<=(LeftCorner.y+height))
			return true;

	return false;

}

Statement* VarAssign::Copy(Point P)
{	
	Point New;
	New.x=P.x-width/2;
	New.y=P.y;
	VarAssign *pAssign = new VarAssign(New,LHS,RHS);
	return pAssign; 

}

//Ahmed Hafez
//Move Variable Assign stat.
void VarAssign::Move(Output *pOut,ApplicationManager *pManager)
{
	window* pWind=pOut->getwindow();

	pWind->SetBuffering(true);   
	bool Drag = false;

	//Declaring a Point
	Point P;

	//Old positions
	int XOld = 0;
	int YOld = 0;
	char cKeyData;

	// While loop until ESC key is pressed to exit
	while(pWind->GetKeyPress(cKeyData) != ESCAPE)
	{
		pOut->PrintMessage("Move Variable Assign statement anywhere in the drawing area ,press ESC to exit"); 

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
			if(pWind->GetButtonState(LEFT_BUTTON, P.x, P.y) == BUTTON_UP) {
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

		// Draw Var Assign
		Inlet.x = LeftCorner.x + width /2;
		Inlet.y = LeftCorner.y;

		Outlet.x = Inlet.x;
		Outlet.y = LeftCorner.y + height;	

		if(pConn!=NULL) //Check connector
			pConn->setStartPoint(Outlet); //set start
		pManager->EditConnectors(this); //Edit
		pManager->UpdateInterface(); //update
		pOut->CreateDesignToolBar(); //Create design tool bar

		// Update the screen buffer
		pWind->UpdateBuffer();

	}
	pWind->SetBuffering(false);
}


void VarAssign::Save(ofstream &OutFile)
{

	OutFile<<StatType<<"\t"<<ID<<"\t"<<LeftCorner.x<<"\t"<<LeftCorner.y<<"\t"<<LHS<<"\t"<<RHS<<"\t“"<<Comment<<"”"<<endl;


}

void VarAssign::Load(ifstream &Infile)
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
	Var2->name=RHS;
	Inlet.x = LeftCorner.x + width /2;
	Inlet.y = LeftCorner.y;

	Outlet.x = Inlet.x;
	Outlet.y = LeftCorner.y + height;	


	UpdateStatementText();

}

void VarAssign::setVar(Variable *V,int i)
{
	if(i==0)
		Var=V;
	else if(i==1)
		Var2=V;
}
Variable *VarAssign::getVar(int i)
{
	if(i==0)
		return Var;
	else if(i==1)
		return Var2;
}

void VarAssign::GenerateCode(ofstream &OutFile)
{if(LHS!="")
if(Comment!="")
	OutFile<<LHS<<" = "<<RHS<<";	//"<<Comment<<endl;
else OutFile<<LHS<<" = "<<RHS<<";\n";

}
void VarAssign::Simulate(Output *pOut,Input *pIn)
{
	if(Var->name!="")
		Var->value=Var2->value;
}
