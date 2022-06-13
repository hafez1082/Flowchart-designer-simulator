#include "Run.h"
#include "..\ApplicationManager.h"
#include "..\GUI\Input.h"
#include "..\GUI\Output.h"

#include <sstream>

using namespace std;


//constructor: set the ApplicationManager pointer inside this action
Run::Run(ApplicationManager *pAppManager):Action(pAppManager)
{Stat=NULL;	Conn=NULL;}

void Run::ReadActionParameters()
{


}

void Run::Execute()
{
	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();

	pOut->ClearStatusBar();

	int StatCount=pManager->GetStatCount();
	int VarCount=pManager->GetVarCount();

	for(int i=0;i<StatCount;i++)
		if(pManager->GetStatList(i)->GetStatType()=="STRT")
		{Stat=pManager->GetStatList(i); break;}


		Conn=Stat->getStatConnector(0);
		while(Conn!=NULL)
		{
			Stat=Conn->getDstStat();
			Stat->Simulate(pOut,pIn);
			if(Stat->GetStatType()=="COND")
				Conn=Stat->getStatConnector(3);
			else Conn=Stat->getStatConnector(0);
		}

}

