#ifndef VAR_ASSIGN_H
#define VAR_ASSIGN_H

#include "Statement.h"
//Variable Assignment statement class
//The variable assignment statement assigns a variable to a Variable
class VarAssign : public Statement
{
private:
	string LHS;	//Left Handside of the assignment (name of a variable)
	string RHS;	//Right Handside (name of a variable)
	double height ,width;
	Connector *pConn;	//Variable Assignment Stat. has one Connector to next statement
	Variable *Var2;
	Point Inlet;	//A point where connections enters this statement 
	Point Outlet;	//A point a connection leaves this statement

	Point LeftCorner;	//left corenr of the statement block.

	virtual void UpdateStatementText();

public:
	VarAssign(Point Lcorner, string LeftHS="", string RightHS="");

	void setLHS(const string &L);
	void setRHS(const string &R);

	virtual void setStatConnector(Connector *Conn,int ConnType);
	virtual Connector* getStatConnector(int ConnType);

	void Edit(Output *pOut,Input *pIn) ;
	virtual	void Resize(char R);
	virtual void Move(Output *pOut,ApplicationManager *pManager);
	virtual void Draw(Output* pOut) const;
	virtual Point GetConnPoint(int Order,int connType=0);
	virtual bool IsOnStat(Point P);
	virtual Statement* Copy(Point P);
	virtual void Save(ofstream &OutFile);
	virtual void Load(ifstream &Infile);

	virtual void setVar(Variable *V,int i);
	virtual Variable *getVar(int i);

	void GenerateCode(ofstream &OutFile);
	virtual void Simulate(Output *pOut,Input *pIn);	//Execute the statement in the simulation mode


};

#endif