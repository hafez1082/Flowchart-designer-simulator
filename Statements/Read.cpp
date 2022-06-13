#include "Read.h"
#include <sstream>
#include "..\ApplicationManager.h"
using namespace std;

Read::Read(Point Lcorner, string Variable)
{	
	StatType="READ";
	Line = Variable;

	//Will be set in actions
	value = 0;

	UpdateStatementText();

	LeftCorner = Lcorner;

	pConn = NULL;	//No connectors yet
	width=UI.ASSGN_WDTH;
	height=UI.ASSGN_HI;
	Inlet.x = LeftCorner.x + width /2;
	Inlet.y = LeftCorner.y;

	Outlet.x = Inlet.x-50;
	Outlet.y = LeftCorner.y + height;	



}

void Read::setVar(const string &Variables)
{

	Line = Variables;

	UpdateStatementText();
}

void Read::Draw(Output* pOut) const
{
	//Call Output::DrawAssign function to draw assignment statement 	
	pOut->DrawRead_Write(LeftCorner,width , height,"READ",Line, Selected);

}

void Read::setStatConnector(Connector *Conn,int ConnType)
{
	pConn=Conn;
}

Connector* Read::getStatConnector(int ConnType)
{
	return pConn;
}

//This function should be called when LHS or RHS changes
void Read::UpdateStatementText()
{

	if(Line=="")	//No Line ==>statement have no valid text yet
		Text = "Read ";
	else{
		//Build the statement text: Read then equals Line
		ostringstream T;
		T<<"Read "<<Line;
		Text = T.str();	 
	}

}


bool Read::IsOnStat(Point P)		// check if the point in the statement area or not
{int x[4],y[4];
x[0]=LeftCorner.x;     x[1]=x[0]+width;          x[2]=x[1]-50;           x[3]=x[0]-50;
y[0]=LeftCorner.y;                      y[1]=y[0];                                   y[2]=y[0]+height;      y[3]=y[2];
int i, j, c = 0;
for (i = 0, j = 3; i < 4; j = i++) 
	if ( ((y[i]>P.y) != (y[j]>P.y)) && (P.x < (x[j]-x[i]) * (P.y-y[i]) / (y[j]-y[i]) + x[i]) )
		c = !c;
if(c%2!=0)
	return true;
return false;


}

Point Read::GetConnPoint(int Order,int connType)	//get connection point of the statement due to it's order
{
	if(Order==1)
		return Outlet;
	return Inlet;
}

void Read::Edit(Output *pOut,Input *pIn)		//Edit the Statement parameters
{
	string S;
	while(1)
	{pOut->PrintMessage("Enter the name of variable you want to Read");
	S=pIn->GetString(pOut,Line);
	if(CheckVar(S))
	{setVar(S);	break;}}
	pOut->ClearStatusBar();
	Var->name=Line;
}

//Ahmed Hafez
//Move Read stat.
void Read::Move(Output *pOut,ApplicationManager *pManager)
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
		pOut->PrintMessage("Move Read statement anywhere in the drawing area ,press ESC to exit");

		// Dragging 
		if(Drag == false) 
		{
			if(pWind->GetButtonState(LEFT_BUTTON, P.x, P.y) == BUTTON_DOWN) 
			{
				if(IsOnStat(P)) //Check rang of P
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
					if(LeftCorner.x + (P.x - XOld)-50>UI.MnItWdth&&LeftCorner.x + (P.x - XOld)+width<UI.width)
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

		// Draw read
		Inlet.x = LeftCorner.x + width /2;
		Inlet.y = LeftCorner.y;

		Outlet.x = Inlet.x;
		Outlet.y = LeftCorner.y + height;	

		if(pConn!=NULL) //Checks connector
			pConn->setStartPoint(Outlet);

		pManager->EditConnectors(this); //To edit connector
		pManager->UpdateInterface(); //To update
		pOut->CreateDesignToolBar(); //create design tool bar

		// Update the screen buffer
		pWind->UpdateBuffer();
	}
	pWind->SetBuffering(false);
}


void Read::Resize(char R){		//Resize the Statement


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

	Outlet.x = Inlet.x-50;
	Outlet.y = LeftCorner.y + height;

	if(pConn!=NULL)
		pConn->setStartPoint(Outlet);
}


Statement* Read::Copy(Point P)
{
	Point New;
	New.x=P.x-width/2;
	New.y=P.y;
	Read *pRead = new Read(New,Line);
	return pRead;
}

void Read::Save(ofstream &OutFile){	//Save the Statement parameters to a file


	OutFile<<StatType<<"\t"<<ID<<"\t"<<LeftCorner.x<<"\t"<<LeftCorner.y<<"\t"<<Line<<"\t“"<<Comment<<"”"<<endl;
	//Print comment
}

void Read::Load(ifstream &Infile)	//Load the Statement parameters from a file
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
		D >> Line;Comment=line.substr(line.find('“')+1);
		Comment.resize(Comment.size() -1 );
	}

	Var->name=Line;
	Inlet.x = LeftCorner.x + width /2;
	Inlet.y = LeftCorner.y;

	Outlet.x = Inlet.x-50;
	Outlet.y = LeftCorner.y + height;	

	UpdateStatementText();
}

void Read::GenerateCode(ofstream &OutFile) 	//write the statement code to a file
{if(Line!="")
if(Comment!="")
	OutFile << "cin >> " << Line <<";	//"<<Comment<<endl;
else OutFile << "cin >> " << Line <<";\n";

}

void Read::Simulate(Output *pOut,Input *pIn)
{
	if(Var->name!="")
	{
		pOut->PrintMessage("please enter a value of "+Var->name);
		Var->value=pIn->GetValue(pOut);
	}
}