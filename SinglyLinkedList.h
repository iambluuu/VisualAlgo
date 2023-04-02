#pragma once

#include "TGUI/TGUI.hpp"

#include "Node.h"

struct SinglyLinkedList {
	int NodeNumber;
	int Duration;
	Node* Head;

	SinglyLinkedList() {
		Head = nullptr;
		NodeNumber = 0;
		Duration = 700;
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
	void drawArrowFlow(RenderWindow& app, tgui::Gui& gui, Node* Cur);

	void genList(RenderWindow& app, tgui::Gui& gui);
	void genList(RenderWindow& app, tgui::Gui& gui, const tgui::String s);

	bool insertNode(RenderWindow& app, tgui::Gui& gui, int i, int v);
	void insertAtEnd(RenderWindow& app, tgui::Gui& gui, Node*& NewNode);

	bool removeNode(RenderWindow& app, tgui::Gui& gui, int i);
	void removeAtBeginning(RenderWindow& app, tgui::Gui& gui);
	void removeAtEnd(RenderWindow& app, tgui::Gui& gui, Node*& Cur);

};