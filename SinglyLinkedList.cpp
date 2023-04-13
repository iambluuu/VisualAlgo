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

void SLL::NodeDisappear(Node* Cur, int Elapsed)
{
	NodeAppear(Cur, Duration - Elapsed);
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
	if (!Cur)
		return;

	Cur->changeNodePosition(CurX + (NxtX - CurX) * (double)Elapsed / Duration, CurY + (NxtY - CurY) * (double)Elapsed / Duration);
}

void SLL::SlideNodes( Node* Cur, float CurX, float CurY, float NxtX, float NxtY, int Elapsed)
{
	if (!Cur)
		return;

	MoveNode(Cur, CurX, CurY, NxtX, NxtY, Elapsed);

	for (Node* tmp = Cur->nxt; tmp; tmp = tmp->nxt) 
		tmp->changeNodePosition(tmp->prev->Pos.x + 95, tmp->prev->Pos.y);

}

void SLL::SetNodesNormal(Node* A, Node* B, int Dummy)
{
	if (!A || !B)
		return;

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
	//if (Head)
	//	app.draw(Head->Title);

	for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
		tmp->updateArrow(tmp->nxt);
		tmp->drawArrow();
		tmp->drawNode(255);
	}
}

void SLL::TitleAppear(Node* Cur, Nodestate NodeState, int Elapsed)
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

void SLL::TitleDisappear(Node* Cur, Nodestate NodeState, int Elapsed)
{
	TitleAppear(Cur, NodeState, Duration - Elapsed);
}

void SLL::HighlightAppear(int Elapsed)
{
	if (Elapsed >= Duration / 2)
		Elapsed = Duration / 2;

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");

	TextHighlight->setPosition({ 0, 4 });
	TextHighlight->setVisible(1);
	TextHighlight->setInheritedOpacity(2 * (float)Elapsed / Duration);
}

void SLL::MoveHighlight(int CurLine, int NextLine, int Elapsed)
{
	if (Elapsed >= Duration / 2)
		Elapsed = Duration / 2;

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");

	int Distance = (int)(2 * (double)Elapsed / Duration * (NextLine - CurLine) * 26);
	
	TextHighlight->setPosition({ 0, 4 + CurLine * 26 + Distance });
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

void SLL::DeleteNode(Node*& Cur, int Dummy)
{
	if (Cur->prev) {
		Cur->prev->nxt = Cur->nxt;
	}

	if (Cur->nxt) {
		Cur->nxt->prev = Cur->prev;
	}

	if (Cur == Head)
		Head = Cur->nxt;

	delete Cur;
}

void SLL::initList() {
	int i = 0;

	for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
		tmp->NumberInList = i++;

		tmp->NodeState = Normal;
		tmp->ArrowState = Normal;
		
		if (!tmp->nxt)
			Tail = tmp;
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
	}
	else {
		Cur->NodeState = CurState;
	}

	delete tmp;
}

void SLL::ChangeValue(Node* Cur, sf::String preVal, sf::String nextVal, int Elapsed)
{
	if (!Cur)
		return;

	Node* tmp = new Node(nextVal);
	tmp->Title.setFillColor(Color(0, 0, 0, 0));

	tmp->NodeState = Cur->NodeState;
	tmp->changeNodePosition(Cur->Pos.x, Cur->Pos.y);

	tmp->drawNode((int)(255 * (double)Elapsed / Duration));
	Cur->drawNode((int)(255 * (1 - (double)Elapsed / Duration)));

	if ((double)Elapsed / Duration >= 4.0 / 5) {
		Cur->changeNodeValue(nextVal);
	}
	else {
		Cur->changeNodeValue(preVal);
	}

	delete tmp;
}

void SLL::drawArrowFlow(Node* Cur, int Elapsed)
{
	if (!Cur->nxt)
		return;

	Cur->TmpArrow.setPosition(Cur->Arrow.getPosition());
	Cur->TmpArrow.setRotation(Cur->Arrow.getRotation());

	int ArrowLength = (int)(Util::DistanceBetweenNodes(Cur->Pos, Cur->nxt->Pos) - 46);

	Cur->TmpArrow.setTextureRect(IntRect(100 - ArrowLength, 0, (int)(ArrowLength * (double)Elapsed / Duration), 10));
	app.draw(Cur->TmpArrow);
	
	if ((double)Elapsed / Duration >= 4.5 / 5) {
			Cur->ArrowState = Visited;
	}
}

