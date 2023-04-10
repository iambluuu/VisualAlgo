#include "SinglyLinkedList.h"
#include <math.h>
#include <functional>

using namespace std;
using namespace sf;

int SLL::getSize()
{
	return NodeNumber;
}

void SLL::changePosition(Node* Cur, float x, float y)
{
	Cur->changeNodePosition(x, y);
}

void SLL::NodeAppear(Node* Cur, int Elapsed)
{
	Cur->drawNode((int)(255 * (double)Elapsed / Duration));
}

void SLL::drawNode(Node * Cur, int Dummy)
{
	Cur->drawNode(255);
}

void SLL::setNodeState(Node* Cur, Nodestate NodeState, int Dummy)
{
	if (!Cur)
		return;

	if (NodeState == Visited)
		Cur->ArrowState = Visited;

	if (NodeState == New)
		Cur->ArrowState = New;

	Cur->NodeState = NodeState;
}

void SLL::MoveNode( Node* Cur, float CurX, float CurY, float NxtX, float NxtY, int Elapsed)
{
	Cur->changeNodePosition(CurX + (NxtX - CurX) * (double)Elapsed / Duration, CurY + (NxtY - CurY) * (double)Elapsed / Duration);
}

void SLL::SlideNodes( Node* Cur, float CurX, float CurY, float NxtX, float NxtY, int Elapsed)
{
	MoveNode(Cur, CurX, CurY, NxtX, NxtY, Elapsed);

	for (Node* tmp = Cur->nxt; tmp; tmp = tmp->nxt) 
		tmp->changeNodePosition(tmp->prev->Pos.x + 95, tmp->prev->Pos.y);

}

void SLL::SetNodesNormal(Node* A, Node* B, int Dummy)
{
	for (Node* tmp = A; tmp != B; tmp = tmp->nxt) {
		tmp->ArrowState = Normal;
		//tmp->NodeState = Normal;
	}
	
	B->ArrowState = Normal;
}

void SLL::ClearAction()
{
	for (int i = 0; i < action.size(); i++)
		for (int j = 0; j < action[i].size(); j++)
			action[i][j](Duration);

	while (!action.empty())
		action.pop_back();
}

void SLL::drawList(int Dummy)
{
	if (Head)
		app.draw(Head->Title);

	for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
		tmp->updateArrow(tmp->nxt);
		tmp->drawArrow();
		tmp->drawNode(255);
	}
}

void SLL::drawTitle(Node* Cur, Nodestate NodeState, int Elapsed)
{
	if (!Cur)
		return;

	Cur->Title.setFillColor(Color(229, 9, 20, (int)(255 * (double)Elapsed / Duration)));
	switch (NodeState) {
	case Normal:
		if (Cur->NumberInList == 0)
			Cur->Title.setString(String("Head"));
		else
			Cur->Title.setString(String(""));

		break;

	case Visited:
		if (Cur->NumberInList == 0)
			Cur->Title.setString(String("Head"));
		else
			Cur->Title.setString(String(""));

		break;

	case Selecting:
		if (Cur->NumberInList == 0)
			Cur->Title.setString(String("Cur/Head"));
		else
			Cur->Title.setString(String("Cur/") + String(to_string(Cur->NumberInList)));
		break;

	case New:

		if (Cur->NumberInList == 0)
			Cur->Title.setString(String("New/Head"));
		else if (Cur->NumberInList > 0)
			Cur->Title.setString(String("New/") + String(to_string(Cur->NumberInList)));
		else
			Cur->Title.setString(String("New"));
		break;

	case Remove:

		break;

	case Next:
		if (Cur->NumberInList == 0)
			Cur->Title.setString(String("Head"));
		else
			Cur->Title.setString(String("Aft"));

		break;
	}
	app.draw(Cur->Title);
}

