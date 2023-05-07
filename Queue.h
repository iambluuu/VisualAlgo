#pragma once

#include "TGUI/TGUI.hpp"

#include "Node.h"

struct Queue {
	float DefaultPosX;
	float DefaultPosY;
	int maxNodeNumber;

	vector<vector<function<void(int)> > >  action;
	signal Signal;
	Clock timer;

	bool ShowMode;
	bool ShowDirection;
	int Last;

	int InsertModes;
	int DeleteModes;
	int GenModes;

	int CurStep;
	int NodeNumber;
	int Elapsed;
	int Duration;
	Node* Head;
	Node* Tail;

	Queue() {
		DefaultPosX = 250;
		DefaultPosY = 250;
		maxNodeNumber = 12;

		Head = nullptr;
		Tail = nullptr;
		Signal = Pending;
		NodeNumber = 0;
		Elapsed = 0;
		Duration = 700;
		CurStep = 0;
		Last = 0;

		InsertModes = 0;
		DeleteModes = 0;
		GenModes = 0;

		ShowMode = 1;
		ShowDirection = 0;
	}

	~Queue() {
		while (Head) {
			Node* tmp = Head->nxt;
			delete Head;
			Head = tmp;
		}
	}


	int getSize();

	void initButtons();
	void initProgress();
	void interactQueue();
	void HandleEvent(Event& e);
	void ClearAction();

	void initList();
	void drawList(int Dummy);
	void drawTitle(Node* Cur, String s, int Dummy);
	void drawListPartial(Node* A, Node* B, int Dummy);
	void drawListExcept(Node* ExceptNode, int Dummy);
	void drawArrowFlow(Node* Cur, int Elapsed);

	void changePosition(Node* Cur, float x, float y);
	void NodeAppear(Node* Cur, int Elapsed);
	void NodeDisappear(Node* Cur, int Elapsed);
	void TitleAppear(Node* Cur, String s, int Elapsed);
	void TitleDisappear(Node* Cur, String s, int Elapsed);
	void HighlightAppear(int Line, int Elapsed);
	void MoveHighlight(int CurLine, int NextLine, int Elapsed);
	void drawNode(Node* Cur, int Dummy);
	void setNodeState(Node* Cur, Nodestate NodeState, int Dummy);

	void ChangeState(Node* Cur, Nodestate CurState, Nodestate NextState, int Elapsed);
	void ChangeValue(Node* Cur, sf::String preVal, sf::String nextVal, int Elapsed);
	void SetNodesNormal(Node* A, Node* B, int Dummy);
	void ConnectNode(Node* A, Node* B, int Elapsed);
	void DisconnectNode(Node* A, Node* B, int Elapsed);
	void MoveNode(Node* Cur, float CurX, float CurY, float NxtX, float NxtY, int Elapsed);
	void SlideNodes(Node* Cur, float CurX, float CurY, float NxtX, float NxtY, int Elapsed);

	void genList();
	void genList(const tgui::String s);

	void ClearList(int Dummy);
	void popAll();

	void insertAtEnd(Node*& NewNode);
	bool insertNode(int i, int v);

	bool removeNode(int i);
	void removeAtBeginning();
	void DeleteNode(Node*& Cur, int Dummy);

	void Peek();
};