void SLL::ConnectNode( Node* A, Node* B, int Elapsed)
{
	if (!B)
		return;

	A->updateArrow(B);
	A->ArrowState = New;

	int Length = (int)((Util::DistanceBetweenNodes(A->Pos, B->Pos) - 46) * Elapsed / Duration);
	A->Arrow.setTextureRect(IntRect(100 - Length, 0, Length, 10));
	A->drawArrow();

}

void SLL::DisconnectNode(Node* A, Node* B, int Elapsed)
{
	if (!B)
		return;

	Elapsed = Duration - Elapsed;

	A->updateArrow(B);
	A->ArrowState = Remove;

	int Length = (int)((Util::DistanceBetweenNodes(A->Pos, B->Pos) - 46) * Elapsed / Duration);
	A->Arrow.setTextureRect(IntRect(100 - Length, 0, Length, 10));
	A->drawArrow();
}

void SLL::insertAtBeginning(Node*& NewNode)
{
	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("Node NewNode = new Node(v)\nNewNode.next = Head\nHead = NewNode"));

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

	action.back().push_back(bind(&SLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(std::bind(&SLL::drawListExcept, this, NewNode, std::placeholders::_1));
	action.back().push_back(std::bind(&SLL::NodeAppear, this, NewNode, std::placeholders::_1));

	if (!Head->nxt) {
		action.push_back(vector<function<void(int) > >());
		
		action.back().push_back(std::bind(&SLL::drawList, this, placeholders::_1));
		action.back().push_back(std::bind(&SLL::MoveHighlight, this, 0, 2, placeholders::_1));
		action.back().push_back(std::bind(&SLL::TitleAppear, this, NewNode, New, placeholders::_1));
		return;
	}
	
	//TextHighlight->setSize(Line2->getSize());
	//TextHighlight->setPosition(Line2->getPosition());
	
	//Connect NewNode to Head
	action.push_back(vector<function<void(int) > > ());

	action.back().push_back(std::bind(&SLL::MoveHighlight, this, 0, 1, placeholders::_1));
	action.back().push_back(std::bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(std::bind(&SLL::drawNode, this, NewNode, placeholders::_1));
	action.back().push_back(std::bind(&SLL::ConnectNode, this,NewNode, NewNode->nxt, placeholders::_1));

	//TextHighlight->setSize(Line3->getSize());
	//TextHighlight->setPosition(Line3->getPosition());
	if (NewNode->nxt) {
		action.push_back(vector<function<void(int) > >());
		action.back().push_back(std::bind(&SLL::MoveHighlight, this, 1, 2, placeholders::_1));
		action.back().push_back(std::bind(&SLL::TitleAppear, this, NewNode, New, placeholders::_1));
		action.back().push_back(std::bind(&SLL::MoveNode, this, NewNode, DefaultPosX, DefaultPosY + 100, DefaultPosX, DefaultPosY, placeholders::_1));
		action.back().push_back(std::bind(&SLL::SlideNodes, this,NewNode->nxt, DefaultPosX, DefaultPosY, DefaultPosX + 95, DefaultPosY, placeholders::_1));
		action.back().push_back(std::bind(&SLL::drawList, this, placeholders::_1));
	}
}

void SLL::insertAtEnd(Node* & NewNode)
{
	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("Node Cur = Head\nfor (k = 0; k < n - 1; k++)\n	Cur = Cur.next\nCur.next = new Node(v)"));

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
	action.back().push_back(bind(&SLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(bind(&SLL::TitleAppear, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Cur, Normal, Selecting, placeholders::_1));


	while (Cur->nxt != NewNode) {

		action.push_back(vector<function<void(int)> >());

		if (Cur == Head)
			action.back().push_back(bind(&SLL::MoveHighlight, this, 0, 1, placeholders::_1));
		else
			action.back().push_back(bind(&SLL::MoveHighlight, this, 2, 1, placeholders::_1));
		action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::TitleDisappear, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
		action.back().push_back(bind(&SLL::drawArrowFlow, this, Cur, placeholders::_1));
		
		action.push_back(vector<function<void(int)> >());

		action.back().push_back(bind(&SLL::MoveHighlight, this, 1, 2, placeholders::_1));
		action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
		action.back().push_back(bind(&SLL::TitleAppear, this, Cur->nxt, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur, Selecting, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur->nxt, Normal, Selecting, placeholders::_1));

		Cur = Cur->nxt;
	}

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::MoveHighlight, this, 2, 3, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, Cur, Selecting, placeholders::_1));

	action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(bind(&SLL::TitleAppear, this, NewNode, New, placeholders::_1));
	action.back().push_back(bind(&SLL::NodeAppear, this, NewNode, placeholders::_1));

	action.back().push_back(bind(&SLL::ConnectNode, this, NewNode->prev, NewNode, placeholders::_1));

}

bool SLL::insertNode( int i, int v)
{
	if (i > NodeNumber || i < 0 || NodeNumber == maxNodeNumber) {
		Signal = Pending;
		return 0;
	}

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

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("Node Cur = Head\nfor(k = 0; k < i - 1; k++)\n     Cur = Cur.next\nNode Aft = Cur.next\nNode NewNode = new Node(v)\nNewNode.next = Aft\nCur.next = NewNode"));

	//run to node
	Cur = Head;
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&SLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(bind(&SLL::TitleAppear, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Cur, Normal, Selecting, placeholders::_1));


	while (Cur->nxt != NewNode) {
		action.push_back(vector<function<void(int)> >());

		if (Cur == Head)
			action.back().push_back(bind(&SLL::MoveHighlight, this, 0, 1, placeholders::_1));
		else
			action.back().push_back(bind(&SLL::MoveHighlight, this, 2, 1, placeholders::_1));

		action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::TitleDisappear, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
		action.back().push_back(bind(&SLL::drawArrowFlow, this, Cur, placeholders::_1));

		action.push_back(vector<function<void(int)> >());

		action.back().push_back(bind(&SLL::MoveHighlight, this, 1, 2, placeholders::_1));
		action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
		action.back().push_back(bind(&SLL::TitleAppear, this, Cur->nxt, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur, Selecting, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur->nxt, Normal, Selecting, placeholders::_1));

		Cur = Cur->nxt;
	}

	//Change Aft State
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&SLL::MoveHighlight, this, 2, 3, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));

	action.back().push_back(bind(&SLL::TitleAppear, this, NewNode->nxt, Next, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, NewNode->nxt, Normal, Next, placeholders::_1));

	//NewNode appears
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&SLL::MoveHighlight, this, 3, 4, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, NewNode, New, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, NewNode->nxt, Next, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(bind(&SLL::NodeAppear, this, NewNode, placeholders::_1));
		
	//Connect Nodes	
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&SLL::MoveHighlight, this, 4, 5, placeholders::_1));
	action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, NewNode, New, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(bind(&SLL::ConnectNode, this, NewNode, NewNode->nxt, placeholders::_1));
	action.back().push_back(bind(&SLL::drawNode, this, NewNode, placeholders::_1));

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
	action.back().push_back(bind(&SLL::MoveHighlight, this, 5, 6, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, NewNode, New, placeholders::_1));
	action.back().push_back(bind(&SLL::ConnectNode, this, Cur, NewNode, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListPartial, this, Head, NewNode->prev, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListPartial, this, NewNode, Tail, placeholders::_1));
	action.back().push_back(bind(&SLL::drawNode, this, NewNode, placeholders::_1));
	
	//Move NewNode up
	action.push_back(vector<function<void(int) > >());
	action.back().push_back(bind(&SLL::TitleAppear, this, NewNode, New, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, NewNode, New, placeholders::_1));
	action.back().push_back(std::bind(&SLL::MoveNode, this, NewNode, Cur->Pos.x + 95, Cur->Pos.y + 100, Cur->Pos.x + 95, Cur->Pos.y, placeholders::_1));
	action.back().push_back(std::bind(&SLL::SlideNodes, this, NewNode->nxt, Cur->Pos.x + 95, Cur->Pos.y, Cur->Pos.x + 95 + 95, DefaultPosY, placeholders::_1));
	action.back().push_back(std::bind(&SLL::drawList, this, placeholders::_1));

	return 1;
}

