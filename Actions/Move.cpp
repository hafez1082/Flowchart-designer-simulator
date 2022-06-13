//Ahmed Hafez
#include "Move.h"

#include "..\ApplicationManager.h"

#include "..\GUI\Input.h"
#include "..\GUI\Output.h"

#include <sstream>

using namespace std;

//constructor: set the ApplicationManager pointer inside this action
Move::Move(ApplicationManager *pAppManager):Action(pAppManager)
{}

void Move::ReadActionParameters()
{
	Input *pIn = pManager->GetInput(); //Get input
	Output *pOut = pManager->GetOutput(); //Get output
	Stat=pManager->GetSelectedStatement(); //Check selected stat.

	if(Stat==NULL) //If no statement is selected
	{
		pOut->ClearStatusBar(); //Clear status bar
		pOut->PrintMessage("No statement is selected to move"); //Print Message
	}
}

void Move::Execute()
{
	ReadActionParameters(); //Including ReadActionParameters

	Output *pOut = pManager->GetOutput(); //Get output

	if(Stat !=NULL) //If statement is selected
	{
		Stat->Move(pOut,pManager); //Move using pOut and pManager
		pManager->setEditedDesign(true); //EditedDesign will be true
	}
}