void SLL::drawListPartial(Node* A, Node* B, int Dummy)
{
	if (!A || !B) 
		return;

	for (Node* tmp = A; tmp != B; tmp = tmp->nxt) {
		tmp->updateArrow(tmp->nxt);
		tmp->drawArrow();
		tmp->drawNode(255);
	}

	if (B)
		B->drawNode(255);
}

void SLL::drawListExcept(Node* ExceptNode, int Dummy) 
{
	for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
		if (tmp == ExceptNode)
			continue;

		if (tmp->nxt) {
			if (tmp->nxt == ExceptNode && ExceptNode->nxt) {
				tmp->updateArrow(ExceptNode->nxt);
				tmp->drawArrow();
			}
			else if (tmp->nxt != ExceptNode)
				tmp->drawArrow();
		}

		tmp->drawNode(255);
	}
}

void SLL::initList() {
	int i = 0;

	for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
		tmp->NumberInList = i++;

		tmp->NodeState = Normal;
		tmp->ArrowState = Normal;
	}

	drawList(1);
}

void SLL::genList()
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

		Tail = Cur;
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
				tmp->drawArrow();
			}

			tmp->drawNode(255);		
		}

		app.display();

		Elapsed = clock.getElapsedTime().asMilliseconds();
	}

}

void SLL::genList( const tgui::String s)
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

		Tail = Cur;
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
				tmp->drawArrow();
			}

			tmp->drawNode(255);
		}

		app.display();

		Elapsed = clock.getElapsedTime().asMilliseconds();
	}

}

void SLL::ChangeState(Node* Cur, Nodestate CurState, Nodestate NextState, int Elapsed)
{
	if (!Cur)
		return;

	Node* tmp = new Node(Cur->Val);
	tmp->Title.setFillColor(Color(0, 0, 0, 0));

	tmp->NodeState = NextState;
	tmp->changeNodePosition(Cur->Pos.x, Cur->Pos.y);

	tmp->drawNode((int)(255 * (double)Elapsed / Duration));
	Cur->drawNode((int)(255 * (1 - (double)Elapsed / Duration)));

	if ((double)Elapsed / Duration >= 4.0 / 5) {
		Cur->NodeState = NextState;
		if (NextState == Visited)
			Cur->ArrowState = Visited;
	}
	else {
		Cur->NodeState = CurState;
	}

	delete tmp;
}

void SLL::drawArrowFlow(Node* Cur, int Elapsed)
{
	if (!Cur->nxt)
		return;

	Cur->TmpArrow.setPosition(Cur->Arrow.getPosition());
	Cur->TmpArrow.setRotation(Cur->Arrow.getRotation());

	int ArrowLength = (int)(Util::DistanceBetweenNodes(Cur->Pos, Cur->nxt->Pos) - 40);

	Cur->TmpArrow.setTextureRect(IntRect(100 - ArrowLength, 0, (int)(ArrowLength * (double)Elapsed / Duration), 10));
	app.draw(Cur->TmpArrow);
	
}

void SLL::ConnectNode( Node* A, Node* B, int Elapsed)
{
	if (!B)
		return;

	A->updateArrow(B);
	A->ArrowState = New;

	int Length = (int)((Util::DistanceBetweenNodes(A->Pos, B->Pos) - 40) * Elapsed / Duration);
	A->Arrow.setTextureRect(IntRect(100 - Length, 0, Length, 10));
	A->drawArrow();

}

//void SLL::InsertNode(Node* A)
//{
//	int Elapsed = 0;
//	Clock clock;
//
//	while (Elapsed <= Duration) {
//		app.clear(Color::White);
//		gui.draw();
//
//		drawList(app);
//
//		app.display();
//
//		A->changeNodePosition(A->Pos.x, DefaultPosY + 100 * (1 - ((double)Elapsed / Duration)));
//		
//		if (A->nxt) {
//			A->nxt->changeNodePosition(A->Pos.x + 100 * (double)Elapsed / Duration, DefaultPosY);
//
//			for (Node* tmp = A->nxt->nxt; tmp; tmp = tmp->nxt)
//				tmp->changeNodePosition(tmp->prev->Pos.x + 95, DefaultPosY);
//		}
//		
//
//		Elapsed = clock.getElapsedTime().asMilliseconds();
//	}
//}