void SLL::removeAtBeginning() {
	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("Node Cur = Head\nHead = Cur.next\ndelete Cur"));

	Node* Dell = Head;
	Dell->NumberInList = 0;

	for (Node* tmp = Dell->nxt; tmp; tmp = tmp->nxt)
		tmp->NumberInList--;

	//tmp = Head
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Dell, Normal, Selecting, placeholders::_1));
	action.back().push_back(bind(&SLL::TitleAppear, this, Dell, Selecting, placeholders::_1));

	//Next = Head

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::MoveHighlight, this, 0, 1, placeholders::_1));
	action.back().push_back(bind(&SLL::SetNodesNormal, this, Dell, Tail, placeholders::_1));
	action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Dell, Normal, Remove, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Dell->nxt, Normal, Next, placeholders::_1));
	action.back().push_back(bind(&SLL::TitleAppear, this, Dell->nxt, Next, placeholders::_1));
	action.back().push_back(bind(&SLL::TitleDisappear, this, Dell, Selecting, placeholders::_1));

	//Update Nodes position
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&SLL::MoveHighlight, this, 1, 2, placeholders::_1));
	action.back().push_back(bind(&SLL::SetNodesNormal, this, Dell, Tail, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, Dell, Remove, placeholders::_1));

	action.back().push_back(bind(&SLL::drawListExcept, this, Dell, placeholders::_1));
	action.back().push_back(bind(&SLL::DisconnectNode, this, Dell, Dell->nxt, placeholders::_1));
	action.back().push_back(bind(&SLL::NodeDisappear, this, Dell, placeholders::_1));
	action.back().push_back(bind(&SLL::SlideNodes, this, Dell->nxt, Dell->Pos.x + 95, DefaultPosY, Dell->Pos.x, DefaultPosY, placeholders::_1));

	//Delete
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::DeleteNode, this, Dell, placeholders::_1));
	
	return;
}

