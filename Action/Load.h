#pragma "once"
#ifndef LOAD_H
#define LOAD_H

#include "Action.h"
#include "..\Statements\Statement.h"
#include "..\ApplicationManager.h"
//Load Action
//This class is responsible for getting the file name and loading the corresponding chart from the file
class Load : public Action
{
private:
	Statement *Stat;
	Connector *Conn;
	Variable *V;
public:
	Load(ApplicationManager *pAppManager);

	//cin Assignemt statements position
	//Read File name from the user
	virtual void ReadActionParameters();

	//draw and declare an assignemnt statement to the list of statements
	//Loading the corresponding file
	virtual void Execute() ;

};

#endif