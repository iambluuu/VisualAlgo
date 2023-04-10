#pragma once

#include "TGUI/TGUI.hpp"

#include "Node.h"

enum signal {
	Pending, Inserting, Removing, Searching
};

struct SinglyLinkedList {
	vector<vector<function<void (int)> > >  action;
	signal Signal;
	Clock timer;

	bool ShowMode;
	bool ShowDirection;

	int CurStep;
	int NodeNumber;
	int Elapsed;
	int Duration;
	Node* Head;
	Node* Tail;
	RectangleShape TextHighlight;

	SinglyLinkedList() {
		Head = nullptr;
		Tail = nullptr;
		Signal = Pending;
		NodeNumber = 0;
		Elapsed = 0;
		Duration = 700;
		CurStep = 0;

		ShowMode = 1;
		ShowDirection = 0;

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

	void initButtons();
	void interactSLL();
	void HandleEvent(Event& e);
	void ClearAction();
	 
	void initList();
	void drawList(int Dummy);
	void drawListPartial(Node* A, Node* B, int Dummy);
	void drawListExcept(Node* ExceptNode, int Dummy);
	void drawArrowFlow( Node* Cur, int Elapsed);

	void changePosition(Node* Cur, float x, float y);
	void NodeAppear( Node* Cur, int Elapsed);
	void drawNode( Node* Cur, int Dummy);
	void setNodeState(Node* Cur, Nodestate NodeState, int Dummy);

	void ChangeState(Node* Cur, Nodestate CurState, Nodestate NextState, int Elapsed);
	void SetNodesNormal(Node* A, Node* B, int Dummy);
	void ConnectNode( Node* A, Node* B, int Elapsed);
	void MoveNode( Node* Cur, float CurX, float CurY, float NxtX, float NxtY, int Elapsed);
	void SlideNodes( Node* Cur, float CurX, float CurY, float NxtX, float NxtY, int Elapsed);

	void genList();
	void genList( const tgui::String s);

	void insertAtBeginning( Node*& NewNode);
	void insertAtEnd(Node*& NewNode);
	bool insertNode(int i, int v);

	bool removeNode(int i);
	void removeAtBeginning();
	void removeAtEnd();

};