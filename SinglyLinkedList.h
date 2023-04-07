#pragma once

#include "TGUI/TGUI.hpp"

#include "Node.h"

enum SIGNAL {
	PENDING, INSERT_NODE, DELETE_NODE, CREATE_LIST, CREATE_LIST_RANDOM
};

struct SinglyLinkedList {
	vector<vector<function<void(int)> > > action;

	SIGNAL Signal;

	int CurFrame;
	int NodeNumber;
	int Duration;
	Node* Head;

	RectangleShape TextHighlight;

	SinglyLinkedList() {
		Signal = PENDING;
		Head = nullptr;

		CurFrame = 0;
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

	void HandleEvent(RenderWindow& app, Event& e);
	void drawAll(RenderWindow& app, int Position);

	int getSize();

	void initButtons(RenderWindow& app);
	void interactSLL(RenderWindow& app);
	 
	void initList(RenderWindow& app);
	void drawList(RenderWindow& app, Node* A, Node* B, Nodestate State, Nodestate ArrowState, int Elapsed);

	void changeState(RenderWindow& app, Node*& Cur, Nodestate CurState, Nodestate NextState, int Elapsed);
	void drawArrowFlow(RenderWindow& app, Node* Cur, Node* NewNode, int Elapsed);

	void ConnectNode(RenderWindow& app, Node* A, Node* B, int Elapsed);

	void MoveNodeToList(RenderWindow& app, Node* A, int Elapsed);

	void genList(RenderWindow& app);
	void genList(RenderWindow& app, const tgui::String s);

	//void insertAtBeginning(RenderWindow& app, Node*& NewNode);
	//void insertAtEnd(RenderWindow& app, Node*& NewNode);
	bool insertNode(RenderWindow& app,  int i, int v);

	bool removeNode(RenderWindow& app, int i);
	void removeAtBeginning(RenderWindow& app);
	void removeAtEnd(RenderWindow& app, Node*& Cur);

};