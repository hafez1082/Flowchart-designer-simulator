#include "ApplicationManager.h"
#include "Actions\AddSmplAssign.h"
#include "Actions\AddVarAssign.h"
#include "Actions\AddSOAssign.h"
#include "Actions\AddStart.h"
#include "Actions\AddEnd.h"
#include "Actions\AddRead.h"
#include "Actions\AddWrite.h"
#include "Actions\AddCondition.h"
#include "Actions\AddConnector.h"
#include "Actions\Select.h"
#include "Actions\Load.h"
#include "Actions\Save.h"
#include "Actions\Move.h"
#include "Actions\Run.h"
#include "Actions\Step.h"
#include "Actions\Generate.h"
#include "GUI\Input.h"
#include "GUI\Output.h"

//Constructor
ApplicationManager::ApplicationManager()
{
	//Create Input and output
	pOut = new Output;
	pIn = pOut->CreateInput();
	StatID=1;
	StatCount = 0;
	ConnCount = 0;
	VarCount=0;
	pSelectedStat = NULL;	//no Statement is selected yet
	pSelectedConn=NULL;
	pCopied=NULL;
	editedDesign=false;
	//Create an array of Statement pointers and set them to NULL		
	for(int i=0; i<MaxCount; i++)
	{
		StatList[i] = NULL;	
		ConnList[i] = NULL;
		VarList[i]	= NULL;
	}
	for(int j=0;j<10000;j++)
	{
		SaveArr[j]="U"+to_string(long double(j+1));
		SaveArr[j]=SaveArr[j]+".txt";
		string s="Undo\\"+SaveArr[j];
		remove(s.c_str());
	}
	GL=0,plus=0;
}


//==================================================================================//
//								Actions Related Functions							//
//==================================================================================//
ActionType ApplicationManager::GetUserAction()
{
	//Ask the input to get the action from the user.
	return pIn->GetUserAction(P);		
}
////////////////////////////////////////////////////////////////////////////////////
//Creates an action and executes it
void ApplicationManager::ExecuteAction(ActionType ActType) 
{
	Action* pAct = NULL;

	//According to ActioType, create the corresponding action object
	switch (ActType)
	{
	case ADD_START :
		pAct= new AddStart(this);
		break;

	case ADD_END :
		pAct=new AddEnd(this);
		break;

	case ADD_SMPL_ASSIGN:
		pAct = new AddSmplAssign(this);
		break;

	case ADD_VAR_ASSIGN :
		pAct=new AddVarAssign(this);
		break;

	case ADD_SINGLE_OPERATOR_ASSIGN :
		pAct=new AddSOAssign(this);
		break;

	case ADD_CONDITION:
		pAct= new AddCondition(this);
		break;
	case ADD_READ_STAT :
		pAct= new AddRead(this);
		break;

	case ADD_WRITE_STAT :
		pAct= new AddWrite(this);
		break;

	case ADD_CONNECTOR:
		pAct= new AddConnector(this); 
		break;

	case SIM_MODE:
		SwitchToSim();
		break;

	case MOVE:
		pAct=new Move(this);
		break;


	case SAVE:
		pAct= new Save(this);
		break;

	case LOAD:
		pAct=new Load(this);
		break;

	case NEW:
		while(GetStatList(0))
			DeleteStat(GetStatList(0));
		UndoRedo();
		break;

	case UNDO:
		UndoRedo(1);
		break;

	case RUN:
		pAct=new Run(this);
		break;

	case STEP:
		pAct=new Step(this);
		break;


	case GENERATE:
		pAct=new Generate(this);
		break;


	case SELECT:
		pAct=new Select(this);
		break;

	case STATUS:
		UnSelect();
		return;

	case DSN_TOOL:
		UnSelect();
		break;


	case SIM_TOOL:
		UnSelect();
		break;

	case DSN_MODE:
		pOut->PrintMessage("Design Mode");
		pOut->CreateDesignToolBar();
		break;


	case EXIT:				
		onExit();
		break;
	}


	//Execute the created action
	if(pAct != NULL)
	{
		pAct->Execute();//Execute
		delete pAct;	//Action is not needed any more ==> delete it
	}
}



//==================================================================================//
//						Statements Management Functions								//
//==================================================================================//
int ApplicationManager::GetStatCount()
{
	return StatCount;
}
int ApplicationManager::GetConnCount()
{
	return ConnCount;
}

int ApplicationManager::GetVarCount()
{
	return VarCount;
}