void SLL::insertAtBeginning(Node*& NewNode)
{
	//tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");

	//PseudoCode->removeAllWidgets();
	//PseudoCode->loadWidgetsFromFile("assets/Description/SLLInsertAtBeginning.txt");

	//tgui::TextArea::Ptr Line1 = PseudoCode->get<tgui::TextArea>("Line1");
	//tgui::TextArea::Ptr Line2 = PseudoCode->get<tgui::TextArea>("Line2");
	//tgui::TextArea::Ptr Line3 = PseudoCode->get<tgui::TextArea>("Line3");
	//tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");

	//TextHighlight->setVisible(1);

	//TextHighlight->setSize(Line1->getSize());
	//TextHighlight->setPosition(Line1->getPosition());

	NewNode->nxt = Head;
	if (Head)
		Head->prev = NewNode;

	Head = NewNode;

	for (Node* tmp = NewNode->nxt; tmp; tmp = tmp->nxt)
		tmp->NumberInList++;

	if (NewNode->nxt) {
		changePosition(NewNode, DefaultPosX, DefaultPosY + 100);
		changePosition(NewNode->nxt, DefaultPosX, DefaultPosY);
	} else {
		changePosition(NewNode, DefaultPosX, DefaultPosY);
	}
	
	//NewNode appears
	action.push_back(vector<function<void(int) > >());
	action.back().push_back(std::bind(&SLL::drawListExcept, this, NewNode, std::placeholders::_1));
	action.back().push_back(bind(&SLL::drawTitle, this, NewNode, New, placeholders::_1));
	action.back().push_back(std::bind(&SLL::NodeAppear, this, NewNode, std::placeholders::_1));
	
	//TextHighlight->setSize(Line2->getSize());
	//TextHighlight->setPosition(Line2->getPosition());
	
	//Connect NewNode to Head
	action.push_back(vector<function<void(int) > > ());
	action.back().push_back(std::bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(std::bind(&SLL::drawNode, this, NewNode, placeholders::_1));
	action.back().push_back(std::bind(&SLL::ConnectNode, this,NewNode, NewNode->nxt, placeholders::_1));

	//TextHighlight->setSize(Line3->getSize());
	//TextHighlight->setPosition(Line3->getPosition());
	if (NewNode->nxt) {
		action.push_back(vector<function<void(int) > >());
		action.back().push_back(std::bind(&SLL::MoveNode, this, NewNode, DefaultPosX, DefaultPosY + 100, DefaultPosX, DefaultPosY, placeholders::_1));
		action.back().push_back(std::bind(&SLL::SlideNodes, this,NewNode->nxt, DefaultPosX, DefaultPosY, DefaultPosX + 95, DefaultPosY, placeholders::_1));
		action.back().push_back(std::bind(&SLL::drawList, this, placeholders::_1));
	}
}

void SLL::insertAtEnd(Node* & NewNode)
{
	Node* Cur = Head;

	while (Cur->nxt) {
		Cur = Cur->nxt;
	}

	Cur->nxt = NewNode;
	NewNode->prev = Cur;
	Tail = NewNode;
	NewNode->changeNodePosition(Cur->Pos.x + 95, Cur->Pos.y);

	
	Cur = Head;
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Cur, Normal, Selecting, placeholders::_1));


	while (Cur->nxt != NewNode) {

		action.push_back(vector<function<void(int)> >());
		action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur, Selecting, placeholders::_1));

		action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
		action.back().push_back(bind(&SLL::drawArrowFlow, this, Cur, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur, Selecting, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur->nxt, Normal, Selecting, placeholders::_1));

		Cur = Cur->nxt;
	}

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, Cur, Selecting, placeholders::_1));

	action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(bind(&SLL::NodeAppear, this, NewNode, placeholders::_1));

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::ConnectNode, this, NewNode->prev, NewNode, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(std::bind(&SLL::drawNode, this, NewNode, placeholders::_1));
	
}

