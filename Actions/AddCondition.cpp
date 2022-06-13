#include "AddCondition.h"



#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

#include <sstream>

using namespace std;

//constructor: set the ApplicationManager pointer inside this action
AddCondition::AddCondition(ApplicationManager *pAppManager):Action(pAppManager)
{}

void AddCondition::ReadActionParameters()
{
	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();
	pManager->UnSelect();
	while(1)
	{pOut->PrintMessage("Condition Statement: Click to add the statement");
	pIn->GetPointClicked(Position);

	Point P[4];
	P[0].x=Position.x;            P[1].x=Position.x-UI.ASSGN_WDTH/2;             P[2].x=P[0].x;               P[3].x=Position.x+UI.ASSGN_WDTH/2;
	P[0].y=Position.y;     P[1].y=Position.y+UI.ASSGN_HI;                                       P[2].y=P[1].y+UI.ASSGN_HI;        P[3].y=P[1].y;
	if(P[1].x>UI.MnItWdth&&P[0].y>UI.TlBrWdth&&P[2].y<UI.height-UI.StBrWdth)
		if (pManager->onObject(P[0]) && pManager->onObject(P[1]) && pManager->onObject(P[2]) && pManager->onObject(P[3]))
			continue;
		else
			break;
	if(pManager->onBars(Position)||pManager->onObject(Position))
	{pOut->ClearStatusBar();
	return;}


	}
	pOut->ClearStatusBar();

}

void AddCondition::Execute()
{
	ReadActionParameters();

	if(pManager->onBars(Position)||pManager->onObject(Position))
		return;


	Condition *pCondition = new Condition(Position);

	pManager->AddStatement(pCondition);
	pManager->setEditedDesign(true);
	pManager->UndoRedo();
}

