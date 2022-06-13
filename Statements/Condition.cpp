#include "Condition.h"
#include <sstream>
#include "..\ApplicationManager.h"
using namespace std;

Condition::Condition(Point T, string LeftHS,string Op, string RightHS)
{
	Var2=new Variable;
	Var2->name="";
	Var2->value=0;
	Var2->repeat=1;
	D=0;
	v="";
	StatType="COND";
	LHS = LeftHS;
	RHS = RightHS;
	Operator=Op;
	UpdateStatementText();

	Top = T;

	TConn = NULL;	//No connectors yet
	FConn = NULL;
	ResultConn=NULL;
	width=UI.ASSGN_WDTH;
	height=2*UI.ASSGN_HI;

	Inlet.x = Top.x;
	Inlet.y = Top.y;

	TOutlet.x = Top.x + width/2;
	TOutlet.y = Top.y + height/2;

	FOutlet.x = Top.x - width/2;
	FOutlet.y = Top.y + height/2;
}

void Condition::setLHS(const string &L)
{
	LHS = L;
	UpdateStatementText();
}
void Condition::setOperator(const string &Op)
{
	Operator = Op;
	UpdateStatementText();
}
void Condition::setRHS(const string &R)
{
	RHS = R;
	UpdateStatementText();
}

bool Condition::editRHS(string RHS)
{if(RHS=="")
return false;
if(CheckVar(RHS))
{
	v=RHS;
	Var2->name=RHS;
	return true;}
else if(CheckVal(RHS))
{Var2=new Variable; Var2->name=""; Var2->value=0; Var2->repeat=1;
D=stod(RHS); 
return true;}
return false;

}
void Condition::Draw(Output* pOut) const
{
	//Call Output::DrawAssign function to draw assignment statement 	

	pOut->DrawCondition(Top,width,height,Text,Selected);

}
void Condition::setStatConnector(Connector *Conn,int ConnType)
{
	if(ConnType==1)
		TConn=Conn;
	else if(ConnType==2)
		FConn=Conn;

}

Connector* Condition::getStatConnector(int ConnType)
{
	if(ConnType==1)
		return TConn;
	else if(ConnType==2)
		return FConn;
	else if(ConnType==3)
		return ResultConn;
}




//This function should be called when LHS or RHS changes
void Condition::UpdateStatementText()
{
	if(LHS=="")	//No left handside ==>statement have no valid text yet
		Text = "";
	else
	{
		//Build the statement text: Left handside then equals then right handside
		ostringstream T;

		T<<"if ("<<LHS<<" "<<Operator<<" "<<RHS<<" )";
		Text = T.str();	 
	}
}

bool Condition::IsOnStat(Point P)
{
	int x[4],y[4];
	x[0]=Top.x;            x[1]=Top.x-width/2;             x[2]=x[0];               x[3]=Top.x+width/2;
	y[0]=Top.y;     y[1]=Top.y+height/2;                                       y[2]=y[1]+height/2;        y[3]=y[1];
	int i, j, c = 0;
	for (i = 0, j = 3; i < 4; j = i++) 
		if ( ((y[i]>P.y) != (y[j]>P.y)) && (P.x < (x[j]-x[i]) * (P.y-y[i]) / (y[j]-y[i]) + x[i]) )
			c = !c;
	if(c%2!=0) //a
		return true;
	return false;

}

Point Condition::GetConnPoint(int Order,int ConnType)
{
	if(Order==1)
	{
		if(ConnType==1)
			return TOutlet;
		else if(ConnType==2)
			return FOutlet;
	}
	else if(Order==2)
		return Inlet;

}



void Condition::Edit(Output *pOut,Input *pIn)
{
	string S;

	while(1)
	{pOut->PrintMessage("enter the < LeftHand Side > ");
	S=pIn->GetString(pOut,LHS);
	if(CheckVar(S))
	{setLHS(S);	break;}}

	while(1)
	{pOut->PrintMessage("enter the < Comparison Operator > { ==, !=, >, <, >= and <= }");
	S=pIn->GetString(pOut,Operator);
	if(S=="=="||S=="!="||S==">"||S=="<"||S==">="||S=="<=")
	{setOperator(S);	break;}
	}
	while(1)
	{pOut->PrintMessage("enter the < RightHand Side > ");
	RHS=pIn->GetString(pOut,RHS);
	if(editRHS(RHS))
	{setRHS(RHS); break;}
	pOut->ClearStatusBar();
	}
	Var->name=LHS;
}

//Ahmed Hafez
//Move Condition stat.
void Condition::Move(Output *pOut,ApplicationManager *pManager)
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
		pOut->PrintMessage("Move Condition statement anywhere in the drawing area ,press ESC to exit"); 

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
					Top.x = Top.x + (P.x - XOld);
					XOld = P.x;
				}
				if(P.y != YOld) 
				{
					Top.y = Top.y + (P.y - YOld);
					YOld = P.y;
				}
			}
		}

		// Draw condition
		TOutlet.x = Top.x+width/2;
		TOutlet.y = Top.y + height/2;

		FOutlet.x = Top.x-width/2;
		FOutlet.y = Top.y + height/2;

		Inlet.x = Top.x;
		Inlet.y = Top.y;

		if(TConn!=NULL) //Checks connector in True 
			TConn->setStartPoint(TOutlet); 

		if(FConn!=NULL) //Checks connector in false
			FConn->setStartPoint(FOutlet);

		pManager->EditConnectors(this); //To edit connector
		pManager->UpdateInterface(); //To update
		pOut->CreateDesignToolBar(); //Create design tool bar

		// Update the screen buffer
		pWind->UpdateBuffer();
	}
	pWind->SetBuffering(false);
}