bool SLL::insertNode( int i, int v)
{
	if (i > NodeNumber || i < 0 || NodeNumber == maxNodeNumber)
		return 0;

	initList();

	Node* Cur = Head;
	Node* NewNode = new Node(v);
	NewNode->NumberInList = i;

	NodeNumber++;

	NewNode->changeNodeValue(v);
	NewNode->NodeState = New;
	NewNode->ArrowState = New;

	if (i == 0) {
		insertAtBeginning(NewNode);
		return 1;
	}

	if (i == NodeNumber - 1) {
		insertAtEnd(NewNode);
		return 1;
	}

	for (int j = 0; j < i - 1; j++) 
		Cur = Cur->nxt;
	
	if (Cur->nxt) {
		Cur->nxt->prev = NewNode;
		NewNode->nxt = Cur->nxt;
	}
	Cur->nxt = NewNode;
	NewNode->prev = Cur;
	NewNode->changeNodePosition(Cur->Pos.x + 95, Cur->Pos.y + 100);

	for (Node* tmp = NewNode->nxt; tmp; tmp = tmp->nxt)
		tmp->NumberInList++;

	////tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");

	////PseudoCode->removeAllWidgets();
	////PseudoCode->loadWidgetsFromFile("assets/Description/SLLInsert.txt");

	////tgui::TextArea::Ptr Line1 = PseudoCode->get<tgui::TextArea>("Line1");

	////tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");
	////TextHighlight->setVisible(1);

	////int Height = TextHighlight->getSize().y;

	//run to node
	Cur = Head;
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(bind(&SLL::drawTitle, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Cur, Normal, Selecting, placeholders::_1));


	while (Cur->nxt != NewNode) {
		action.push_back(vector<function<void(int)> >());
			action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur, Selecting, placeholders::_1));
	
		action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
		action.back().push_back(bind(&SLL::drawArrowFlow, this, Cur, placeholders::_1));
		action.back().push_back(bind(&SLL::drawTitle, this, Cur->nxt, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur, Selecting, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur->nxt, Normal, Selecting, placeholders::_1));

		Cur = Cur->nxt;
	}

	//Change Aft State
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));

	action.back().push_back(bind(&SLL::drawTitle, this, NewNode->nxt, Next, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, NewNode->nxt, Normal, Next, placeholders::_1));

	//NewNode appears
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::setNodeState, this, NewNode, New, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, NewNode->nxt, Next, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(bind(&SLL::NodeAppear, this, NewNode, placeholders::_1));
		
	//Connect Nodes	
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));

	action.back().push_back(bind(&SLL::setNodeState, this, NewNode, New, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(bind(&SLL::ConnectNode, this, NewNode, NewNode->nxt, placeholders::_1));
	action.back().push_back(bind(&SLL::drawNode, this, NewNode, placeholders::_1));

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));

	action.back().push_back(bind(&SLL::setNodeState, this, NewNode, New, placeholders::_1));
	action.back().push_back(bind(&SLL::ConnectNode, this, Cur, NewNode, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListPartial, this, Head, NewNode->prev, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListPartial, this, NewNode, Tail, placeholders::_1));
	action.back().push_back(bind(&SLL::drawNode, this, NewNode, placeholders::_1));
	
	//Move NewNode up
	action.push_back(vector<function<void(int) > >());
	action.back().push_back(bind(&SLL::setNodeState, this, NewNode, New, placeholders::_1));
	action.back().push_back(std::bind(&SLL::MoveNode, this, NewNode, Cur->Pos.x + 95, Cur->Pos.y + 100, Cur->Pos.x + 95, Cur->Pos.y, placeholders::_1));
	action.back().push_back(std::bind(&SLL::SlideNodes, this, NewNode->nxt, Cur->Pos.x + 95, Cur->Pos.y, Cur->Pos.x + 95 + 95, DefaultPosY, placeholders::_1));
	action.back().push_back(std::bind(&SLL::drawList, this, placeholders::_1));

	return 1;
}