void SLL::removeAtEnd() {
	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("Node Cur = Head, Aft = Head.next\nwhile(Aft.next != null)\n    Cur = Cur.next, Aft = Aft.next\nCur.next = null\ndelete Aft"));

	//Run to node
	Node* Cur = Head;
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&SLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&SLL::TitleAppear, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&SLL::TitleAppear, this, Cur->nxt, Next, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Cur, Normal, Selecting, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Cur->nxt, Normal, Next, placeholders::_1));

	while (Cur->nxt->nxt != nullptr) {
		action.push_back(vector<function<void(int)> >());

		if (Cur == Head)
			action.back().push_back(bind(&SLL::MoveHighlight, this, 0, 1, placeholders::_1));
		else
			action.back().push_back(bind(&SLL::MoveHighlight, this, 2, 1, placeholders::_1));

		action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&SLL::drawArrowFlow, this, Cur, placeholders::_1));
		action.back().push_back(bind(&SLL::TitleDisappear, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::TitleDisappear, this, Cur->nxt, Next, placeholders::_1));

		action.push_back(vector<function<void(int)> >());

		action.back().push_back(bind(&SLL::MoveHighlight, this, 1, 2, placeholders::_1));
		action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur, Selecting, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur->nxt, Next, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur->nxt->nxt, Normal, Next, placeholders::_1));
		action.back().push_back(bind(&SLL::TitleAppear, this, Cur->nxt, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::TitleAppear, this, Cur->nxt->nxt, Next, placeholders::_1));

		Cur = Cur->nxt;
	}
	
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::MoveHighlight, this, 2, 3, placeholders::_1));
	action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListExcept, this, Cur->nxt, placeholders::_1));
	action.back().push_back(bind(&SLL::drawNode, this, Cur->nxt, placeholders::_1));
	action.back().push_back(bind(&SLL::DisconnectNode, this, Cur, Cur->nxt, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Cur->nxt, Next, Remove, placeholders::_1));

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::MoveHighlight, this, 3, 4, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListExcept, this, Cur->nxt, placeholders::_1));
	action.back().push_back(bind(&SLL::NodeDisappear, this, Cur->nxt, placeholders::_1));

	//Delete
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::DeleteNode, this, Cur->nxt, placeholders::_1));
	
	return;
}

