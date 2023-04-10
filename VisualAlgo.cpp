#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"

#include "Global.h"
#include "SinglyLinkedList.h"

using namespace std;
using namespace sf;

int main()
{

	app.setFramerateLimit(120);

	gui.setTarget(app);

	SLL A;
	//A.genList(app, gui);
	
	while (app.isOpen()) {
		switch (State) {
		case _SLL:
			gui.removeAllWidgets();
			gui.loadWidgetsFromFile("assets/SLLGui.txt");
			A.initButtons();

			while (State == _SLL) 
				A.interactSLL();
			
			break;
		case _DLL:
			app.clear();
			app.display();
			gui.removeAllWidgets();
			cout << "DLL\n";

			break;
		case _CLL:
			cout << "CLL\n";
			break;
		case _Stack:
			cout << "Stack\n";
			break;
		case _Queue:
			cout << "Queue\n";
			break;
		case _DArray:
			cout << "Dynamic Array\n";
			break;
		case _Array:
			cout << "Array\n";
			break;
		}
	}
	
	
}