//Add a statement to the list of statements
void ApplicationManager::AddStatement(Statement *pStat)
{

	if(pStat->GetStatType()=="STRT" || pStat->GetStatType()=="END")
		for(int i=0; i<StatCount; i++)
			if(StatList[i]->GetStatType()==pStat->GetStatType())
			{
				pOut->PrintMessage("Action in not accessible:"+pStat->GetStatType()+" Statement already exists");
				return;
			}



			if(StatCount < MaxCount)
			{
				StatList[StatCount++] = pStat;
				if(pStat->GetID()==0)
					StatList[StatCount-1]->SetID(StatID++);

			}
}

////////////////////////////////////////////////////////////////////////////////////
Statement *ApplicationManager::GetStatement(Point P) const
{

	for(int i=0;i<StatCount;i++)
		if(StatList[i]->IsOnStat(P))
			return StatList[i];

	return NULL;
}

void ApplicationManager::DeleteStat (Statement* Stat)
{if(Stat!=NULL)
{
	int ID = Stat->GetID();

	//********************* Deleting Out-Connectors *************************//
	//If Stat has 1 out-connector (The statement is not END or COND)
	if(Stat->GetStatType()=="VAR"||Stat->GetStatType()=="SNGLOP"||Stat->GetStatType()=="COND")
	{
		deleteVariable(Stat->getVar(0));
		deleteVariable(Stat->getVar(1));
	}
	else deleteVariable(Stat->getVar(0));

	if(Stat->GetStatType() != "END" && Stat->GetStatType() != "COND")	DeleteConn (Stat->getStatConnector(0));

	//If Stat is COND (has 2 out-connectors)
	else if(Stat->GetStatType() == "COND"){
		DeleteConn (Stat->getStatConnector(1));
		DeleteConn (Stat->getStatConnector(2));
	}

	//----------------------------------------------------------------------//
	//********************* Deleting In-Connectors **************************//
	int count=0;
	for (int i = 0 ; i< ConnCount ; i++)
		if (ConnList[i]->getDstID() == ID)	count++;
	while(count!=0)
		for (int i = 0 ; i< ConnCount ; i++)
			if (ConnList[i]->getDstID() == ID)	{DeleteConn (ConnList[i]); count--;}


			int i;
			for (i = 0 ; i < StatCount ; i++) if (StatList[i] == Stat) break;	//Get its order in link
			for (i ; i < StatCount ; i++) StatList [i] = StatList [i+1];
			StatList [StatCount-1] = NULL;
			StatCount--;



}
//---------------------------------------------------------------------//
}


void ApplicationManager::SetConnector(Connector *pConn)
{
	if(ConnCount < MaxCount)
		ConnList[ConnCount++] = pConn;



}

////////////////////////////////////////////////////////////////////////////////////
Connector *ApplicationManager::GetConnector(Point P) const
{

	for(int i=0;i<ConnCount;i++)
		if(ConnList[i]->IsOnConnect(P))
			return ConnList[i];

	return NULL;
}



////////////////////////////////////////////////////////////////////////////////////

void ApplicationManager::EditConnectors(Statement *Stat)
{
	for (int i = 0 ; i< ConnCount ; i++)
		if (ConnList[i]->getDstID() ==Stat->GetID())	
		{
			ConnList[i]->setEndPoint(Stat->GetConnPoint(2));
		}


}

void ApplicationManager::DeleteConn (Connector* Conn)
{
	if (Conn != NULL){	//If statement has this connector

		Conn->getSrcStat()->setStatConnector(NULL,Conn->getConnType());	//Set pointer of connector's tail statement to NULL		

		int i;
		for (i = 0 ; i < ConnCount ; i++) if (ConnList[i] == Conn) break;	//Get its order in link

		// int i = Conn - ConnList[i]
		//Shift connector list backwards from i to the end (deleting the desired connector)
		for (i ; i < ConnCount ; i++) ConnList [i] = ConnList [i+1];
		ConnList [ConnCount-1] = NULL;
		ConnCount--;
	}
}


Variable* ApplicationManager::AddVariable(Variable *V)
{
	if(V->name=="")
		return NULL;
	for(int i=0;i<VarCount;i++)
		if(VarList[i]->name==V->name)
		{ VarList[i]->repeat++; return VarList[i];}

		VarList[VarCount++]=V;
		return NULL;
}

Variable *ApplicationManager::GetVariable(int j) const
{
	return VarList[j];
}

void ApplicationManager::deleteVariable(Variable *V)
{if(VarCount==0)
return;
int i;
for(i=0;i<VarCount;i++)
	if(VarList[i]==V) break;
if(i==VarCount)
	return;
if(VarList[i]->repeat==1)
{
	for(i;i<VarCount;i++)
		VarList[i]=VarList[i+1];

	VarList[VarCount-1]=NULL;
	VarCount--;
}


}