bool SLL::removeNode(int i)
{
	if (i >= NodeNumber || i < 0) {
		Signal = Pending;
		return 0;
	}

	initList();

	NodeNumber--;

	Node* Cur = Head;

	for (int j = 0; j < i; j++) 
		Cur = Cur->nxt;

	Node* Dell = Cur;

	if (i == 0) {
		removeAtBeginning();
		return 1;
	}

	if (i == NodeNumber) {
		removeAtEnd();
		return 1;
	}

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("Node Cur = Head\nfor (k = 0; k < i - 1; k++)\n    Cur = Cur.next\nNode Del = Cur.next\nNode Aft = Del.next\nCur.next = Aft\ndelete Del"));

	for (Node* tmp = Dell->nxt; tmp; tmp = tmp->nxt)
		tmp->NumberInList--;

	//Run to node
	Cur = Head;
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&SLL::TitleAppear, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Cur, Normal, Selecting, placeholders::_1));

	while (Cur->nxt != Dell) {
		action.push_back(vector<function<void(int)> >());

		if (Cur == Head)
			action.back().push_back(bind(&SLL::MoveHighlight, this, 0, 1, placeholders::_1));
		else
			action.back().push_back(bind(&SLL::MoveHighlight, this, 2, 1, placeholders::_1));

		action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::TitleDisappear, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&SLL::drawArrowFlow, this, Cur, placeholders::_1));


		action.push_back(vector<function<void(int)> >());

		action.back().push_back(bind(&SLL::MoveHighlight, this, 1, 2, placeholders::_1));
		action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&SLL::TitleAppear, this, Cur->nxt, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur, Selecting, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur->nxt, Normal, Selecting, placeholders::_1));

		Cur = Cur->nxt;
	}

	//Change remove node
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&SLL::MoveHighlight, this, 2, 3, placeholders::_1));
	action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Dell, Normal, Remove, placeholders::_1));

	//Aft = Del.next
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&SLL::MoveHighlight, this, 3, 4, placeholders::_1));
	action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Dell->nxt, Normal, Next, placeholders::_1));
	action.back().push_back(bind(&SLL::TitleAppear, this, Dell->nxt, Next, placeholders::_1));

	//Move Cur down
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&SLL::MoveHighlight, this, 4, 5, placeholders::_1));
	action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, Dell, Remove, placeholders::_1));
	action.back().push_back(bind(&SLL::MoveNode, this, Dell, Dell->prev->Pos.x + 95, DefaultPosY, Dell->prev->Pos.x + 95, DefaultPosY + 100, placeholders::_1));
	action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));

	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, Dell, Remove, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListPartial, this, Head, Dell->prev, placeholders::_1));
	action.back().push_back(bind(&SLL::drawListPartial, this, Dell, Tail, placeholders::_1));
	action.back().push_back(bind(&SLL::ConnectNode, this, Dell->prev, Dell->nxt, placeholders::_1));

	//Update Nodes position
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::MoveHighlight, this, 5, 6, placeholders::_1));
	action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, Dell, Remove, placeholders::_1));

	action.back().push_back(bind(&SLL::drawListExcept, this, Dell, placeholders::_1));
	action.back().push_back(bind(&SLL::DisconnectNode, this, Dell, Dell->nxt, placeholders::_1));
	action.back().push_back(bind(&SLL::NodeDisappear, this, Dell, placeholders::_1));
	action.back().push_back(bind(&SLL::SlideNodes, this, Dell->nxt, Dell->Pos.x + 95, DefaultPosY, Dell->Pos.x, DefaultPosY, placeholders::_1));
	
	//Delete
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::DeleteNode, this, Dell, placeholders::_1));

	return 1;
}



