#include "SOAssign.h"
#include <sstream>
#include <cstdlib>
#include "..\ApplicationManager.h"
using namespace std;

SOAssign::SOAssign(Point Lcorner, string LeftHS, string RightHS)
{
	Var2=new Variable;
	Var2->name="";
	Var2->value=0;
	Var2->repeat=1;
	StatType="SNGLOP";
	LHS =LeftHS;
	RHS=RightHS;
	D1=0;
	D2=0;
	v1="";
	v2="";
	Operator="";


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

void SOAssign::setLHS(const string &L)
{
	LHS = L;
	UpdateStatementText();
}

void SOAssign::setRHS(const string &R)
{   
	RHS=R;
	UpdateStatementText();
}



bool SOAssign::editRHS(string &RHS)
{if(RHS=="")
return false;
string s1="",s2="",s4="",s3="";
double d1=0,d2=0;
int i=0;
while(RHS[i]!='+'&&RHS[i]!='-'&&RHS[i]!='*'&&RHS[i]!='/'&&i!=RHS.length())
	i++;

if(i==RHS.length())
	return false;

s1=RHS.substr(0,i);
s2=RHS.substr(i+1);
if(CheckVar(s1))
	s3=s1;
else if(CheckVal(s1))
	d1=stod(s1);
else return false;
if(CheckVar(s2))
	s4=s2;
else if(CheckVal(s2))
	d2=stod(s2);

else return false;
ostringstream T;
if(s3!=""&&d2!=0)
	T<<s3<<RHS[i]<<d2;
else if(d1!=0&&s4!="")
	T<<d1<<RHS[i]<<s4;
else if(d1!=0&&d2!=0)
	T<<d1<<RHS[i]<<d2;
else return false;

D1=d1;	D2=d2;	v1=s3;	v2=s4;	Operator=RHS[i];	
RHS= T.str();
return true;
}



void SOAssign::setStatConnector(Connector *Conn,int ConnType)
{
	pConn=Conn;
}

Connector* SOAssign::getStatConnector(int ConnType)
{
	return pConn;
}



void SOAssign::Draw(Output* pOut) const
{
	//Call Output::DrawAssign function to draw assignment statement 	
	pOut->DrawAssign(LeftCorner, width, height, Text, Selected);

}


//This function should be called when LHS or RHS changes
void SOAssign::UpdateStatementText()
{
	if(LHS=="")	//No left handside ==>statement have no valid text yet
		Text = "    =";
	else
	{
		//Build the statement text: Left handside then equals then right handside
		ostringstream T;
		T<<LHS<<" = "<<RHS;	
		Text = T.str();	 


	}
}

void SOAssign::Resize(char R) 
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

void SOAssign::Edit(Output *pOut,Input *pIn)
{

	string S;

	while(1)
	{   pOut->PrintMessage("enter the < LeftHand Side > ");
	S=pIn->GetString(pOut,LHS);
	if(CheckVar(S))
	{setLHS(S);	break;}}


	while(1)
	{
		pOut->PrintMessage("enter the < RightHand Side > ");
		RHS=pIn->GetString(pOut,RHS);

		if(editRHS(RHS))
		{setRHS(RHS); break;}
	}

	Var->name=LHS;
	if(v1!="")
		Var2->name=v1;
	else if(v2!="")
		Var2->name=v2;
	else {Var2=new Variable; Var2->name=""; Var2->value=0; Var2->repeat=1;}
}

Point SOAssign::GetConnPoint(int Order,int connType)
{
	if(Order==1)
		return Outlet;
	return Inlet;
}

bool SOAssign::IsOnStat(Point p)
{
	if(p.x>=LeftCorner.x&&p.x<=(LeftCorner.x+width))
		if(p.y>=LeftCorner.y&&p.y<=(LeftCorner.y+height))
			return true;

	return false;

}

Statement* SOAssign::Copy(Point P)
{	
	Point New;
	New.x=P.x-width/2;
	New.y=P.y;
	SOAssign *pAssign = new SOAssign(New,LHS,RHS);
	return pAssign; 

}

//Ahmed Hafez
//Move Operator Assign stat.
void SOAssign::Move(Output *pOut,ApplicationManager *pManager)
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
		pOut->PrintMessage("Move Operator Assign statement anywhere in the drawing area ,press ESC to exit"); 

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

		// Draw SOAssign
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


void SOAssign::Save(ofstream &OutFile)
{

	OutFile<<StatType<<"\t"<<ID<<"\t"<<LeftCorner.x<<"\t"<<LeftCorner.y<<"\t"<<LHS<<"\t"<<RHS<<"\t“"<<Comment<<"”"<<endl;


}

void SOAssign::Load(ifstream &Infile)
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
	editRHS(RHS);
	Var->name=LHS;
	if(v1!="")
		Var2->name=v1;
	else if(v2!="")
		Var2->name=v2; 
	Inlet.x = LeftCorner.x + width /2;
	Inlet.y = LeftCorner.y;

	Outlet.x = Inlet.x;
	Outlet.y = LeftCorner.y + height;	

	UpdateStatementText();

}

void SOAssign::setVar(Variable *V,int i)
{
	if(i==0)
		Var=V;
	else if(i==1)
		Var2=V;
}
Variable *SOAssign::getVar(int i)
{
	if(i==0)
		return Var;
	else if(i==1)
		return Var2;
}


void SOAssign::GenerateCode(ofstream &OutFile)
{if(LHS!="")
if(Comment!="")
	OutFile<<LHS<<" = "<<RHS<<";	//"<<Comment<<endl;
else OutFile<<LHS<<" = "<<RHS<<";\n";

}

void SOAssign::Simulate(Output *pOut,Input *pIn)
{if(Var->name!="")
{
	if(Var2->name=="")
	{if(Operator=="+")
	Var->value=D1+D2;
	else if(Operator=="-")
		Var->value=D1-D2;
	else if(Operator=="*")
		Var->value=D1*D2;
	else if(Operator=="/")
		Var->value=D1/D2;
	}
	else 
	{
		if(D1==0&&D2!=0)
		{if(Operator=="+")
		Var->value=Var2->value+D2;
		else if(Operator=="-")
			Var->value=Var2->value-D2;
		else if(Operator=="*")
			Var->value=Var2->value*D2;
		else if(Operator=="/")
			Var->value=Var2->value/D2;
		}
		else if(D2==0&&D1!=0)
		{
			if(Operator=="+")
				Var->value=D1+Var2->value;
			else if(Operator=="-")
				Var->value=D1-Var2->value;
			else if(Operator=="*")
				Var->value=D1*Var2->value;
			else if(Operator=="/")
				Var->value=D1/Var2->value;
		}
	}







}


}
