#ifndef START_H
#define START_H

#include "Statement.h"

class Start : public Statement
{
private:

	Connector *pConn;	//Start Stat. has one Connector to next statement

	Point Outlet;	//A point a connection leaves this statement
	double height ,width;
	Point Center;	//Center of the statement block.

public:
	Start(Point center);

	virtual void setStatConnector(Connector *Conn,int ConnType);
	virtual Connector* getStatConnector(int ConnType);

	virtual void Move(Output *pOut,ApplicationManager *pManager);

	virtual void Resize(char R);
	virtual void Draw(Output* pOut) const;
	virtual bool IsOnStat(Point P);
	virtual Point GetConnPoint(int Order,int connType=0);
	virtual void Edit(Output *pOut,Input *pIn);
	virtual void GenerateCode(ofstream &OutFile);
	virtual void Save(ofstream &OutFile);	//Save the Statement parameters to a file
	virtual void Load(ifstream &Infile);	//Load the Statement parameters from a file



};

#endif