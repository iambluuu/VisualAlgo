#pragma once

#include "TGUI/TGUI.hpp"

#include "Node.h"

struct SinglyLinkedList {
	int NodeNumber;
	int Duration;
	Node* Head;
	RectangleShape TextHighlight;

	SinglyLinkedList() {
		Head = nullptr;
		NodeNumber = 0;
		Duration = 700;

		TextHighlight.setFillColor(Color(0,0,0,0));
	}

	~SinglyLinkedList() {
		while (Head) {
			Node* tmp = Head->nxt;
			delete Head;
			Head = tmp;
		}
	}


	int getSize();

	void initButtons(RenderWindow& app, tgui::Gui& gui);
	void interactSLL(RenderWindow& app, tgui::Gui& gui);
	 
	void initList(RenderWindow& app);
	void drawList(RenderWindow& app);
	void changeState(RenderWindow& app, tgui::Gui& gui, Node*& Cur, Nodestate NextState);
	void drawArrowFlow(RenderWindow& app, tgui::Gui& gui, Node* Cur);

	void NodeAppear(RenderWindow& app, tgui::Gui& gui, Node* Cur);
	void ConnectNode(RenderWindow& app, tgui::Gui& gui, Node* A, Node* B);
	void InsertNode(RenderWindow& app, tgui::Gui& gui, Node* A);

	void genList(RenderWindow& app, tgui::Gui& gui);
	void genList(RenderWindow& app, tgui::Gui& gui, const tgui::String s);

	void insertAtBeginning(RenderWindow& app, tgui::Gui& gui, Node*& NewNode);
	void insertAtEnd(RenderWindow& app, tgui::Gui& gui, Node*& NewNode);
	bool insertNode(RenderWindow& app, tgui::Gui& gui, int i, int v);

	bool removeNode(RenderWindow& app, tgui::Gui& gui, int i);
	void removeAtBeginning(RenderWindow& app, tgui::Gui& gui);
	void removeAtEnd(RenderWindow& app, tgui::Gui& gui, Node*& Cur);

};