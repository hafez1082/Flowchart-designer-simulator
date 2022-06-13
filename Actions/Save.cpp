#include "Save.h"
#include<cstdlib>


#pragma "once"
#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

#include <sstream>
#include <fstream>

using namespace std;

//used constructor to set theApplicationManager pointer inside the given action
Save::Save(ApplicationManager *pAppManager):Action(pAppManager)
{
	Stat=NULL;
}
//needed feedback for the user 
void Save::ReadActionParameters()
{

	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();
	pOut->PrintMessage("Saving in <Test.txt> ");
}

//the saving process itself
void Save::Execute()
{
	// basic in/out data fetch
	ReadActionParameters();
	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();
	ofstream myfile;

	//creating/opining the file in which the data would be saved in
	myfile.open("Test.txt");
	myfile<<pManager->GetVarCount()<<endl;
	for(int i=0;i<pManager->GetVarCount();i++)
		myfile<<pManager->GetVariable(i)->name<<"		";

	myfile<<endl<<pManager->GetStatCount()<<endl;
	for(int i=0;i<pManager->GetStatCount();i++)
	{
		Stat=pManager->GetStatList(i);
		Stat->Save(myfile);
	}
	myfile<<pManager->GetConnCount()<<endl;
	for(int i=0;i<pManager->GetConnCount();i++)
	{
		Conn=pManager->GetConnList(i);
		Conn->Save(myfile);
	}
	myfile.close();
	system("Test.txt");
	pManager->setEditedDesign(false);

}