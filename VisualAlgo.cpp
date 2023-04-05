#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"

#include "SinglyLinkedList.h"

using namespace std;
using namespace sf;

int main()
{
	RenderWindow app(VideoMode(1600, 900), "VisualAlgo", Style::Close);

	app.setFramerateLimit(120);

	tgui::Gui gui(app);

	SinglyLinkedList A;
	//A.genList(app, gui);
	
	while (app.isOpen()) {
		switch (State) {
		case SLL:
			gui.removeAllWidgets();
			gui.loadWidgetsFromFile("assets/SLLGui.txt");
			A.initButtons(app, gui);

			while (State == SLL) 
				A.interactSLL(app, gui);
			
			break;
		case DLL:
			app.clear();
			app.display();
			gui.removeAllWidgets();
			cout << "DLL\n";

			break;
		case CLL:
			cout << "CLL\n";
			break;
		case Stack:
			cout << "Stack\n";
			break;
		case Queue:
			cout << "Queue\n";
			break;
		case DArray:
			cout << "Dynamic Array\n";
			break;
		case Array:
			cout << "Array\n";
			break;
		}
	}
	
	
}
