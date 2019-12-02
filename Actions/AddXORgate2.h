#ifndef _ADD_XOR_GATE_H
#define _ADD_XOR_GATE_H

#include "action.h"
#include "..\Components\XOR2.h"

class AddXORgate2 : public Action
{
private:
	//Parameters for rectangular area to be occupied by the gate
	int Cx, Cy;	//Center point of the gate
	//int x1, y1, x2, y2;	//Two corners of the rectangluar area
public:
	AddXORgate2(ApplicationManager* pApp);
	virtual ~AddXORgate2(void);
	ActionType Type = ADD_XOR_GATE_2;
	//Execute action (code depends on action type)
	virtual void Execute();

	


};

#endif