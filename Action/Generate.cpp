#include "Generate.h"
#include<cstdlib>
#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

#include <sstream>

using namespace std;

//constructor: set the ApplicationManager pointer inside this action
Generate::Generate(ApplicationManager *pAppManager):Action(pAppManager)
{
	Stat=NULL;
	Conn=NULL;
	V=NULL;
	index=0;
	tab=0;

}

void Generate::ReadActionParameters()
{

	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();
	pOut->PrintMessage("Design Generated to Code in file <Generated Code.txt> ");
}

void Generate::Execute()
{
	ReadActionParameters();
	int x=0,y=0;
	int StatCount=pManager->GetStatCount();
	int VarCount=pManager->GetVarCount();
	bool b1=false,b2=false;
	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();
	ofstream myfile;
	myfile.open("GeneratedCode.txt");

	for(int i=0;i<StatCount;i++)
		if(pManager->GetStatList(i)->GetStatType()=="STRT")
		{
			Stat=pManager->GetStatList(i); break;
		}
		Conn=Stat->getStatConnector(0);
		Stat->GenerateCode(myfile);
		if(VarCount!=0)
		{
			myfile<<"double ";
			for(int i=0;i<VarCount;i++)
			{
				V=pManager->GetVariable(i);
				if(i!=pManager->GetVarCount()-1)
					myfile<<V->name<<"= 0,";
				else myfile<<V->name<<"= 0;";
			}
		}
		myfile<<endl;
		while(Conn!=NULL)
		{
			Stat=Conn->getDstStat();
			if(Stat->GetStatType()=="COND")
			{
				if(isWhile(Stat))
					whileCond(myfile,Stat,Conn);
				else 
				{

					SList[index++]=Stat;
					ifCond(myfile,Conn);

				}
			}
			else {
				for(int i=0;i<tab;i++)
					myfile<<"\t";
				Stat->GenerateCode(myfile);
				Conn=Stat->getStatConnector(0);}
		}

		myfile.close();
		system("GeneratedCode.txt");

}



Statement * Generate::findStat(Statement *Stat)
{int count=0;
Connector *C1=Stat->getStatConnector(1);
Connector *C2=Stat->getStatConnector(2);
int f=0;
Statement *S1[50];
Statement *S2[50];
int x=0,y=0;
while(f<pManager->GetStatCount()&&(C1!=NULL||C2!=NULL) )
{
	if(C1!=NULL)
	{
		S1[f]=C1->getDstStat();
		if(S1[f]->GetStatType()=="COND")
		{if(S1[f]==SList[index-1])
		{index--; w=1; return NULL; }
		if(isWhile(S1[f]))
			C1=S1[f]->getStatConnector(3-w);
		else C1=S1[f]->getStatConnector(1);}
		else C1=S1[f]->getStatConnector(1);
		x++;

	}
	if(C2!=NULL)
	{

		S2[f]=C2->getDstStat();
		if(S2[f]->GetStatType()=="COND")
		{if(S2[f]==SList[index-1])
		{index--; w=2; return NULL; }

		if(isWhile(S2[f]))
			C2=S2[f]->getStatConnector(3-w);
		else C2=S2[f]->getStatConnector(2);}
		else C2=S2[f]->getStatConnector(2);
		y++;

	}
	f++;
}
for(int i=0;i<x;i++)
	for(int j=0;j<y;j++)
		if(S1[i]==S2[j])
		{
			return S1[i];
		}


		return NULL;

}

bool Generate::isWhile(Statement *Stat)
{Connector *C1=Stat->getStatConnector(1);
Connector *C2=Stat->getStatConnector(2);
Statement *S;
while(C1->getDstStat()!=Stat)
{
	S=C1->getDstStat();
	while(S->GetStatType()=="COND")
	{	SList[index++]=Stat;
	if(!findStat(S))
		return true;

	S=findStat(S);
	if(S==Stat)
		break;
	index--;}
	if(S==Stat)
		break;


	C1=S->getStatConnector(0);
	if(C1==NULL)
		break;
}
if(C1!=NULL)
	if(C1->getDstStat()==Stat||S==Stat)
	{
		w=1;	
		return true;
	}

	while(C2->getDstStat()!=Stat)
	{
		S=C2->getDstStat();
		while(S->GetStatType()=="COND")
		{SList[index++]=Stat;
		if(!findStat(S))
			return true;

		S=findStat(S);
		if(S==Stat)
			break;
		index--;
		}

		if(S==Stat)
			break;

		C2=S->getStatConnector(0);
		if(C2==NULL) break;
	}
	if(C2!=NULL)
		if(C2->getDstStat()==Stat||S==Stat)
		{
			w=2;
			return true;
		}

		return false;
}

void Generate::whileCond(ofstream &myfile,Statement *Stat,Connector *&Conn)
{
	Statement *S;
	Connector *C=Stat->getStatConnector(w);
	for(int i=0;i<tab;i++)
		myfile<<"\t";
	if(w==1)
		myfile<<"while (";
	else if(w==2)
		myfile<<"while (!";
	Stat->GenerateCode(myfile);
	for(int i=0;i<tab;i++)
		myfile<<"\t";
	myfile<<"{\n";
	tab++;
	while(C->getDstStat()!=Stat)
	{
		S=C->getDstStat();
		if(S->GetStatType()=="COND")
		{
			SList[index++]=S;
			ifCond(myfile,C);

		}
		else {for(int i=0;i<tab;i++)
			myfile<<"\t";

		S->GenerateCode(myfile); C=S->getStatConnector(0);}
	}tab--;

	for(int i=0;i<tab;i++)
		myfile<<"\t";

	myfile<<"}\n";
	Conn=Stat->getStatConnector(3-w);




}




void Generate::ifCond(ofstream &myfile,Connector *&Conn)
{Statement *S;
for(int i=0;i<tab;i++)
	myfile<<"\t";

myfile<<"if (";
SList[index-1]->GenerateCode(myfile);
for(int i=0;i<tab;i++)
	myfile<<"\t";
myfile<<"{\n";
tab++;
Connector *Conn1=SList[index-1]->getStatConnector(1);
Connector *Conn2=SList[index-1]->getStatConnector(2);
while(Conn1->getDstStat()!=findStat(SList[index-1]))
{S=Conn1->getDstStat();
if(S->GetStatType()=="COND")
	if(isWhile(S))
		whileCond(myfile,S,Conn1);
	else 
	{
		SList[index++]=S;
		ifCond(myfile,Conn1);
	}
else {
	for(int i=0;i<tab;i++)
		myfile<<"\t";

	S->GenerateCode(myfile); Conn1=S->getStatConnector(0);}
}tab--;
for(int i=0;i<tab;i++)
	myfile<<"\t";
myfile<<"} \n";

for(int i=0;i<tab;i++)
	myfile<<"\t";
myfile<<"else\n";

for(int i=0;i<tab;i++)
	myfile<<"\t";

myfile<<"{\n";

tab++;
while(Conn2->getDstStat()!=findStat(SList[index-1]))
{
	S=Conn2->getDstStat();
	if(S->GetStatType()=="COND")
		if(isWhile(S))
			whileCond(myfile,S,Conn2);
		else 
		{
			SList[index++]=S;
			ifCond(myfile,Conn2);
		}
	else {for(int i=0;i<tab;i++)
		myfile<<"\t";

	S->GenerateCode(myfile);	Conn2=S->getStatConnector(0);}
}tab--;
for(int i=0;i<tab;i++)
	myfile<<"\t";
myfile<<"}\n";
Conn=Conn2;
index--;


}
