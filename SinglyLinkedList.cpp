#include "SinglyLinkedList.h"
#include <math.h>

using namespace std;
using namespace sf;

int SinglyLinkedList::getSize()
{
	return NodeNumber;
}

void SinglyLinkedList::drawList(RenderWindow& app, Node* A, Node* B, Nodestate State, Nodestate ArrowState, int Elapsed)
{
	if (!A)
		return;
	
	std::cout << "2 1 " << A << " " << B << "\n";
	for (Node* tmp = A; tmp != B; tmp = tmp->nxt) {
		std::cout << "2 2 " << tmp << "\n";
		tmp->drawArrow(app, ArrowState);
		tmp->drawNode(app, State, 255);
	}

}

void SinglyLinkedList::initList(RenderWindow& app) {
	int i = 0;

	for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
		tmp->NumberInList = i++;
	}

	drawList(app, Head, nullptr, Normal, Normal, 1);
}

void SinglyLinkedList::genList(RenderWindow& app)
{
	srand(time(0));

	//Release old List
	Node* tmp = Head;

	while (tmp) {
		Node* Dummy = tmp;
		tmp = tmp->nxt;
		delete Dummy;
	}

	//Gen new list

	NodeNumber = rand() % 10;

	while (NodeNumber == 0)
		NodeNumber = rand() % 10;

	Head = new Node(rand() % 100);
	Head->NumberInList = 0;
	Head->changeNodePosition(DefaultPosX, DefaultPosY);
	
	Node* Cur = Head;

	for (int i = 1; i < NodeNumber; i++) {
		Cur->nxt = new Node(rand() % 100);
		Node* tmp = Cur;

		Cur = Cur->nxt;
		Cur->prev = tmp;

		Cur->NumberInList = i;
		
		Cur->changeNodePosition(tmp->Pos.x + 95, tmp->Pos.y);
	}

	
	int Elapsed = 0;
	Clock clock;

	while (Elapsed <= Duration) {
		app.clear(Color::White);

		gui.draw();

		for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
			if (tmp->nxt) {
				int ArrowLength = (int)((Util::DistanceBetweenNodes(tmp->Pos, tmp->nxt->Pos) - 40) * Elapsed / Duration);
				tmp->Arrow.setTextureRect(IntRect(100 - ArrowLength, 0, ArrowLength, 10));
				tmp->drawArrow(app, Normal);
			}

			tmp->drawNode(app, Normal, 255);
		}

		app.display();

		Elapsed = clock.getElapsedTime().asMilliseconds();
	}

}

void SinglyLinkedList::genList(RenderWindow& app, const tgui::String s)
{
	vector<tgui::String> parts = tgui::String(s).split(',', true);

	if (parts.size() == 0)
		return;

	NodeNumber = min((int)parts.size(), maxNodeNumber);

	for (int i = 0; i < parts.size(); i++) {
		int tmp;

		if (!parts[i].attemptToInt(tmp))
			return;
	}
	//Release old List
	Node* tmp = Head;

	while (tmp) {
		Node* Dummy = tmp;
		tmp = tmp->nxt;
		delete Dummy;
	}

	//Gen new list
	Head = new Node(parts[0].toInt());
	Head->NumberInList = 0;
	Head->changeNodePosition(DefaultPosX, DefaultPosY);

	Node* Cur = Head;

	for (int i = 1; i < NodeNumber; i++) {
		Cur->nxt = new Node(parts[i].toInt());
		Node* tmp = Cur;

		Cur = Cur->nxt;
		Cur->prev = tmp;

		Cur->NumberInList = i;
		Cur->changeNodePosition(tmp->Pos.x + 95, tmp->Pos.y);
	}


	int Elapsed = 0;
	Clock clock;

	while (Elapsed <= Duration) {
		app.clear(Color::White);

		gui.draw();

		for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
			if (tmp->nxt) {
				int ArrowLength = (int)((Util::DistanceBetweenNodes(tmp->Pos, tmp->nxt->Pos) - 40) * Elapsed / Duration);
				tmp->Arrow.setTextureRect(IntRect(100 - ArrowLength, 0, ArrowLength, 10));
				tmp->drawArrow(app, Normal);
			}

			tmp->drawNode(app, Normal, 255);
		}

		app.display();

		Elapsed = clock.getElapsedTime().asMilliseconds();
	}

}

