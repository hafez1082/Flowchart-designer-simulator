#ifndef SELECT_H
#define SELECT_H


#include "..\Statements\Statement.h"
#include "..\ApplicationManager.h"
#include "Action.h"

class Select : public Action
{
private:
	Point Position;	//Position where the user clicks to edit the stat.
	Statement *Stat;
	Connector *Conn;
public:
	Select(ApplicationManager *pAppManager);

	//Read Assignemt statements position
	virtual void ReadActionParameters();

	//Create and add an assignemnt statement to the list of statements
	virtual void Execute() ;

};

#endif