//void SLL::removeAtBeginning(RenderWindow& app) {
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
//void SLL::removeAtEnd(  Node*& Cur) {
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

//bool SLL::removeNode( int i)
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


void SLL::initButtons()
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

	RandomGen->onPress([=] {
		ClearAction();
		Signal = Pending;
		genList();
		});

	UserInputEx->onPress([=] {
		ClearAction();
		Signal = Pending;
		tgui::String s = UserInput->getText();

		genList(s);
		});

	InsertEx->onPress([=] {
		if (NodeNumber == maxNodeNumber)
		return;

		Signal = Inserting;
		timer.restart();
	
		ClearAction();

		CurStep = 0;

		int Pos = InsertPos->getText().toInt();
		int Val = InsertVal->getText().toInt();

		insertNode(Pos, Val);
		});

	DeleteEx->onPress([=] {
		if (NodeNumber == 0)
		return;

		ClearAction();

		int Pos = DeletePos->getText().toInt();

		//removeNode(Pos);
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

void SLL::HandleEvent(Event& e)
{

	switch (Signal) {
	case Pending:
		drawList(1);
		break;
	case Inserting:
		if (ShowMode == 0) {
			if (Elapsed >= Duration) {
				CurStep++;
				timer.restart();
				Elapsed = 0;
			}

			if (CurStep >= action.size()) {
				Signal = Pending;
				drawList(1);
				break;
			}

			for (int i = 0; i < action[CurStep].size(); i++) {
				action[CurStep][i](Elapsed);
			}
		}

		if (ShowMode == 1) {

			if (e.type == Event::KeyPressed) {

				switch (e.key.code) {
				case Keyboard::Right:
					if (ShowDirection == 1) {
						ShowDirection = 0;
						Elapsed = (Duration - Elapsed);
						timer.restart();
					}
					else {
						if (CurStep + 1 == (int)action.size()) {
							Elapsed = Duration;
						}
						else {
							for (int i = 0; i < (int)action[CurStep].size(); i++)
								action[CurStep][i](Duration);

							CurStep++;

							Elapsed = 0;
					
							timer.restart();
						}
					}

					break;

				case Keyboard::Left:
					if (ShowDirection == 0) {
						ShowDirection = 1;
						Elapsed = (Duration - Elapsed);
						timer.restart();
					}
					else {
						if (CurStep == 0) {
							Elapsed = 0;
						}
						else {
							for (int i = 0; i < (int)action[CurStep].size(); i++)
								action[CurStep][i](0);

							CurStep--;
							Elapsed = Duration;
							timer.restart();
						}
					}

					break;
				default:
					break;
				}
			}
		}

		break;
	}
}


void SLL::interactSLL()
{
	app.clear();
	gui.draw();
	Event e;

	Elapsed = timer.getElapsedTime().asMilliseconds();

	if (ShowDirection == 1) 
		Elapsed = max(0, Duration - Elapsed);
	else
		Elapsed = min(Duration, Elapsed);

	while (app.pollEvent(e)) {
		if (e.type == Event::Closed) {
			app.close();
			State = _EndProgram;
			return;
		}
		HandleEvent(e);
		gui.handleEvent(e);
		
	}

	switch (Signal) {
	case Pending:
		drawList(1);
		break;

	default:
		for (int i = 0; i < (int)action[CurStep].size(); i++) {
			action[CurStep][i](Elapsed);
		}

		break;
	
	}

	app.display();
}