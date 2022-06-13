#ifndef GENERATE_H
#define GENERATE_H

#include "Action.h"
#include "..\Statements\Statement.h"
#include "..\ApplicationManager.h"

class Generate : public Action
{
private:
	Statement *Stat;
	Connector *Conn;
	Variable *V;
	Statement* SList[100];
	int index;
	int w,tab;
public:
	Generate(ApplicationManager *pAppManager);

	//Read Assignemt statements position
	virtual void ReadActionParameters();

	//Create and add an assignemnt statement to the list of statements
	virtual void Execute() ;

	bool isWhile(Statement *Stat);		//check if the condition statement is while loop
	void whileCond(ofstream &myfile,Statement *Stat,Connector *&Conn);	//generate th ecode of the while loop
	void ifCond(ofstream &myfile,Connector *&Conn);		//generate the code of the if condition
	Statement *findStat(Statement *Stat);		//find the intersection statement between the if condition two branches

};

#endif