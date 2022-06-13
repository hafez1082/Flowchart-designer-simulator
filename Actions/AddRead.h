#ifndef ADD_READ_H
#define ADD_READ_H

#include "Action.h"
#include "..\Statements\Read.h"
//Add Read Statement Action
//This class is responsible for 
// 1 - Getting read stat. coordinates from the user
// 2 - Creating an object of read class and fill it parameters
// 3 - Adding the created object to the list of statements

class AddRead : public Action
{
private:
	Point Position;	//Position where the user clicks to add the stat.
public:
	AddRead(ApplicationManager *pAppManager);
	//Read Read statements position

	virtual void ReadActionParameters();
	//Create and add a Read statement to the list of statements
	virtual void Execute() ;

};

#endif