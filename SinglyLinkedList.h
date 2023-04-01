#pragma once

#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"

#include "Global.h"
#include "Node.h"
#include "Buttons.h"

struct SinglyLinkedList {
	int NodeNumber;
	Node* Head;

	SinglyLinkedList() {
		Head = nullptr;
		NodeNumber = 0;
	}

	~SinglyLinkedList() {
		while (Head) {
			Node* tmp = Head->nxt;
			delete Head;
			Head = tmp;
		}
	}

	void initButtons(RenderWindow& app, tgui::Gui& gui);
	void interactSLL(RenderWindow& app, tgui::Gui& gui);
	 
	void initList(RenderWindow& app);
	void drawList(RenderWindow& app);
	int getInput(tgui::Gui& gui);

	void genList(RenderWindow& app, tgui::Gui& gui);

	bool insertNode(RenderWindow& app, tgui::Gui& gui, int i, int v);
	void insertAtEnd(RenderWindow& app, tgui::Gui& gui, Node*& NewNode);

	bool removeNode(RenderWindow& app, tgui::Gui& gui, int i);
	void removeAtBeginning(RenderWindow& app, tgui::Gui& gui);
	void removeAtEnd(RenderWindow& app, tgui::Gui& gui, Node*& Cur);

};