void SinglyLinkedList::changeState(RenderWindow& app, Node*& Cur, Nodestate CurState, Nodestate NextState, int Elapsed)
{
	Node* tmp = new Node(Cur->Val);
	tmp->changeNodePosition(Cur->Pos.x, Cur->Pos.y);

	int duration = 300;
	tmp->drawNode(app, NextState, (int)(255 * (double)Elapsed / duration));
	Cur->drawNode(app, CurState, (int)(255 * (1 - (double)Elapsed / duration)));

	delete tmp;
}

void SinglyLinkedList::drawArrowFlow(RenderWindow& app,  Node* Cur, Node* NewNode, int Elapsed)
{
	if (!Cur->nxt)
		return;

	Cur->drawNode(app, Selecting, 255);
	Cur->drawArrow(app, Normal);
	Cur->TmpArrow.setPosition(Cur->Arrow.getPosition());
	Cur->TmpArrow.setRotation(Cur->Arrow.getRotation());

	int ArrowLength = (int)(Util::DistanceBetweenNodes(Cur->Pos, Cur->nxt->Pos) - 40);

	Cur->TmpArrow.setTextureRect(IntRect(100 - ArrowLength, 0, (int)(ArrowLength * (double)Elapsed / Duration), 10));
	app.draw(Cur->TmpArrow);
}

void SinglyLinkedList::ConnectNode(RenderWindow& app,  Node* A, Node* B, int Elapsed)
{
	A->updateArrow(B);

	int Length = (int)((Util::DistanceBetweenNodes(A->Pos, B->Pos) - 40) * Elapsed / Duration);
	A->Arrow.setTextureRect(IntRect(100 - Length, 0, Length, 10));
	A->drawArrow(app, New);
}

void SinglyLinkedList::MoveNodeToList(RenderWindow& app,  Node* A, int Elapsed)
{
	A->changeNodePosition(A->Pos.x, DefaultPosY + 100 * (1 - ((double)Elapsed / Duration)));

	A->drawNode(app, New, 255);
		
	if (A->nxt) {
		A->nxt->changeNodePosition(A->Pos.x + 100 * (double)Elapsed / Duration, DefaultPosY);
		A->nxt->drawNode(app, Next, 255);
		A->nxt->drawArrow(app, Normal);

		for (Node* tmp = A->nxt->nxt; tmp; tmp = tmp->nxt) {
			tmp->changeNodePosition(tmp->prev->Pos.x + 95, DefaultPosY);
		}

		drawList(app, A->nxt->nxt, nullptr, Normal, Normal, 1);
	}
		
}

//void SinglyLinkedList::insertAtBeginning(RenderWindow& app,  Node*& NewNode)
//{
//	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
//
//	PseudoCode->removeAllWidgets();
//	PseudoCode->loadWidgetsFromFile("assets/Description/SLLInsertAtBeginning.txt");
//
//	tgui::TextArea::Ptr Line1 = PseudoCode->get<tgui::TextArea>("Line1");
//	tgui::TextArea::Ptr Line2 = PseudoCode->get<tgui::TextArea>("Line2");
//	tgui::TextArea::Ptr Line3 = PseudoCode->get<tgui::TextArea>("Line3");
//	tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");
//
//	TextHighlight->setVisible(1);
//
//	TextHighlight->setSize(Line1->getSize());
//	TextHighlight->setPosition(Line1->getPosition());
//
//	if (Head) {
//		NewNode->changeNodePosition(DefaultPosX, DefaultPosY + 100);
//		NodeAppear(app,  NewNode);
//
//		NewNode->nxt = Head;
//		Head->prev = NewNode;
//
//		Head->NumberInList++;
//	} else {
//		NewNode->changeNodePosition(DefaultPosX, DefaultPosY);
//
//		NodeAppear(app,  NewNode);
//	}
//		
//	Head = NewNode;
//
//	TextHighlight->setSize(Line2->getSize());
//	TextHighlight->setPosition(Line2->getPosition());
//	if (NewNode->nxt)
//		ConnectNode(app,  NewNode, NewNode->nxt);
//	else
//		Util::Wait();
//
//	TextHighlight->setSize(Line3->getSize());
//	TextHighlight->setPosition(Line3->getPosition());
//	if (NewNode->nxt) {
//		NewNode->NumberInList = 0;
//		MoveNodeToList(app,  NewNode);
//	}
//
//	for (Node* tmp = Head->nxt; tmp; tmp = tmp->nxt)
//		tmp->NumberInList++;
//}

