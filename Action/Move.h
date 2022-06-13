//Ahmed Hafez
#ifndef MOVE_H
#define MOVE_H

#include "Action.h"
#include "..\ApplicationManager.h"

//Move Statments class
//Responsible for moving the selected statements and connectors
class Move : public Action
{
private:
	Point Position; //The position of the stat.
	Statement*Stat; //Check if statement is selected
public:
	Move(ApplicationManager *pAppManager);

	//Read Assignemt statements position
	virtual void ReadActionParameters();

	//Create and add an assignemnt statement to the list of statements
	virtual void Execute() ;

};

#endif