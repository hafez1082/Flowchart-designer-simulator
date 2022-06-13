#include "Statement.h"

Statement::Statement()	
{
	Var=new Variable;
	Var->name="";
	Var->value=0;
	Var->repeat=1;
	ID=0;
	Text = "";
	Comment="";
	StatType="None";
	Selected = false;		
}

void Statement::SetSelected(bool s)
{	Selected = s; }

bool Statement::IsSelected() const
{	return Selected; }


string Statement::GetStatType()
{
	return StatType;

}

void Statement::SetID(int I)
{
	ID=I;
}

int Statement:: GetID()
{
	return ID;

}


void Statement::AddComment(string C)
{
	Comment= C;
}
string Statement::GetComment()
{
	return Comment;
}
bool Statement::CheckVar(string t)
{
	if(((int)t[0]>=65&&(int)t[0]<=90)||((int)t[0]>=97&&(int)t[0]<=122))
	{	for(int i=1;i<t.length();i++)
	if((int)t[i]<48||((int)t[i]>57&&(int)t[i]<65)||((int)t[i]>90&&(int)t[i]<97)||(int)t[i]>122)
		return false;
	return true;
	}
	return false;
}

bool Statement::CheckVal(string Str)
{
	int count=0;
	for(int i=0;i<Str.length();i++)
		if(Str[i]=='.')
			count++;
	if(count>1)
		return false;

	for(int i=0;i<Str.length();i++)
		if(((int)Str[i]<48||(int)Str[i]>57)&&Str[i]!='.')
			return false; 

	return true;

}



void Statement::PrintInfo(Output *pOut)
{
	pOut->PrintMessage(Text + " // " + Comment );
}


void Statement::UpdateStatementText()
{
	return ;
}



void Statement::Resize(char R)
{
	return ;
}

Statement* Statement::Copy(Point P)
{
	return NULL;
}

void Statement::setVar(Variable *V,int i)
{

	Var=V;
}

Variable* Statement::getVar(int i)
{
	return Var ;
}
void Statement::Simulate(Output *pOut,Input *pIn)
{
	return;

}