void ApplicationManager::InitializeVar()
{
	for(int i=0;i<VarCount;i++)
		VarList[i]->value=0;


}

//Returns the selected statement
Statement *ApplicationManager::GetSelectedStatement() const
{	return pSelectedStat;	}

////////////////////////////////////////////////////////////////////////////////////
//Set the statement selected by the user
void ApplicationManager::SetSelectedStatement(Statement *pStat)
{	pSelectedStat = pStat;	}

Connector *ApplicationManager::GetSelectedSConnector() const
{return pSelectedConn;}

void ApplicationManager::SetSelectedConnector(Connector *pConn)
{pSelectedConn=pConn;}



Statement* ApplicationManager::GetStatList(int j)
{
	return StatList[j];
}
Connector* ApplicationManager::GetConnList(int j)
{
	return ConnList[j];
}


void ApplicationManager::SetCopied(Statement* P)
{
	pCopied=P;
}


Statement* ApplicationManager::GetCopied()
{
	return pCopied;
}

void ApplicationManager::SetCopyType(int x)
{
	CopyType=x;	
}
int ApplicationManager::GetCopyType()
{
	return CopyType;
}


void ApplicationManager::UnSelect()
{if(GetSelectedStatement()!=NULL)
{

	GetSelectedStatement()->SetSelected(false);
	SetSelectedStatement(NULL);

}
else if(GetSelectedSConnector()!=NULL)
{
	GetSelectedSConnector()->SetSelected(false);
	SetSelectedConnector(NULL);

}

UpdateInterface();

}

Point ApplicationManager::getPoint()
{
	return P;
}

bool ApplicationManager::onBars(Point P)
{
	if(P.x>=0&&P.x<=UI.width)
		if(P.y>=0&&P.y<=UI.TlBrWdth)
			return true;
	if(P.x>=0&&P.x<=UI.MnItWdth)
		if(P.y>=UI.TlBrWdth&&P.y<=(UI.height-UI.StBrWdth))
			return true;
	return false;
}

bool ApplicationManager::onObject(Point P)
{
	if(GetStatement(P)!=NULL)
		return true;
	if(GetConnector(P)!=NULL)
		return true;
	return false;

}

void ApplicationManager::setEditedDesign(bool E)
{
	editedDesign=E;
}

bool ApplicationManager::getEditedDesign()
{
	return editedDesign;
}

void ApplicationManager::onExit()
{
	if(getEditedDesign())
	{	
		pOut->DrawExitMsg();
		Point Position;
		while(1)
		{
			pIn->GetPointClicked(Position);
			if(Position.x>=(UI.width/2-150)+49.28&&Position.x<=(UI.width/2-150)+133.72&&Position.y>=(UI.height/2-75)+74.66&&Position.y<=(UI.height/2-75)+104.32)	//yes button
			{Action* pAct=new Save(this);	pAct->Execute(); break;}
			else if(Position.x>=(UI.width/2-150)+169.66&&Position.x<=(UI.width/2-150)+253.38&&Position.y>=(UI.height/2-75)+74.38&&Position.y<=(UI.height/2-75)+103.63)		//no button
			{
				while(GetStatList(0))
					DeleteStat(GetStatList(0));
				break;
			}
		}
	}
}

void ApplicationManager::UndoRedo(int n)
{
	Statement *Stat=NULL;
	Connector *Conn=NULL;
	Variable *V;
	int l=0;
	if(n==0)
	{
		ofstream ufile;
		if(GL==0)
		{
			ufile.open("Undo\\"+SaveArr[GL]);
			ufile<<""<<endl;
			ufile.close();
			GL++;
		}
		plus=GL;
		ufile.open("Undo\\"+SaveArr[GL]);
		ufile<<GetVarCount()<<endl;
		for(int i=0;i<GetVarCount();i++)
			ufile<<GetVariable(i)->name<<"		";
		ufile<<endl<<GetStatCount()<<endl;
		for(int i=0;i<GetStatCount();i++)
		{
			Stat=GetStatList(i);
			Stat->Save(ufile);
		}
		ufile<<GetConnCount()<<endl;
		for(int i=0;i<GetConnCount();i++)
		{
			Conn=GetConnList(i);
			Conn->Save(ufile);
		}
		GL++;
		ufile.close();
	}
	else 
	{
		if(n==1)
			l=plus-1;
		else l=plus+1;
		if(n==1 && l<0)
		{
			return;
		}
		else if(n==2 && l==GL )
		{
			return;
		}
		string line,S;
		int x,y,z;
		Point P; P.x=200; P.y=200;
		while(GetStatList(0))
			DeleteStat(GetStatList(0));
		ifstream ifile("Undo\\"+SaveArr[l]);
		if (ifile.is_open())
		{
			ifile>>x;
			for(int i=0;i<x;i++)
			{
				V=new Variable;
				ifile>>V->name;
				AddVariable(V);
			}
			ifile>>y;

			for(int j=0;j<y;j++)
			{
				ifile>>line;
				if(line=="STRT")
				{
					Stat=new Start(P);
				}
				else if(line=="COND")
				{
					Stat=new Condition(P);
				}
				else if(line=="END")
				{
					Stat=new End(P);
				}
				else if(line=="SMPL")
				{
					Stat=new SmplAssign(P);
				}
				else if(line=="READ")
				{
					Stat=new Read(P);
				}
				else if(line=="WRITE")
				{
					Stat=new Write(P);
				}
				else if(line=="SNGLOP")
				{
					Stat=new SOAssign(P);
				}
				else if(line=="VAR")
				{
					Stat=new VarAssign(P);
				}
				Stat->Load(ifile);
				AddStatement(Stat);
			}
			ifile>>z;
			for(int i=0;i<z;i++)
			{
				Conn=new Connector(NULL,NULL);
				Conn->Load(ifile);
				SetConnector(Conn);
			}
			LoadModify();
			if(n==1)
			{
				plus--;
			}
			else 
			{
				plus++;
			}

			ifile.close();
		}
	}
}


