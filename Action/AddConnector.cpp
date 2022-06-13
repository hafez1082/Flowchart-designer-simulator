#include "AddConnector.h"



#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

#include <sstream>

using namespace std;

//constructor: set the ApplicationManager pointer inside this action
AddConnector::AddConnector(ApplicationManager *pAppManager):Action(pAppManager)
{ConnType=0;}

void AddConnector::ReadActionParameters()
{
	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();
	pManager->UnSelect();
	bool exit=false;
	while(!exit)
	{
		pOut->PrintMessage("Click on the first statement"); //connect the connector with the first selected statement
		pIn->GetPointClicked(Position);
		if(pManager->onBars(Position))
		{pOut->ClearStatusBar(); return;}

		SrcStat=pManager->GetStatement(Position);

		if(SrcStat!=NULL)
			exit=true;
		else continue;	

		if(SrcStat->GetStatType()=="END") //End statement can't be connected first
		{exit=false; continue;}

		SrcStat->SetSelected(true);
		pManager->UpdateInterface();


		if(SrcStat->GetStatType()=="COND") //this connects the connector with conditional statetment wheather from true or false
		{
again:
			pOut->DrawCondMsg();
			Point P;
			pIn->GetPointClicked(P);


			if(P.x>=(UI.width/2-150)+169.66&&P.x<=(UI.width/2-150)+253.38&&P.y>=(UI.height/2-75)+74.38&&P.y<=(UI.height/2-75)+103.63)		//True button
			{
				if(SrcStat->getStatConnector(1)!=NULL)
				{
					exit=false;	
					SrcStat->SetSelected(false);
					pManager->UpdateInterface();
					continue;
				}
				pManager->UpdateInterface();
				Start=SrcStat->GetConnPoint(1,1);
				ConnType=1;
			}
			else if(P.x>=(UI.width/2-150)+49.28&&P.x<=(UI.width/2-150)+133.72&&P.y>=(UI.height/2-75)+74.66&&P.y<=(UI.height/2-75)+104.32)	//false button
			{
				if(SrcStat->getStatConnector(2)!=NULL)
				{
					exit=false;	
					SrcStat->SetSelected(false);
					pManager->UpdateInterface();
					continue;
				}
				pManager->UpdateInterface();
				Start=SrcStat->GetConnPoint(1,2);
				ConnType=2;
			}
			else goto again;
		}
		else
		{
			if(SrcStat->getStatConnector(0)!=NULL)
			{
				exit=false;	
				SrcStat->SetSelected(false);
				pManager->UpdateInterface();
				continue;
			}

			Start=SrcStat->GetConnPoint(1);}

	}


	exit = false;
	while(!exit)
	{
		pOut->PrintMessage("Click on the second statement"); // connect the connector with the next selected statement
		pIn->GetPointClicked(Position);
		if(pManager->onBars(Position))
		{
			pOut->ClearStatusBar();
			SrcStat->SetSelected(0);
			return;
		}

		DstStat=pManager->GetStatement(Position);

		if(DstStat!=NULL)
			exit=true;
		else continue;

		if(DstStat->GetStatType()=="STRT") // the next statement can't be start statement
		{exit=false; continue;}

		End=DstStat->GetConnPoint(2);

	}

	SrcStat->SetSelected(false);
	pOut->ClearStatusBar();

}

void AddConnector::Execute()
{
	ReadActionParameters();
	if(pManager->onBars(Position))
		return;


	Connector *pConnect = new Connector(SrcStat,DstStat);
	pConnect->setStartPoint(Start);
	pConnect->setEndPoint(End);
	pConnect->setSrcID(SrcStat->GetID());
	pConnect->setDstID(DstStat->GetID());
	pConnect->setConnType(ConnType);

	pManager->SetConnector(pConnect);


	SrcStat->setStatConnector(pConnect,ConnType);
	pManager->setEditedDesign(true);

	pManager->UndoRedo();
}