//void SinglyLinkedList::insertAtEnd(RenderWindow& app,  Node* & NewNode)
//{
//	Node* Cur = Head;
//
//	//Run To Node
//	int Elapsed = 0;
//	Clock clock;
//
//	while (Cur && Cur->nxt) {
//		Cur->NodeState = Selecting;
//		
//		drawArrowFlow(app,  Cur);
//
//		Cur->NodeState = Visited;
//		Cur = Cur->nxt;
//	}
//	
//	Cur->NodeState = Selecting;
//	
//	Cur->ArrowState = New;
//	NewNode->changeNodePosition(Cur->Pos.x + 95, DefaultPosY);
//	
//	//NewNode apppears
//	NodeAppear(app,  NewNode);
//	
//	Cur->nxt = NewNode;
//	NewNode->prev = Cur;
//
//	//Connect NewNode
//	NewNode->NumberInList = NodeNumber - 1;
//	ConnectNode(app,  NewNode->prev, NewNode);
//
//
//}

bool SinglyLinkedList::insertNode(RenderWindow& app, int i, int v)
{
	if (i > NodeNumber || i < 0 || NodeNumber == maxNodeNumber)
		return 0;

	initList(app);

	Node* Current = Head;
	Node* NewNode = new Node(v);

	NodeNumber++;

	NewNode->changeNodeValue(v);

	//if (i == 0) {
	//	insertAtBeginning(app, NewNode);
	//	return 1;
	//}

	//if (i == NodeNumber - 1) {
	//	insertAtEnd(app, NewNode);
	//	return 1;
	//}

	//actually insert
	for (int j = 0; j < i - 1; j++) {
		Current = Current->nxt;
	}

	NewNode->prev = Current;
	NewNode->nxt = Current->nxt;

	Current->nxt->prev = NewNode;
	Current->nxt = NewNode;

	int duration = 300;
	Current = Head;

	//Anchor
	Node* Stationary = Current;

	NewNode->nxt->changeNodePosition(Stationary->Pos.x + 95, DefaultPosY);
	Stationary->updateArrow(NewNode->nxt);

	//Run to node
	for (int j = 0; j < i - 1; j++) {
	
		//Switch state to Selecting
		action.push_back(vector<function<void(int)> >());
		action.back().push_back(bind(&drawList, this, app, Head, Current, Visited, Visited, std::placeholders::_1));
		action.back().push_back(bind(&changeState, this, app, Current, Normal, Selecting, std::placeholders::_1));
		action.back().push_back(bind(&drawList, this, app, Current->nxt, NewNode, Normal, Normal, std::placeholders::_1));
		action.back().push_back(bind(&drawList, this, app, NewNode->nxt, nullptr, Normal, Normal, std::placeholders::_1));

		//Flow to next Node
		action.push_back(vector<function<void(int)> >());

		action.back().push_back(std::bind(&drawList, this, app, Head, Current->nxt, Visited, Visited, std::placeholders::_1));		
		action.back().push_back(std::bind(&drawArrowFlow, this, app, Current, Current->nxt, std::placeholders::_1));
		action.back().push_back(std::bind(&drawList, this, app, Current->nxt, NewNode, Normal, Normal, placeholders::_1));
		action.back().push_back(std::bind(&drawList, this, app, NewNode->nxt, nullptr, Normal, Normal, placeholders::_1));

		Current = Current->nxt;
	}

	std::cerr << "Finished Run to node\n";

	//Switch state Aft to Next
	for (double Elapsed = 0.0; (int)Elapsed <= duration; Elapsed += (double)100 / 6) {
		action.push_back([=, &app]() {
			drawList(app, Head, NewNode, Visited, Visited, 1);
			changeState(app, Current, Normal, Next, Elapsed);
			drawList(app, Current, nullptr, Visited, Visited, 1);
		});
	}



	//NewNode appears
	NewNode->changeNodePosition(Current->Pos.x, Current->Pos.y + 100);

	for (double Elapsed = 0.0; (int)Elapsed <= Duration; Elapsed += (double)100 / 6) {
		action.push_back([=, &app]() {
			drawList(app, Head, NewNode, Visited, Visited, 1);
			NewNode->drawNode(app, New, (int)(255 * Elapsed / Duration));
			drawList(app, Current, nullptr, Visited, Visited, 1);
			});
	}


	//Connect NewNode to Aft
	for (double Elapsed = 0.0; (int)Elapsed <= Duration; Elapsed += (double)100 / 6) {
		action.push_back([=, &app]() {
			Stationary->updateArrow(NewNode->nxt);

			drawList(app, Head, NewNode, Visited, Visited, 1);
			NewNode->drawNode(app, New, 255);
			ConnectNode(app, NewNode, Current, Elapsed);
			drawList(app, Current, nullptr, Visited, Visited, 1);
			});
	}

	//Connect Prev to NewNode
	for (double Elapsed = 0.0; (int)Elapsed <= Duration; Elapsed += (double)100 / 6) {
		action.push_back([=, &app]() {
			Stationary->updateArrow(NewNode);

			drawList(app, Head, NewNode, Visited, Visited, 1);

			NewNode->drawNode(app, New, 255);
			ConnectNode(app, NewNode, Current, Elapsed);
			drawList(app, Current, nullptr, Visited, Visited, 1);
			});
	}

	//Move NewNode up

	for (double Elapsed = 0.0; (int)Elapsed <= Duration; Elapsed += (double)100 / 6) {
		action.push_back([=, &app]() {
			MoveNodeToList(app, NewNode, Elapsed);

			drawList(app, Head, nullptr, Visited, Visited, 1);
			});
	}

	return 1;
}