void ApplicationManager::LoadModify()
{
	for(int i=0;i<ConnCount;i++)
	{
		for(int j=0;j<StatCount;j++)
		{
			if(ConnList[i]->getSrsID()==StatList[j]->GetID())
			{
				StatList[j]->setStatConnector(ConnList[i],ConnList[i]->getConnType());
				ConnList[i]->setStartPoint(StatList[j]->GetConnPoint(1,ConnList[i]->getConnType()));
				ConnList[i]->setSrcStat(StatList[j]);
			}

			if(ConnList[i]->getDstID()==StatList[j]->GetID())
			{
				ConnList[i]->setEndPoint(StatList[j]->GetConnPoint(2,ConnList[i]->getConnType()));
				ConnList[i]->setDstStat(StatList[j]);
			}

		}
	}

	int ID; bool right=false;
	if(StatList[0])
	{
		ID=StatList[0]->GetID();
		right=true;
	}
	for(int i=1;i<StatCount;i++)
		if(StatList[i]->GetID()>ID)
			ID=StatList[i]->GetID();
	if(right)
		StatID=ID+1;

	for(int i=0;i<VarCount;i++)
	{
		for(int j=0;j<StatCount;j++)
		{
			if(StatList[j]->getVar(0)->name==VarList[i]->name)
				StatList[j]->setVar(VarList[i],0);

			if(StatList[j]->GetStatType()=="COND"||StatList[j]->GetStatType()=="VAR"||StatList[j]->GetStatType()=="SNGLOP")
				if(StatList[j]->getVar(1)->name==VarList[i]->name)
					StatList[j]->setVar(VarList[i],1);
		}
	}
}


void ApplicationManager::SwitchToSim()
{int count=0;
for(int i=0;i<StatCount;i++)
	if(StatList[i]->GetStatType()=="STRT"||StatList[i]->GetStatType()=="END")
		count++;

if(count!=2)
{pOut->PrintMessage("the design is incomplete the program can't switch to simulation mode"); return;}
pOut->PrintMessage("Simulation mode");
pOut->CreateSimulationToolBar();
UnSelect();

}

//==================================================================================//
//							Interface Management Functions							//
//==================================================================================//

//Draw all figures on the user interface
void ApplicationManager::UpdateInterface() const
{	pOut->ClearDrawArea();

//Draw all connections
for(int i=0; i<ConnCount; i++)
	ConnList[i]->Draw(pOut);


//Draw all statements
for(int i=0; i<StatCount; i++)
	StatList[i]->Draw(pOut);


if(UI.AppMode==DESIGN)
	pOut->CreateDesignToolBar();
else if(UI.AppMode==SIMULATION)
	pOut->CreateSimulationToolBar();


}
////////////////////////////////////////////////////////////////////////////////////
//Return a pointer to the input
Input *ApplicationManager::GetInput() const
{	return pIn; }
//Return a pointer to the output
Output *ApplicationManager::GetOutput() const
{	return pOut; }
////////////////////////////////////////////////////////////////////////////////////


//Destructor
ApplicationManager::~ApplicationManager()
{
	for(int i=0; i<StatCount; i++)
		delete StatList[i];
	for(int i=0; i<StatCount; i++)
		delete ConnList[i];
	for(int i=0; i<VarCount; i++)
		delete VarList[i];
	delete pIn;
	delete pOut;

}
