#ifndef RUN_H
#define RUN_H

#include "..\Statements\Statement.h"
#include "..\ApplicationManager.h"
#include "Action.h"



class Run : public Action
{
private:
	Statement* Stat;
	Connector *Conn;
public:
	Run(ApplicationManager *pAppManager);

	//Read Assignemt statements position
	virtual void ReadActionParameters();

	//Create and add an assignemnt statement to the list of statements
	virtual void Execute() ;

};

#endif