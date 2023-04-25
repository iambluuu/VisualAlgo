#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"

#include "Global.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"
#include "CircularLinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "Array.h"
#include "DynamicArray.h"

using namespace std;
using namespace sf;

int main()
{
	app.setFramerateLimit(120);
	gui.setTarget(app);
	theme.load("assets/themes/CyberPurple.txt");
	ThemeNum = 0;

	SLL A;
	Stack B;
	Queue C;
	DLL D;
	CLL E;
	Array F;
	DArray G;
	
	while (app.isOpen()) {
		switch (State) {
		case _SLL:
			gui.removeAllWidgets();
			gui.loadWidgetsFromFile("assets/themes/SLLGui.txt");
			A.Signal = Pending;
			A.NodeNumber = 0;
			A.ShowMode = 1;
			A.initButtons();

			while (State == _SLL) 
				A.interactSLL();
			
			break;
		case _DLList:
			gui.removeAllWidgets();
			gui.loadWidgetsFromFile("assets/themes/SLLGui.txt");
			D.Signal = Pending;
			D.NodeNumber = 0;
			D.ShowMode = 1;
			D.initButtons();
			
			while (State == _DLList)
				D.interactDLL();

			break;
		case _CLL:
			gui.removeAllWidgets();
			gui.loadWidgetsFromFile("assets/themes/SLLGui.txt");
			E.Signal = Pending;
			E.NodeNumber = 0;
			E.ShowMode = 1;
			E.initButtons();

			while (State == _CLL)
				E.interactCLL();

			cout << "CLL\n";
			break;
		case _Stack:
			gui.removeAllWidgets();
			gui.loadWidgetsFromFile("assets/themes/StackGui.txt");
			B.Signal = Pending;
			B.NodeNumber = 0;
			B.ShowMode = 1;
			B.initButtons();

			while (State == _Stack)
				B.interactStack();

			break;
		case _Queue:
			gui.removeAllWidgets();
			gui.loadWidgetsFromFile("assets/themes/QueueGui.txt");
			C.Signal = Pending;
			C.NodeNumber = 0;
			C.ShowMode = 1;

			C.initButtons();

			while (State == _Queue)
				C.interactQueue();

			break;
		case _DArray:
			gui.removeAllWidgets();
			gui.loadWidgetsFromFile("assets/themes/DArrayGui.txt");
			G.Signal = Pending;
			G.size = 0;
			G.capacity = 0;
			G.ShowMode = 1;
			G.initButtons();

			while (State == _DArray)
				G.interactDArr();

			cout << "Dynamic Array\n";
			break;
		case _Array:
			gui.removeAllWidgets();
			gui.loadWidgetsFromFile("assets/themes/DArrayGui.txt");
			F.Signal = Pending;
			F.size = 0;
			F.capacity = 0;
			F.ShowMode = 1;
			F.initButtons();

			while (State == _Array)
				F.interactArr();

			cout << "Array\n";
			break;
		}
	}
	
	
}
