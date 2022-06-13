#ifndef STEP_H
#define STEP_H

#include "..\Statements\Statement.h"
#include "..\ApplicationManager.h"
#include "Action.h"



class Step : public Action
{
private:
	Statement* Stat;
	Connector *Conn;
public:
	Step(ApplicationManager *pAppManager);

	//Read Assignemt statements position
	virtual void ReadActionParameters();

	//Create and add an assignemnt statement to the list of statements
	virtual void Execute() ;

};

#endif