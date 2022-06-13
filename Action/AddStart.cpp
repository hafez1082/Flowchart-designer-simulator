#include "AddStart.h"


#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

#include <sstream>

using namespace std;

AddStart::AddStart(ApplicationManager *pAppManager):Action(pAppManager)
{}

void AddStart::ReadActionParameters()
{
	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();
	pManager->UnSelect();
	while(1)
	{pOut->PrintMessage("Start Statement: Click to add the statement");
	pIn->GetPointClicked(Position);

	Point P[4]; //array of points
	P[0].x=Position.x-UI.ASSGN_WDTH/2;            P[1].x=Position.x-UI.ASSGN_WDTH/2;             P[2].x=Position.x+UI.ASSGN_WDTH/2;               P[3].x=Position.x+UI.ASSGN_WDTH/2;
	P[0].y=Position.y-UI.ASSGN_HI/2;     P[1].y=Position.y+UI.ASSGN_HI/2;                                       P[2].y=P[1].y;        P[3].y=P[0].y;
	if(P[0].x>UI.MnItWdth&&P[0].y>UI.TlBrWdth&&P[1].y<UI.height-UI.StBrWdth)
		if (pManager->onObject(P[0]) && pManager->onObject(P[1]) && pManager->onObject(P[2]) && pManager->onObject(P[3]))
			continue;
		else
			break;
	if(pManager->onBars(Position)||pManager->onObject(Position))
	{pOut->ClearStatusBar();	return;}


	}
	pOut->ClearStatusBar();

}

void AddStart::Execute()
{
	ReadActionParameters();

	if(pManager->onBars(Position)||pManager->onObject(Position))
		return;

	Start *pAssign = new Start(Position); //create start statement

	pManager->AddStatement(pAssign);
	pManager->setEditedDesign(true);
	pManager->UndoRedo();
}