//void SinglyLinkedList::removeAtBeginning(RenderWindow& app) {
//	int Elapsed = 0;
//	Clock clock;
//
//	Node* Cur = Head;
//	Cur->ArrowState = Remove;
//
//	Head = Cur->nxt;
//	if (Head)
//		Head->NumberInList = 0;
//
//	while (Elapsed <= Duration) {
//		app.clear(Color::White);
//		gui.draw();
//
//		drawList(app);
//		Cur->drawNode(app, (int)(255 * (1 - (double)Elapsed / Duration)));
//		Cur->drawArrow(app, (int)(255 * (1 - (double)Elapsed / Duration)));
//		
//
//		app.display();
//		Elapsed = clock.getElapsedTime().asMilliseconds();
// 	}
//
//	Elapsed = 0;
//	clock.restart();
//
//	
//	delete Cur;
//
//	if (Head)
//		Head->prev = nullptr;
//	else
//		return;
//
//	while (Elapsed <= Duration) {
//		app.clear(Color::White);
//		gui.draw();
//
//		Head->changeNodePosition(DefaultPosX + (int)(95 * (1 - (double)Elapsed / Duration)), DefaultPosY);
//		
//		
//		for (Node* tmp = Head->nxt; tmp; tmp = tmp->nxt)
//			tmp->changeNodePosition(tmp->prev->Pos.x + 95, DefaultPosY);
//
//		drawList(app);
//
//		app.display();
//		Elapsed = clock.getElapsedTime().asMilliseconds();
//	}
//
//	return;
//}
//
//void SinglyLinkedList::removeAtEnd(RenderWindow& app,  Node*& Cur) {
//	int Elapsed = 0;
//	Clock clock;
//
//	while (Elapsed <= Duration) {
//		app.clear(Color::White);
//		gui.draw();
//
//		int Length = (int)((Util::DistanceBetweenNodes(Cur->prev->Pos, Cur->Pos) -40) * (1 - (double)Elapsed / Duration));
//		Cur->prev->Arrow.setTextureRect(IntRect(100 - Length, 0, Length, 10));
//
//		for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
//			if (tmp->nxt)
//				tmp->drawArrow(app);
//
//			if (tmp == Cur) 
//				tmp->drawNode(app, (int)(255 * (1 - (double)Elapsed / Duration)));
//			else
//				tmp->drawNode(app, 255);
//
//		}
//
//		app.display();
//		Elapsed = clock.getElapsedTime().asMilliseconds();
//	}
//
//
//	Cur->prev->nxt = nullptr;
//
//	delete Cur;
//
//	return;
//}
//
//bool SinglyLinkedList::removeNode(RenderWindow& app, int i)
//{
//	if (i >= NodeNumber || i < 0)
//		return 0;
//
//	initList(app);
//
//	NodeNumber--;
//
//	//Run to node
//	Node* Cur = Head;
//
//	for (int j = 0; j < i; j++) {
//		Cur->NodeState = Selecting;
//		drawList(app);
//
//		if (j < i - 1) {
//			drawArrowFlow(app,  Cur);
//			Cur->NodeState = Visited;
//		}
//		Cur = Cur->nxt;
//	}
//
//	Node* Dell = Cur;
//	Dell->NumberInList = -1;
//	changeState(app, Dell, Remove);
//
//	if (i == 0) {
//		removeAtBeginning(app);
//		return 1;
//	}
//
//	if (i == NodeNumber) {
//		removeAtEnd(app, Dell);
//		return 1;
//	}
//	
//	//Move Cur down
//	int Elapsed = 0;
//	Clock clock;
//
//	while (Elapsed <= Duration) {
//		app.clear(Color::White);
//		gui.draw();
//
//		Cur->changeNodePosition(Cur->Pos.x, DefaultPosY + (int)(100 * (double)Elapsed / Duration));
//
//		drawList(app);
//
//		Elapsed = clock.getElapsedTime().asMilliseconds();
//		app.display();
//	}
//
//	//Update Next
//	ConnectNode(app,  Cur->prev, Cur->nxt);
//	Cur->prev->nxt = Cur->nxt;
//	Cur->nxt->prev = Cur->prev;
//
//	//Update Nodes position
//	Elapsed = 0;
//	clock.restart();
//
//	Cur->ArrowState = Remove;
//
//	while (Elapsed <= Duration) {
//		app.clear(Color::White);
//		gui.draw();
//
//		Cur->drawArrow(app, (int)(255 * (1 - (double)Elapsed / Duration)));
//
//		Cur->drawNode(app, (int)(255 * (1 - (double)Elapsed / Duration)));
//
//		Cur->nxt->changeNodePosition(Cur->Pos.x + (int)(95 * (1 - (double)Elapsed / Duration)), DefaultPosY);
//		
//		for (Node* tmp = Cur->nxt->nxt; tmp; tmp = tmp->nxt) 
//			tmp->changeNodePosition(tmp->prev->Pos.x + 95, DefaultPosY);
//
//		Cur->changeNodePosition(Cur->Pos.x, Cur->Pos.y);
//
//		drawList(app);
//
//		Elapsed = clock.getElapsedTime().asMilliseconds();
//		app.display();
//	}
//
//	//Delete
//	delete Cur;
//
//	return 1;
//}


