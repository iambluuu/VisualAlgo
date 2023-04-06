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

	void initButtons(RenderWindow& app);
	void interactSLL(RenderWindow& app);
	 
	void initList(RenderWindow& app);
	void drawList(RenderWindow& app);
	void changeState(RenderWindow& app, Node*& Cur, Nodestate NextState);
	void drawArrowFlow(RenderWindow& app, Node* Cur);

	void NodeDisappear(RenderWindow& app, Node* Cur);
	void NodeAppear(RenderWindow& app, Node* Cur);
	void ConnectNode(RenderWindow& app, Node* A, Node* B);
	void DisconnectNode(RenderWindow& app, Node* A, Node* B);
	void InsertNode(RenderWindow& app, Node* A);

	void genList(RenderWindow& app);
	void genList(RenderWindow& app, const tgui::String s);

	void insertAtBeginning(RenderWindow& app, Node*& NewNode);
	void insertAtEnd(RenderWindow& app, Node*& NewNode);
	bool insertNode(RenderWindow& app,  int i, int v);

	bool removeNode(RenderWindow& app, int i);
	void removeAtBeginning(RenderWindow& app);
	void removeAtEnd(RenderWindow& app, Node*& Cur);

};