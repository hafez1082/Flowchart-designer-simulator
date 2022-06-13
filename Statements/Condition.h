#ifndef CONDITION_H
#define CONDITION_H

#include"Statement.h"
//Condition statement class

class Condition : public Statement
{
private:
	Variable *Var2;
	string LHS;	//Left Handside of the Condition (name of a variable)
	string RHS;	//Right Handside (Value)
	string Operator;	//Comparing operator
	double D;
	string v;
	Connector *TConn;	//The connector of the True case of the Condition
	Connector *FConn;	//The connector of the False case of the Condition
	Connector *ResultConn; //virtual connector selects the way in the simulation 
	double width,height;
	Point Inlet;	//A point where connections enters this statement 
	Point TOutlet;	//A point a True connection leaves this statement
	Point FOutlet;	//A point a False connection leaves this statement

	Point Top;	//left corenr of the statement block.

	virtual void UpdateStatementText();

public:
	Condition(Point T, string LeftHS="",string Op="", string RightHS="");

	void setLHS(const string &L);
	void setRHS(const string &R);
	void setOperator(const string &Op);
	bool editRHS(string RHS);
	virtual void setStatConnector(Connector *Conn,int ConnType);
	virtual Connector* getStatConnector(int ConnType);


	virtual void Draw(Output* pOut) const;

	virtual bool IsOnStat(Point P);   // check if the point in th e statement area or not
	virtual Point GetConnPoint(int Order,int connType=0);

	virtual void Edit(Output *pOut,Input *pIn);		//Edit the Statement parameter

	virtual void Move(Output *pOut,ApplicationManager *pManager);		//Move the Statement on the flowchart
	virtual void Resize(char R);		//Resize the Statement

	virtual Statement* Copy(Point P);

	virtual void Save(ofstream &OutFile);	//Save the Statement parameters to a file
	virtual void Load(ifstream &Infile);	//Load the Statement parameters from a file


	virtual void setVar(Variable *V,int i);
	virtual Variable *getVar(int i);

	virtual void GenerateCode(ofstream &OutFile);	//write the statement code to a file
	virtual void Simulate(Output *pOut,Input *pIn);	//Execute the statement in the simulation mode






};

#endif