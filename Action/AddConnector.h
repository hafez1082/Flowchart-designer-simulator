#ifndef ADD_CONNECTOR_H
#define ADD_CONNECTOR_H

#include "Action.h"
#include "..\Statements\Connector.h"

//Add Connector Action
//This class is responsible for 
// 1 - Getting the first stat. coordinates from the user
// 2 - Getting the second stat. coordinates from the user
// 3 - Creating an object of Connector class and fill it parameters
// 4 - Adding the created object to the list of connectors
class AddConnector : public Action
{
private:
	Point Position;	//Position where the user clicks to add the stat.
	Point Start; //The start point of the connector
	Point End;	//The end point of the connector
	Statement *SrcStat;	//the source statement of the connector
	Statement *DstStat;	//the destination statement of the connector
	int ConnType; //connection type
public:
	AddConnector(ApplicationManager *pAppManager);

	//Read Connector parameters
	virtual void ReadActionParameters();

	//Create and add an connector to the list of connector
	virtual void Execute() ;

};

#endif