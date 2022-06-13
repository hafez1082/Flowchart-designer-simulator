#ifndef STATEMENT_H
#define STATEMENT_H
#include<fstream>


#include "..\defs.h"
#include "Connector.h"
//class Output;
#include "..\GUI\Output.h"

class ApplicationManager;

struct Variable
{string name;
double value;
int repeat;
};
//Base class for all Statements
class Statement
{
protected:
	int ID;		//Each Statement has an ID
	string Text;		//Statement text (e.g.  "X = 5" OR "if(salary > 3000)" and so on )
	string	Comment;	//comment of the statement
	string StatType;	//the statement type ( "COND" , "START" , .... etc )
	bool Selected;	//true if the statement is selected on the folwchart
	Variable *Var;

	virtual void UpdateStatementText();	//is called when any part of the stat. is edited	

	/// Add more parameters if needed.

public:
	Statement();
	void SetSelected(bool s);
	bool IsSelected() const;
	string GetStatType();	//get the statement type 
	void SetID(int I);		//set ID of the statement
	int GetID();			// get ID of the statement
	void AddComment(string C); //add a comment to a statement
	string GetComment();
	bool CheckVar(string t);	//check if the variable name is valid or not
	bool CheckVal(string str);	//check if the input value is a valid number or not
	void PrintInfo(Output* pOut);	//print all Statement info on the status bar


	virtual void Draw(Output* pOut) const  = 0 ;	//Draw the statement




	///The following functions should be supported by the Statement class
	///It should be overridden by each inherited Statement
	///Decide the parameters that you should pass to each function	

	virtual bool IsOnStat(Point P) = 0;  // check if the point in the statement area or not
	virtual Point GetConnPoint(int Order,int connType=0) = 0;	//get connection point of the statement due to it's order
	virtual void setStatConnector(Connector *Conn,int ConnType) = 0;		//set the exit connector of the statement 
	virtual Connector* getStatConnector(int ConnType) = 0;	//get the exit connector of the statement
	virtual void Edit(Output *pOut,Input *pIn) = 0;		//Edit the Statement parameters

	virtual void Move(Output *pOut,ApplicationManager *pManager) = 0;		//Move the Statement on the flowchart
	virtual void Resize(char R);		//Resize the Statement

	virtual Statement* Copy(Point P) ;			//copy the statement into another one

	virtual void Save(ofstream &OutFile) = 0;	//Save the Statement parameters to a file
	virtual void Load(ifstream &Infile) = 0;	//Load the Statement parameters from a file

	virtual void setVar(Variable *V,int i);
	virtual Variable* getVar(int i);

	virtual void GenerateCode(ofstream &OutFile) = 0;	//write the statement code to a file
	virtual void Simulate(Output *pOut,Input *pIn) ;	//Execute the statement in the simulation mode




};

#endif