void SLL::searchNode(tgui::String v)
{
	if (!Head)
		return;

	initList();

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("index = 0, Cur = Head\nwhile(Cur.value != v)\n		index++, Cur = Cur.next\nif (Cur == null)\n		return NOT_FOUND\nreturn index"));

	string s = v.toStdString();

	//Run to node
	Node* Cur = Head;
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&SLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&SLL::TitleAppear, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Cur, Normal, Selecting, placeholders::_1));

	while (Cur != nullptr && Cur->Val != s) {

		action.push_back(vector<function<void(int)> >());

		if (Cur == Head)
			action.back().push_back(bind(&SLL::MoveHighlight, this, 0, 1, placeholders::_1));
		else
			action.back().push_back(bind(&SLL::MoveHighlight, this, 2, 1, placeholders::_1));

		action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::TitleDisappear, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&SLL::drawArrowFlow, this, Cur, placeholders::_1));


		if (Cur->nxt) {
			action.push_back(vector<function<void(int)> >());
			action.back().push_back(bind(&SLL::MoveHighlight, this, 1, 2, placeholders::_1));
			action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
			action.back().push_back(bind(&SLL::TitleAppear, this, Cur->nxt, Selecting, placeholders::_1));
			action.back().push_back(bind(&SLL::ChangeState, this, Cur, Selecting, Visited, placeholders::_1));
			action.back().push_back(bind(&SLL::ChangeState, this, Cur->nxt, Normal, Selecting, placeholders::_1));
		}
		

		Cur = Cur->nxt;
	}

	if (!Cur) {
		action.push_back(vector<function<void(int)> >());
		action.back().push_back(bind(&SLL::MoveHighlight, this, 2, 4, placeholders::_1));
		action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Tail, Selecting, Visited, placeholders::_1));
		return;
	}
	
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&SLL::MoveHighlight, this, 2, 5, placeholders::_1));
	action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
	//action.back().push_back(bind(&SLL::setNodeState, this, Cur, New, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Cur, Selecting, New, placeholders::_1));
}

void SLL:: updateNode(int i, int v)
{
	if (i < 0 || i >= NodeNumber)
		return;

	Node* Cur = Head;
		for (int j = 0; j < i - 1; j++)
			Cur = Cur->nxt;

	String preVal = Cur->nxt->Val;

	Cur = Head;
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&SLL::TitleAppear, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeState, this, Cur, Normal, Selecting, placeholders::_1));


	for (int j = 0; j < i - 1; j++) {
		action.push_back(vector<function<void(int)> >());
		action.back().push_back(bind(&SLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::setNodeState, this, Cur, Selecting, placeholders::_1));

		action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&SLL::drawArrowFlow, this, Cur, placeholders::_1));
		action.back().push_back(bind(&SLL::TitleAppear, this, Cur->nxt, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::TitleDisappear, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur, Selecting, Visited, placeholders::_1));
		action.back().push_back(bind(&SLL::ChangeState, this, Cur->nxt, Normal, Selecting, placeholders::_1));

		Cur = Cur->nxt;

		if (j + 1 == i)
			action.back().push_back(bind(&SLL::ChangeState, this, Cur->nxt, Normal, Next, placeholders::_1));
	}

	//Change Aft State
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&SLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
	action.back().push_back(bind(&SLL::setNodeState, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&SLL::drawList, this, placeholders::_1));

	action.back().push_back(bind(&SLL::TitleAppear, this, Cur->nxt, Next, placeholders::_1));
	action.back().push_back(bind(&SLL::ChangeValue, this, Cur->nxt, preVal, String(to_string(v)), placeholders::_1));
	
	initList();
}

