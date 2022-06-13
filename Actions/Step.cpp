#include "Step.h"
#include "..\ApplicationManager.h"
#include "..\GUI\Input.h"
#include "..\GUI\Output.h"

#include <sstream>

using namespace std;


//constructor: set the ApplicationManager pointer inside this action
Step::Step(ApplicationManager *pAppManager):Action(pAppManager)
{Stat=NULL; Conn=NULL;}

void Step::ReadActionParameters()
{


}

void Step::Execute()
{
	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();

	pOut->ClearStatusBar();
	pManager->InitializeVar();
	int StatCount=pManager->GetStatCount();
	int VarCount=pManager->GetVarCount();
	Variable* arr[10];
	Variable *V,*V2;
	int index=0;
	char Key;
	pOut->PrintMessage("to step over press Enter , to exit the debug press Escape");
	for(int i=0;i<StatCount;i++)
		if(pManager->GetStatList(i)->GetStatType()=="STRT")
		{
			Stat=pManager->GetStatList(i); 
			break;
		}

		Stat->SetSelected(1);
		pManager->UpdateInterface();
		Conn=Stat->getStatConnector(3);
		pOut->getwindow()->WaitKeyPress(Key);
		if(Key==13&&pOut->getwindow()->GetKeyPress(Key)!=ESCAPE)
		{
			while(Conn!=NULL)
			{
				int i=0;
				Stat->Simulate(pOut,pIn);
				if(Stat->GetStatType()!="COND"&&Stat->GetStatType()!="SNGLOP"&&Stat->GetStatType()!="VAR")
					V=Stat->getVar(0);
				else {V=Stat->getVar(0);	V2=Stat->getVar(1);

				for(i=0;i<index;i++)
					if(arr[i]->name==V2->name)
					{
						arr[i]->value=V2->value; break;}
					if(i==index)
						if(V2->name!="")
							arr[index++]=V2;
				}

				for(i=0;i<index;i++)
					if(arr[i]->name==V->name)
					{if(Stat->GetStatType()!="COND") arr[i]->value=V->value;	break;}
					if(i==index)
						if(V->name!="")
							arr[index++]=V;



					Conn->SetSelected(0);
					Stat->SetSelected(0);
					Conn=Stat->getStatConnector(3);
					if(Conn==NULL)
						break;
					Stat=Conn->getDstStat();
					Conn->SetSelected(1);
					Stat->SetSelected(1);
					pManager->UpdateInterface();
					string s;
					for(int i=0;i<index;i++)
						s+=" < "+arr[i]->name+" = "+to_string(long double(arr[i]->value))+" > ";

					pOut->PrintMessage(s);

					pOut->getwindow()->WaitKeyPress(Key);




					if(Key==27)
					{
						Conn->SetSelected(0);
						Stat->SetSelected(0);
						break;
					}
			}

		}
		pOut->ClearStatusBar();

}