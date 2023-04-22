#pragma once
#include "ArrayMem.h"

struct DArray {
	vector<ArrayMem*> Arr;
	vector<ArrayMem*> DummyArr;

	vector<vector<function<void(int)> > >  action;
	signal Signal;
	Clock timer;

	int Last;
	int Duration;
	int Elapsed;

	int InsertModes;
	int DeleteModes;
	int GenModes;

	int DefaultPosX;
	int DefaultPosY;

	int CurStep;
	bool ShowMode;
	bool ShowDirection;

	int maxCap;
	int size;
	int capacity;

	DArray() {
		DefaultPosX = 250;
		DefaultPosY = 250;

		timer.restart();
		Signal = Pending;

		Elapsed = 0;
		Duration = 700;
		Last = 0;

		InsertModes = 0;
		DeleteModes = 0;
		GenModes = 0;

		CurStep = 1;
		ShowDirection = 0;
		ShowMode = 1;

		for (int i = 0; i < 12; i++) {
			ArrayMem* Mem = new ArrayMem(i);
			Mem->changeMemPosition(DefaultPosX + 95 * (i - 1), DefaultPosY);
			DummyArr.push_back(Mem);
		}

		for (int i = 0; i < 12; i++) {
			ArrayMem* Mem = new ArrayMem(i);
			Mem->changeMemPosition(DefaultPosX + 95 * (i - 1), DefaultPosY);
			Arr.push_back(Mem);
		}

		maxCap = 12;

		size = 0;
		capacity = 0;
	}

	~DArray() {
		Arr.clear();
	}
	
	void genList();
	void genList(tgui::String s);

	void drawSize();
	void drawCapacity();

	void setArrNormal(vector<ArrayMem*> &A, int Dummy);
	void ChangeState(ArrayMem* Cur, bool CurState, bool NextState, int Elapsed);
	void ChangeValue(ArrayMem* Cur, sf::String preVal, sf::String nextVal, int Elapsed);
	void ArrAppear(vector<ArrayMem*> &A, int l, int r, int Elapsed);
	void ArrDisappear(vector<ArrayMem*> &A, int l, int r, int Elapsed);
	void drawArr(vector<ArrayMem*> &A, int l, int r, int Dummy);
	void MoveArrDown(int Elapsed);

	void shrinkArray();
	void growArray();
	void pushBack(int v);
	void insertAt(int i, int v);
	void popBack();
	void deleteAt(int i)  ;

	void ClearAction();
	void reset();
	void initProgress();
	void initButtons();
	void HandleEvent(Event& e);
	void interactDArr();
};