void SLL::initButtons()
{
	tgui::Button::Ptr InsertEx = gui.get<tgui::Button>("InsertEx");
	tgui::Button::Ptr InsertMode = gui.get<tgui::Button>("InsertMode");
	tgui::Button::Ptr InsertButton = gui.get<tgui::Button>("InsertButton");
	tgui::EditBox::Ptr InsertPos = gui.get<tgui::EditBox>("InsertPos");
	tgui::EditBox::Ptr InsertVal = gui.get<tgui::EditBox>("InsertVal");

	tgui::Button::Ptr DeleteEx = gui.get<tgui::Button>("DeleteEx");
	tgui::Button::Ptr DeleteMode = gui.get<tgui::Button>("DeleteMode");
	tgui::Button::Ptr DeleteButton = gui.get<tgui::Button>("DeleteButton");
	tgui::EditBox::Ptr DeletePos = gui.get<tgui::EditBox>("DeletePos");

	tgui::Button::Ptr CreateButton = gui.get<tgui::Button>("CreateButton");
	tgui::Button::Ptr InputGen = gui.get<tgui::Button>("InputGen");
	tgui::EditBox::Ptr UserInput = gui.get<tgui::EditBox>("EditBox1");
	tgui::Button::Ptr UserInputEx = gui.get<tgui::Button>("UserInputEx");

	tgui::Button::Ptr SearchNode = gui.get<tgui::Button>("SearchNode");
	tgui::Button::Ptr SearchEx = gui.get<tgui::Button>("SearchEx");
	tgui::EditBox::Ptr SearchVal = gui.get<tgui::EditBox>("SearchVal");

	tgui::Button::Ptr UpdateNode = gui.get<tgui::Button>("UpdateNode");
	tgui::Button::Ptr UpdateEx = gui.get<tgui::Button>("UpdateEx");
	tgui::EditBox::Ptr UpdateVal = gui.get<tgui::EditBox>("UpdateVal");
	tgui::EditBox::Ptr UpdatePos = gui.get<tgui::EditBox>("UpdatePos");

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");

	tgui::Button::Ptr SlideIn = gui.get<tgui::Button>("SlideIn");
	tgui::Button::Ptr SlideOut = gui.get<tgui::Button>("SlideOut");
	tgui::Panel::Ptr EditPanel = gui.get<tgui::Panel>("EditPanel");
	EditPanel->loadWidgetsFromFile("assets/ControlPanel.txt");

	tgui::Slider::Ptr Speed = EditPanel->get<tgui::Slider>("Speed");

	Speed->setValue(2);

	InsertButton->onPress([=] {
		InsertMode->setVisible(1 - InsertMode->isVisible());

		if (InsertModes == 0)
			InsertPos->setVisible(InsertMode->isVisible());
	
		InsertVal->setVisible(InsertMode->isVisible());

		InsertEx->setVisible(InsertMode->isVisible());
		});

	InsertMode->onPress([=] {
		InsertModes = (InsertModes + 1) % 3;
		
		switch (InsertModes) {
		case 0:
			InsertMode->setText(tgui::String("Middle"));
			InsertPos->setVisible(1);
			InsertPos->setText(tgui::String(""));
			InsertPos->setPosition({ "InsertMode.right + 10", "InsertMode.top" });
			InsertVal->setPosition({ "InsertPos.right + 10", "InsertMode.top" });
			InsertEx->setPosition({ "InsertVal.right + 10", "InsertMode.top" });

			break;

		case 1:
			InsertMode->setText(tgui::String("Head"));
			InsertPos->setVisible(0);
			InsertVal->setPosition({ "InsertMode.right + 10", "InsertMode.top" });
			InsertEx->setPosition({ "InsertVal.right + 10", "InsertMode.top" });

			break;

		case 2:
			InsertMode->setText(tgui::String("Tail"));
			InsertPos->setVisible(0);
			InsertVal->setPosition({ "InsertMode.right + 10", "InsertMode.top" });
			InsertEx->setPosition({ "InsertVal.right + 10", "InsertMode.top" });

			break;
		}
		});

	DeleteButton->onPress([=] {
		DeleteMode->setVisible(1 - DeleteMode->isVisible());

		if (!DeleteModes)
			DeletePos->setVisible(DeleteMode->isVisible());

		DeleteEx->setVisible(DeleteMode->isVisible());

		tgui::String s = DeletePos->getText();

		if (getSize() > 0 && (s.length() == 0 || s.toInt() > getSize() - 1))
			DeletePos->setText(tgui::String(to_string(rand() % getSize())));

		});

	DeleteMode->onPress([=] {
		DeleteModes = (DeleteModes + 1) % 3;

		switch (DeleteModes) {
		case 0:
			DeleteMode->setText(tgui::String("Middle"));
			DeletePos->setVisible(1);
			DeletePos->setText(tgui::String(""));
			DeletePos->setPosition({ "DeleteMode.right + 10", "DeleteMode.top" });
			DeleteEx->setPosition({ "DeletePos.right + 10", "DeleteMode.top" });

			break;

		case 1:
			DeleteMode->setText(tgui::String("Head"));
			DeletePos->setVisible(0);
			DeleteEx->setPosition({ "DeleteMode.right + 10", "DeleteMode.top" });

			break;

		case 2:
			DeleteMode->setText(tgui::String("Tail"));
			DeletePos->setVisible(0);
			DeleteEx->setPosition({ "DeleteMode.right + 10", "DeleteMode.top" });


			break;
		}
		});

	SearchNode->onPress([=] {
		SearchVal->setVisible(1 - SearchVal->isVisible());
		SearchEx->setVisible(1 - SearchEx->isVisible());
		});

	UpdateNode->onPress([=] {
		UpdateVal->setVisible(1 - UpdateVal->isVisible());
		UpdatePos->setVisible(1 - UpdatePos->isVisible());
		UpdateEx->setVisible(1 - UpdateEx->isVisible());
		});


	CreateButton->onPress([=] {
		InputGen->setVisible(1 - InputGen->isVisible());

		if (!GenModes)
			UserInput->setVisible(InputGen->isVisible());
		UserInputEx->setVisible(InputGen->isVisible());
		});

	InputGen->onPress([=] {
		GenModes = 1 - GenModes;

		if (GenModes == 1) {
			InputGen->setText(tgui::String("Random Input"));

			UserInput->setVisible(0);
			UserInputEx->setPosition({ "InputGen.right + 10", "InputGen.top + 5" });
		}
		else {
			InputGen->setText(tgui::String("Manual Input"));

			UserInput->setVisible(1);
			UserInputEx->setPosition({ "EditBox1.right + 10", "EditBox1.top" });
		}
		});

	UserInputEx->onPress([=] {
		ClearAction();
		Signal = Pending;

		if (GenModes == 1)
			genList();
		else {
			tgui::String s = UserInput->getText();
			genList(s);
		}
		});

	InsertEx->onPress([=] {
		if (NodeNumber == maxNodeNumber)
		return;

		Signal = Inserting;
		timer.restart();
	
		ClearAction();
		ShowDirection = 0;
		CurStep = 0;

		int Pos = InsertPos->getText().toInt();
		int Val = InsertVal->getText().toInt();

		if (InsertModes == 0)
			insertNode(Pos, Val);
		else if (InsertModes == 1)
			insertNode(0, Val);
		else if (InsertModes == 2)
			insertNode(NodeNumber , Val);
		});

	DeleteEx->onPress([=] {
		if (NodeNumber == 0)
		return;

		Signal = Removing;
		timer.restart();

		ClearAction();
		ShowDirection = 0;
		CurStep = 0;

		int Pos = DeletePos->getText().toInt();
		
		if (DeleteModes == 0)
			removeNode(Pos);
		else if (DeleteModes == 1)
			removeNode(0);
		else
			removeNode(NodeNumber - 1);
		});


	SearchEx->onPress([=] {
		Signal = Searching;
		timer.restart();

		ClearAction();
		ShowDirection = 0;
		CurStep = 0;

		tgui::String Val = SearchVal->getText();

		searchNode(Val);
		});

	UpdateEx->onPress([=] {
		Signal = Searching;
		timer.restart();

		ClearAction();
		ShowDirection = 0;
		CurStep = 0;

		tgui::String Val = UpdateVal->getText();
		tgui::String Pos = UpdatePos->getText();

		updateNode(Pos.toInt(), Val.toInt());
		});

	PseudoCode->loadWidgetsFromFile("assets/CodeWindow.txt");

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

	Speed->onValueChange([=] {
		Duration = (int)(700 / (0.5f + 0.25f * Speed->getValue()));
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

	case Removing:
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
						if (CurStep + 2 == (int)action.size()) {
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

	case Searching:
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