void Condition::Resize(char R)
{
	if(R=='+')
	{if(width>=1.5*UI.ASSGN_WDTH)
	return;

	Top.y-=0.25*height;
	height*=1.5;
	width*=1.5;

	}
	else if(R=='-')
	{if(width<=(2.0/3)*UI.ASSGN_WDTH)
	return;

	Top.y+=(1.0/6)*height;
	height/=2;
	width/=2;

	}

	Inlet.x = Top.x;
	Inlet.y = Top.y;

	TOutlet.x = Top.x + width/2;
	TOutlet.y = Top.y + height/2;

	FOutlet.x = Top.x - width/2;
	FOutlet.y = Top.y + height/2;

	if(TConn!=NULL)
		TConn->setStartPoint(TOutlet);
	if(FConn!=NULL)
		FConn->setStartPoint(FOutlet);

}

Statement* Condition::Copy(Point P)
{ 
	Condition *pCondition = new Condition(P,LHS,Operator,RHS);
	return pCondition;
}

void Condition::Save(ofstream &OutFile)
{
	OutFile<<StatType<<"\t"<<ID<<"\t"<<Top.x<<"\t"<<Top.y<<"\t"<<LHS<<"\t"<<Operator<<"\t"<<RHS<<"\t�"<<Comment<<"�"<<endl;
}



void Condition::Load(ifstream &Infile)
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
		B >> Top.x;
		getline( liness, a, '\t' );
		istringstream C(a);
		C >> Top.y;
		getline( liness, a, '\t' );
		istringstream D(a);
		D >> LHS;
		getline( liness, a, '\t' );
		istringstream E(a);
		E >> Operator;
		getline( liness, a, '\t' );
		istringstream F(a);
		F >> RHS;
		Comment=line.substr(line.find('�')+1);
		Comment.resize(Comment.size() -1 );
	}			

	Var->name=LHS;
	if(RHS!="")
		editRHS(RHS);

	Inlet.x = Top.x;
	Inlet.y = Top.y;

	TOutlet.x = Top.x + width/2;
	TOutlet.y = Top.y + height/2;

	FOutlet.x = Top.x - width/2;
	FOutlet.y = Top.y + height/2;
	UpdateStatementText();

}


void Condition::setVar(Variable *V,int i)
{
	if(i==0)
		Var=V;
	else if(i==1)
		Var2=V;
}
Variable *Condition::getVar(int i)
{
	if(i==0)
		return Var;
	else if(i==1)
		return Var2;
}


void Condition::GenerateCode(ofstream &OutFile)
{if(Text!="")

if(Comment!="")
	OutFile<<"("<<LHS<<" "<<Operator<<" "<<RHS<<" ))  //"<<Comment<<"\n";
else
	OutFile<<"("<<LHS<<" "<<Operator<<" "<<RHS<<" ))\n";


}
void Condition::Simulate(Output *pOut,Input *pIn)
{
	if(Var->name!="")
	{
		if(Var2->name=="")
		{if(Operator=="==")
		if(Var->value==D)
			ResultConn=TConn;
		else ResultConn=FConn;
		else if(Operator=="!=")
			if(Var->value!=D)
				ResultConn=TConn;
			else ResultConn=FConn;
		else if(Operator==">")
			if(Var->value>D)
				ResultConn=TConn;
			else ResultConn=FConn;
		else if(Operator=="<")
			if(Var->value<D)
				ResultConn=TConn;
			else ResultConn=FConn;
		else if(Operator==">=")
			if(Var->value>=D)
				ResultConn=TConn;
			else ResultConn=FConn;
		else if(Operator=="<=")
			if(Var->value<=D)
				ResultConn=TConn;
			else ResultConn=FConn;
		}
		else if(Var2->name!="")
		{if(Operator=="==")
		if(Var->value==Var2->value)
			ResultConn=TConn;
		else ResultConn=FConn;
		else if(Operator=="!=")
			if(Var->value!=Var2->value)
				ResultConn=TConn;
			else ResultConn=FConn;
		else if(Operator==">")
			if(Var->value>Var2->value)
				ResultConn=TConn;
			else ResultConn=FConn;
		else if(Operator=="<")
			if(Var->value<Var2->value)
				ResultConn=TConn;
			else ResultConn=FConn;
		else if(Operator==">=")
			if(Var->value>=Var2->value)
				ResultConn=TConn;
			else ResultConn=FConn;
		else if(Operator=="<=")
			if(Var->value<=Var2->value)
				ResultConn=TConn;
			else ResultConn=FConn;
		}





	}


}
