#ifndef SAVE_H
#define SAVE_H

#include "Action.h"
#include "..\Statements\Statement.h"
#include "..\ApplicationManager.h"
//Save Action
//This class is responsible for saving the chart to a text file
class Save : public Action
{
private:
	Statement *Stat;
	Connector *Conn;
public:
	Save(ApplicationManager *pAppManager);
	// Declaring the 2 functions 
	//cin Assignemt statements coords
	virtual void ReadActionParameters();

	//draw and declare an assignemnt statement to the list of statements
	virtual void Execute() ;

};

#endif