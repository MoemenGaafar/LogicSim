#include "Load.h"
#include "..\ApplicationManager.h"
#include "..\Components\AND2.h"
#include "..\Components\OR2.h"
#include "..\Components\NOR2.h"
#include "..\Components\XOR2.h"
#include "..\Components\XNOR2.h"
#include "..\Components\NAND2.h"
#include "..\Components\INV.h"
#include "..\Components\SWITCH.h"
#include "..\Components\LED.h"
#include "..\Components\CONNECTION.h"
#include "SAVE.h"
#include <chrono>
#include <thread>
#include <iostream>
using namespace std; 



Load::Load(ApplicationManager* pApp) :Action(pApp)
{
}

Load::~Load(void)
{
}

void Load::Execute()
{
	//Get a Pointer to the user Interfaces
	UI* pUI = pManager->GetUI();

	//Print Action Message
	pUI->PrintMsg("Click enter to save current circuit then load, esc to abort action, or backspace to load without saving.\n");

	char key; 
	pUI->getpWind()->WaitKeyPress(key);

	while (key != 13 && key != 27 && key!=8) {
		pUI->getpWind()->WaitKeyPress(key);
	}

	if (key == 27)
		pUI->PrintMsg("Load aborted."); 
	else 
	{
		if (key == 13) {
			Action* LSave = new Save(pManager); 
			LSave->Execute();
			std::this_thread::sleep_for(std::chrono::seconds(4));
		}

		pUI->PrintMsg("LOAD: Type in a file's name with directory. Use 2 backslashes. Click enter when done.\n");

		string name;
		name = pUI->GetString();

		ifstream myfile;
		myfile.open(name.c_str());
		if (myfile.is_open())
		{
			pUI->ClearDrawingArea(); 

			pManager->CompCount = 0; 
			 
			
			//Calculate the rectangle Corners
			int gateWidth = pUI->getGateWidth();
			int gateHeight = pUI->getGateHeight();

			string line;

			int GateCount;
			myfile >> GateCount; //Read number of gates from first line 
			getline(myfile, line);//discard first line 
			
			int *ID= new int[GateCount]; 
			

			string type, label;
			int Identity, Cx = 0, Cy = 0;

			for (int i = 0; i < GateCount; i++) {

				
				getline(myfile, line);
				istringstream a(line);
				a >> type;
				a >> Identity;
				a >> label;
				a >> Cx;
				a >> Cy;
				ID[i] = Identity; 



				GraphicsInfo* pGInfo = new GraphicsInfo(2); //Gfx info to be used to load the gate

				pGInfo->PointsList[0].x = Cx - gateWidth / 2;
				pGInfo->PointsList[0].y = Cy - gateHeight / 2;
				pGInfo->PointsList[1].x = Cx + gateWidth / 2;
				pGInfo->PointsList[1].y = Cy + gateHeight / 2;

				


				if (type == "AND2") {
					pManager->CompList[i] = new AND2(pGInfo, Default_FANOUT);
				}


				if (type == "OR2") {
					pManager->CompList[i] = new OR2(pGInfo, Default_FANOUT);
				}


				if (type == "NOR2") {
					pManager->CompList[i] = new NOR2(pGInfo, Default_FANOUT);
				}


				if (type == "XOR2") {
					pManager->CompList[i] = new XOR2(pGInfo, Default_FANOUT);
				}


				if (type == "XNOR2") {
					pManager->CompList[i] = new XNOR2(pGInfo, Default_FANOUT);
				}


				if (type == "NAND2") {
					pManager->CompList[i] = new NAND2(pGInfo, Default_FANOUT);
				}


				if (type == "LED") {
					pManager->CompList[i] = new LED(pGInfo, Default_FANOUT);
				}


				if (type == "SWITCH") {
					pManager->CompList[i] = new Switch(pGInfo, Default_FANOUT);
				}


				if (type == "NOT") {
					pManager->CompList[i] = new INV(pGInfo, Default_FANOUT);
				}

				pManager->CompList[i]->m_Label = label;
				pManager->CompCount++; 
				if (label!="-")
					pUI->LabelComp(label, pManager->CompList[i]->m_pGfxInfo->PointsList[0].x, pManager->CompList[i]->m_pGfxInfo->PointsList[1].y);
				

			}
			

			int ConnCount;
			myfile >> ConnCount;//Read number of components 
			getline(myfile, line);//Discard number of components 
			

			int S_Comp, T_Comp, P_n;
			for (int i = 0; i < ConnCount; i++) {

				getline(myfile, line);
				istringstream a(line);
				a >> S_Comp;
				a >> T_Comp;
				a >> P_n;
				
				GraphicsInfo* r_GfxInfo = new GraphicsInfo(2);
				OutputPin* pSrcPin;
				InputPin pDstPin;

				int sourceComp;
				int destinationComp;

				for (int j = 0; j < GateCount; j++) {
					if (ID[j] == S_Comp) {

						pSrcPin = pManager->CompList[j]->m_OutputPin;
						sourceComp = j;
						cout << j<< endl; 
						break;
					}
							

				}

				for (int j = 0; j < GateCount; j++) {

					if (ID[j] == T_Comp) {

						pDstPin = pManager->CompList[j]->m_InputPins[P_n];
						destinationComp = j;
						cout << j; 
						break;
					}
				}

				

				//Gfx info of connections	
				Component* comp = pManager->CompList[sourceComp];
				int x1 = comp->m_pGfxInfo->PointsList[0].x;
				int y1 = comp->m_pGfxInfo->PointsList[0].y;
				int x2 = comp->m_pGfxInfo->PointsList[1].x;
				int y2 = comp->m_pGfxInfo->PointsList[1].y;


				switch (comp->ComponentType) {
				case T_AND2:
				case T_OR2:
				case T_NAND2:
				case T_NOR2:
				case T_XOR2:
				case T_XNOR2:
				{
					r_GfxInfo->PointsList[0].x = x2;
					r_GfxInfo->PointsList[0].y = y2 - 25;
				}
				case T_NOT:
				{
					r_GfxInfo->PointsList[0].x = x2 - 1;
					r_GfxInfo->PointsList[0].y = y2 - 24;
				}

				case T_SWITCH:
				{
					r_GfxInfo->PointsList[0].x = x2;
					r_GfxInfo->PointsList[0].y = y2 - 25;
				}
				}



				comp = pManager->CompList[destinationComp];
				x1 = comp->m_pGfxInfo->PointsList[0].x;
				y1 = comp->m_pGfxInfo->PointsList[0].y;
				x2 = comp->m_pGfxInfo->PointsList[1].x;
				y2 = comp->m_pGfxInfo->PointsList[1].y;



				switch (comp->ComponentType) {
				case T_AND2:
				case T_OR2:
				case T_NAND2:
				case T_NOR2:
				case T_XOR2:
				case T_XNOR2:
				{

					if (P_n == 1) {
						r_GfxInfo->PointsList[1].x = x1;
						r_GfxInfo->PointsList[1].y = y1 + 13;
					}
					else
					{
						r_GfxInfo->PointsList[1].x = x1;
						r_GfxInfo->PointsList[1].y = y2 - 13;
					}
				}
				case T_NOT: {


					r_GfxInfo->PointsList[1].x = x1;
					r_GfxInfo->PointsList[1].y = y1 + 26;

				}
				case T_LED:
				{
					r_GfxInfo->PointsList[1].x = x1 + 15;
					r_GfxInfo->PointsList[1].y = y2 - 8;
				}
				}



				//Create Connection
				pManager->CompList[GateCount + i] = new Connection(r_GfxInfo, pSrcPin, pDstPin);
				pManager->CompCount++;
				
				
			}

			cout << pManager->CompCount; 
		}

		else
			pUI->PrintMsg("Error opening file.\n");

	} 
}



