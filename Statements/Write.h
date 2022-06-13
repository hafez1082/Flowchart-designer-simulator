#ifndef WRITE_H
#define WRITE_H
#include "Statement.h"


class Write : public Statement
{

private:
	bool IsVar;
	double value;
	string Line;
	double width;
	double height;
	Connector *pConn;	//Simple Assignment Stat. has one Connector to next statement

	Point Inlet;	//A point where connections enters this statement
	Point Outlet;	//A point a connection leaves this statement

	Point LeftCorner;	//left corenr of the statement block.


	virtual void UpdateStatementText();


public:




	///The following functions should be supported by the Statement class
	///It should be overridden by each inherited Statement
	///Decide the parameters that you should pass to each function	

	Write(Point Lcorner, string Variable="");
	void setVar(const string &Variables);

	virtual void setStatConnector(Connector *Conn,int ConnType);
	virtual Connector* getStatConnector(int ConnType);

	virtual void Draw(Output* pOut) const;	//Draw the statement

	virtual bool IsOnStat(Point P);
	virtual Point GetConnPoint(int Order,int connType=0);

	virtual void Edit(Output *pOut,Input *pIn);		//Edit the Statement parameter
	virtual void Move(Output *pOut,ApplicationManager *pManager);		//Move the Statement on the flowchart
	virtual void Resize(char R);
	virtual Statement* Copy(Point P);	

	virtual void Save(ofstream &OutFile);	//Save the Statement parameters to a file
	virtual void Load(ifstream &Infile);	//Load the Statement parameters from a file



	virtual void GenerateCode(ofstream &OutFile);	//write the statement code to a file
	virtual void Simulate(Output *pOut,Input *pIn);	//Execute the statement in the simulation mode



};


#endif