void SinglyLinkedList::initButtons(RenderWindow& app)
{
	tgui::Button::Ptr InsertEx = gui.get<tgui::Button>("InsertEx");
	tgui::Button::Ptr InsertButton = gui.get<tgui::Button>("InsertButton");
	tgui::EditBox::Ptr InsertPos = gui.get<tgui::EditBox>("InsertPos");
	tgui::EditBox::Ptr InsertVal = gui.get<tgui::EditBox>("InsertVal");

	tgui::Button::Ptr DeleteEx = gui.get<tgui::Button>("DeleteEx");
	tgui::Button::Ptr DeleteButton = gui.get<tgui::Button>("DeleteButton");
	tgui::EditBox::Ptr DeletePos = gui.get<tgui::EditBox>("DeletePos");

	tgui::Button::Ptr CreateButton = gui.get<tgui::Button>("CreateButton");
	tgui::Button::Ptr RandomGen = gui.get<tgui::Button>("RandomGen");
	tgui::Button::Ptr InputGen = gui.get<tgui::Button>("InputGen");
	tgui::EditBox::Ptr UserInput = gui.get<tgui::EditBox>("EditBox1");
	tgui::Button::Ptr UserInputEx = gui.get<tgui::Button>("UserInputEx");

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");

	tgui::Button::Ptr SlideIn = gui.get<tgui::Button>("SlideIn");
	tgui::Button::Ptr SlideOut = gui.get<tgui::Button>("SlideOut");
	tgui::Panel::Ptr EditPanel = gui.get<tgui::Panel>("EditPanel");
	EditPanel->loadWidgetsFromFile("assets/ControlPanel.txt");

	InsertButton->onPress([=] {
		InsertPos->setVisible(1 - InsertPos->isVisible());
		InsertVal->setVisible(1 - InsertVal->isVisible());
		InsertEx->setVisible(1 - InsertEx->isVisible());
		});

	DeleteButton->onPress([=] {
		DeletePos->setVisible(1 - DeletePos->isVisible());
		DeleteEx->setVisible(1 - DeleteEx->isVisible());

		tgui::String s = DeletePos->getText();

		if (getSize() > 0 && (s.length() == 0 || s.toInt() > getSize() - 1))
			DeletePos->setText(tgui::String(to_string(rand() % getSize())));

		});

	CreateButton->onPress([=] {
		RandomGen->setVisible(1 - RandomGen->isVisible());
		InputGen->setVisible(1 - InputGen->isVisible());
		UserInput->setVisible(1 - UserInput->isVisible());
		UserInputEx->setVisible(1 - UserInputEx->isVisible());
		});

	RandomGen->onPress([&] {
		genList(app);
		});

	UserInputEx->onPress([=, &app] {
		tgui::String s = UserInput->getText();

		genList(app,  s);
		});

	InsertEx->onPress([=, &app]  {
		Signal = INSERT_NODE;

		CurFrame = 0;
		action.clear();

		int Pos = InsertPos->getText().toInt();
		int Val = InsertVal->getText().toInt();

		insertNode(app, Pos, Val);
		});

	DeleteEx->onPress([=, &app] {
		Signal = DELETE_NODE;
		});

	PseudoCode->onMinimize([=] {
		Vector2f MinSiz = PseudoCode->getMinimumSize();
		Vector2f CurSiz = PseudoCode->getSize();
	
		if (CurSiz.x > MinSiz.x || CurSiz.y > MinSiz.y)
			PseudoCode->setSize({ MinSiz.x, MinSiz.y });
		else
			PseudoCode->setSize({ 360, 277.52 });
		});

	PseudoCode->onMaximize([=] {
		Vector2f MaxSiz = PseudoCode->getMaximumSize();
		PseudoCode->setSize({ MaxSiz.x, MaxSiz.y });
		});

	SlideOut->onClick([=] {
		EditPanel->showWithEffect(tgui::ShowAnimationType::SlideFromRight, 500);
		SlideIn->showWithEffect(tgui::ShowAnimationType::SlideFromRight, 500);
		});

	SlideIn->onClick([=] {
		EditPanel->hideWithEffect(tgui::ShowAnimationType::SlideToRight, 500);
		SlideIn->hideWithEffect(tgui::ShowAnimationType::SlideToRight, 500);
		});
}

void SinglyLinkedList::drawAll(RenderWindow& app, int Position)
{

}

void SinglyLinkedList::HandleEvent(RenderWindow& app, Event& e)
{
	CurFrame++;

	switch (Signal) {
	case PENDING:
		drawList(app, Head, nullptr, Normal, Normal, 1);
		break;

	case INSERT_NODE:
		if (CurFrame > (int)action.size()) {
			CurFrame = 0;
			Signal = PENDING;
		}
		else {
			action[CurFrame](CurFrame);
		}

		break;

	}
}

void SinglyLinkedList::interactSLL(RenderWindow& app) 
{
	app.clear();
	gui.draw();

	Event e;

	while (app.pollEvent(e)) {
		if (e.type == Event::Closed) {
			app.close();
			State = EndProgram;
			return;
		}
		gui.handleEvent(e);

	}
	
	HandleEvent(app, e);
	app.display();
}