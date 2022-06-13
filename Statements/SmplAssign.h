#ifndef SMPL_ASSIGN_H
#define SMPL_ASSIGN_H

#include "Statement.h"


//Simple Assignment statement class
//The simple assignment statement assigns a variable to a value
class SmplAssign : public Statement
{
private:
	string LHS;	//Left Handside of the assignment (name of a variable)
	double RHS;	//Right Handside (Value)
	double height ,width;
	Connector *pConn;	//Simple Assignment Stat. has one Connector to next statement

	Point Inlet;	//A point where connections enters this statement 
	Point Outlet;	//A point a connection leaves this statement

	Point LeftCorner;	//left corenr of the statement block.

	virtual void UpdateStatementText();

public:
	SmplAssign(Point Lcorner, string LeftHS="", double RightHS=0);

	void setLHS(const string &L);
	void setRHS(double R);

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

	void GenerateCode(ofstream &OutFile);
	virtual void Simulate(Output *pOut,Input *pIn);	//Execute the statement in the simulation mode
};

#endif