#ifndef END_H
#define END_H

#include "Statement.h"


class End : public Statement
{
private:


	Point Inlet;	//A point where connections enters this statement 
	double height ,width;
	Point Center;	//Center of the statement block.

public:
	End(Point center);
	virtual void Move(Output *pOut,ApplicationManager *pManager);

	virtual void Draw(Output* pOut) const;
	virtual void setStatConnector(Connector *Conn,int ConnType);
	virtual Connector* getStatConnector(int ConnType);
	virtual bool IsOnStat(Point P);
	virtual Point GetConnPoint(int Order,int connType=0);
	virtual void Edit(Output *pOut,Input *pIn);
	virtual void GenerateCode(ofstream &OutFile);
	virtual void Save(ofstream &OutFile);	//Save the Statement parameters to a file
	virtual void Load(ifstream &Infile);	//Load the Statement parameters from a file
	virtual void Resize(char R);


};

#endif