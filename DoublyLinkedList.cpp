#include "DoublyLinkedList.h"
#include <math.h>
#include <functional>

using namespace std;
using namespace sf;

int DLL::getSize()
{
	return NodeNumber;
}

void DLL::changePosition(Node* Cur, float x, float y)
{
	Cur->changeNodePosition(x, y);
}

void DLL::NodeAppear(Node* Cur, int Elapsed)
{
	Cur->drawNode((int)(255 * (double)Elapsed / Duration));
}

void DLL::NodeDisappear(Node* Cur, int Elapsed)
{
	NodeAppear(Cur, Duration - Elapsed);
}

void DLL::drawNode(Node* Cur, int Dummy)
{
	Cur->drawNode(255);
}

void DLL::setNodeState(Node* Cur, Nodestate NodeState, int Dummy)
{
	if (!Cur)
		return;

	if (NodeState == Visited)
		Cur->ArrowState = Visited;

	if (NodeState == New) {
		Cur->ArrowState = New;
		Cur->ArrowStateU = New;
	}

	Cur->NodeState = NodeState;
}

void DLL::MoveNode(Node* Cur, float CurX, float CurY, float NxtX, float NxtY, int Elapsed)
{
	if (!Cur)
		return;

	Cur->changeNodePosition(CurX + (NxtX - CurX) * (double)Elapsed / Duration, CurY + (NxtY - CurY) * (double)Elapsed / Duration);
}

void DLL::SlideNodes(Node* Cur, float CurX, float CurY, float NxtX, float NxtY, int Elapsed)
{
	if (!Cur)
		return;

	MoveNode(Cur, CurX, CurY, NxtX, NxtY, Elapsed);

	for (Node* tmp = Cur->nxt; tmp; tmp = tmp->nxt)
		tmp->changeNodePosition(tmp->prev->Pos.x + 95, tmp->prev->Pos.y);

}

void DLL::SetNodesNormal(Node* A, Node* B, int Dummy)
{
	if (!A || !B)
		return;

	for (Node* tmp = A; tmp != B; tmp = tmp->nxt) {
		tmp->ArrowState = Normal;
		tmp->ArrowStateU = Normal;
		//tmp->NodeState = Normal;
	}

	B->ArrowState = Normal;
	B->ArrowStateU = Normal;
}

void DLL::ClearAction()
{
	for (int i = 0; i < action.size(); i++)
		for (int j = 0; j < action[i].size(); j++)
			action[i][j](Duration);

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");
	tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");

	TextArea->setText(tgui::String(""));
	TextHighlight->setVisible(0);

	action.clear();
}

void DLL::drawList(int Dummy)
{
	//if (Head)
	//	app.draw(Head->Title);

	for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
		tmp->updateArrow(tmp->nxt);
		tmp->drawDArrow();
		tmp->drawNode(255);
	}
}

void DLL::TitleAppear(Node* Cur, Nodestate NodeState, int Elapsed)
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
	Cur->Title.setOrigin(Cur->Title.getLocalBounds().left + Cur->Title.getLocalBounds().width / 2, Cur->Title.getLocalBounds().top + Cur->Title.getLocalBounds().height / 2);
	Cur->Title.setPosition(Cur->Pos.x + 23, Cur->Pos.y + 50 + Cur->Title.getLocalBounds().height / 2);
	app.draw(Cur->Title);
}

void DLL::TitleDisappear(Node* Cur, Nodestate NodeState, int Elapsed)
{
	TitleAppear(Cur, NodeState, Duration - Elapsed);
}

void DLL::HighlightAppear(int Elapsed)
{
	if (Elapsed >= Duration / 2)
		Elapsed = Duration / 2;

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");

	TextHighlight->setPosition({ 0, 4 });
	TextHighlight->setVisible(1);
	TextHighlight->setInheritedOpacity(2 * (float)Elapsed / Duration);
}

void DLL::MoveHighlight(int CurLine, int NextLine, int Elapsed)
{
	if (Elapsed >= Duration / 2)
		Elapsed = Duration / 2;

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");

	int Distance = (int)(2 * (double)Elapsed / Duration * (NextLine - CurLine) * 26);

	TextHighlight->setPosition({ 0, 4 + CurLine * 26 + Distance });
}

void DLL::drawListPartial(Node* A, Node* B, int Dummy)
{
	if (!A || !B)
		return;

	for (Node* tmp = A; tmp != B; tmp = tmp->nxt) {
		tmp->updateArrow(tmp->nxt);
		tmp->drawDArrow();
		tmp->drawNode(255);
	}

	if (B)
		B->drawNode(255);
}

void DLL::drawListExcept(Node* ExceptNode, int Dummy)
{
	for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
		if (tmp == ExceptNode)
			continue;

		if (tmp->nxt) {
			if (tmp->nxt == ExceptNode && ExceptNode->nxt) {
				tmp->updateArrow(ExceptNode->nxt);
				tmp->drawDArrow();
			}
			else if (tmp->nxt != ExceptNode)
				tmp->drawDArrow();
		}

		tmp->drawNode(255);
	}
}

void DLL::DeleteNode(Node*& Cur, int Dummy)
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

void DLL::initList() {
	int i = 0;

	for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
		tmp->NumberInList = i++;

		tmp->NodeState = Normal;
		tmp->ArrowState = Normal; 
		tmp->ArrowStateU = Normal;

		if (!tmp->nxt)
			Tail = tmp;
	}

	drawList(1);
}

void DLL::genList()
{
	srand(time(0));

	//Release old List
	Node* tmp = Head;

	while (tmp) {
		Node* Dummy = tmp;
		tmp = tmp->nxt;
		delete Dummy;
	}

	Tail = nullptr;

	//Gen new list

	NodeNumber = rand() % maxNodeNumber + 1;

	while (NodeNumber == 0)
		NodeNumber = rand() % 10;

	Head = new Node(rand() % 100);
	Head->NodeType = 1;
	Head->NumberInList = 0;
	Head->changeNodePosition(DefaultPosX, DefaultPosY);
	Tail = Head;

	Node* Cur = Head;

	for (int i = 1; i < NodeNumber; i++) {
		Cur->nxt = new Node(rand() % 100);
		Node* tmp = Cur;

		Cur = Cur->nxt;
		Cur->prev = tmp;
		Cur->NumberInList = i;
		Cur->NodeType = 1;

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
				int ArrowLength = (int)((Util::DistanceBetweenNodes(tmp->Pos, tmp->nxt->Pos) - 46 + 10) * Elapsed / Duration);
				tmp->Arrow.setTextureRect(IntRect(100 - ArrowLength, 0, ArrowLength, 10));
				tmp->drawDArrow();
			}

			tmp->drawNode(255);
		}

		app.display();

		Elapsed = clock.getElapsedTime().asMilliseconds();
	}

}

void DLL::genList(const tgui::String s)
{
	vector<tgui::String> parts = tgui::String(s).split(',', true);

	if (parts.size() == 0)
		return;

	NodeNumber = min((int)parts.size(), maxNodeNumber);

	for (int i = 0; i < parts.size(); i++) {
		int tmp;

		if (!parts[i].attemptToInt(tmp) || tmp >= 1000)
			return;
	}
	//Release old List
	Node* tmp = Head;

	while (tmp) {
		Node* Dummy = tmp;
		tmp = tmp->nxt;
		delete Dummy;
	}

	Tail = nullptr;

	//Gen new list
	Head = new Node(parts[0].toInt());
	Head->NumberInList = 0;
	Head->NodeType = 1;
	Head->changeNodePosition(DefaultPosX, DefaultPosY);
	Tail = Head;

	Node* Cur = Head;

	for (int i = 1; i < NodeNumber; i++) {
		Cur->nxt = new Node(parts[i].toInt());
		Node* tmp = Cur;

		Cur = Cur->nxt;
		Cur->prev = tmp;
		Cur->NumberInList = i;
		Cur->NodeType = 1;

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
				int ArrowLength = (int)((Util::DistanceBetweenNodes(tmp->Pos, tmp->nxt->Pos) - 46 + 10) * Elapsed / Duration);
				tmp->Arrow.setTextureRect(IntRect(100 - ArrowLength, 0, ArrowLength, 10));
				tmp->drawDArrow();
			}

			tmp->drawNode(255);
		}

		app.display();

		Elapsed = clock.getElapsedTime().asMilliseconds();
	}

}

void DLL::ChangeState(Node* Cur, Nodestate CurState, Nodestate NextState, int Elapsed)
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

void DLL::ChangeValue(Node* Cur, sf::String preVal, sf::String nextVal, int Elapsed)
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

void DLL::drawArrowFlow(Node* Cur, int Elapsed)
{
	if (!Cur->nxt)
		return;

	Vector2f OriginPos = Cur->Arrow.getPosition();
	float Angle = Cur->Arrow.getRotation();

	Cur->TmpArrow.setPosition(OriginPos.x - 7 * sin(Angle) - 5 * cos(Angle), OriginPos.y - 7 * cos(Angle) + 5 * sin(Angle));
	Cur->TmpArrow.setRotation(Angle);
	Cur->TmpArrow.setColor(*MainColor);

	int ArrowLength = (int)(Util::DistanceBetweenNodes(Cur->Pos, Cur->nxt->Pos) - 46 + 10);

	Cur->TmpArrow.setTextureRect(IntRect(100 - ArrowLength, 0, (int)(ArrowLength * (double)Elapsed / Duration), 10));
	app.draw(Cur->TmpArrow);

	if ((double)Elapsed / Duration >= 4.5 / 5) {
		Cur->ArrowState = Visited;
	}
}

void DLL::ConnectNode(Node* A, Node* B, int Elapsed)
{
	if (!B)
		return;

	A->updateArrow(B);
	A->ArrowState = New;
	A->ArrowStateU = New;

	double Distance = Util::DistanceBetweenNodes(A->Pos, B->Pos) - 46 + 10;
	int Length = (int)Distance * Elapsed / Duration;
	A->Arrow.setTextureRect(IntRect(100 - Length, 0, Length, 10));

	A->Arrow.setColor(SGreen);
	
	Vector2f OriginPos = A->Arrow.getPosition();
	float Angle = A->Arrow.getRotation() * PI / 180;

	A->Arrow.setPosition(OriginPos.x + 7 * sin(Angle) - 5 * cos(Angle), OriginPos.y - 7 * cos(Angle) - 5 * sin(Angle));
	app.draw(A->Arrow);

	A->Arrow.setRotation(Angle * 180 / PI + 180);
	A->Arrow.setOrigin(A->Arrow.getLocalBounds().left, A->Arrow.getLocalBounds().top + A->Arrow.getLocalBounds().height);
	A->Arrow.setPosition(OriginPos.x - 7 * sin(Angle) + (Distance - 5) * cos(Angle), OriginPos.y + 7 * cos(Angle) - 5 * sin(Angle) + Distance * sin(Angle));
	app.draw(A->Arrow);

	A->Arrow.setOrigin(A->Arrow.getLocalBounds().left, A->Arrow.getLocalBounds().top);
	A->Arrow.setPosition(OriginPos);
	A->Arrow.setRotation(Angle * 180 / PI);

}

void DLL::DisconnectNode(Node* A, Node* B, int Elapsed)
{
	if (!B)
		return;

	Elapsed = Duration - Elapsed;

	A->updateArrow(B);

	double Distance = Util::DistanceBetweenNodes(A->Pos, B->Pos) - 46 + 10;
	int Length = (int)Distance * Elapsed / Duration;
	A->Arrow.setTextureRect(IntRect(100 - Length, 0, Length, 10));

	Vector2f OriginPos = A->Arrow.getPosition();
	float Angle = A->Arrow.getRotation() * PI / 180;

	A->Arrow.setPosition(OriginPos.x + 7 * sin(Angle) - 5 * cos(Angle), OriginPos.y - 7 * cos(Angle) - 5 * sin(Angle));
	app.draw(A->Arrow);

	A->Arrow.setRotation(Angle * 180 / PI + 180);
	A->Arrow.setOrigin(A->Arrow.getLocalBounds().left, A->Arrow.getLocalBounds().top + A->Arrow.getLocalBounds().height);
	A->Arrow.setPosition(OriginPos.x - 7 * sin(Angle) + (Distance - 5) * cos(Angle), OriginPos.y + 7 * cos(Angle) - 5 * sin(Angle) + Distance * sin(Angle));
	app.draw(A->Arrow);

	A->Arrow.setOrigin(A->Arrow.getLocalBounds().left, A->Arrow.getLocalBounds().top);
	A->Arrow.setPosition(OriginPos);
	A->Arrow.setRotation(Angle * 180 / PI);
}

void DLL::insertAtBeginning(Node*& NewNode)
{
	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("Node NewNode = new Node(v)\nNewNode.next = Head\nif(Head)\n	Head.prev = NewNode\nHead = NewNode"));

	NewNode->nxt = Head;
	if (Head)
		Head->prev = NewNode;

	Head = NewNode;

	for (Node* tmp = NewNode->nxt; tmp; tmp = tmp->nxt)
		tmp->NumberInList++;

	if (NewNode->nxt) {
		changePosition(NewNode, DefaultPosX, DefaultPosY + 100);
		changePosition(NewNode->nxt, DefaultPosX, DefaultPosY);
	}
	else {
		changePosition(NewNode, DefaultPosX, DefaultPosY);
	}

	//NewNode appears

	action.push_back(vector<function<void(int) > >());

	action.back().push_back(bind(&DLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(std::bind(&DLL::drawListExcept, this, NewNode, std::placeholders::_1));
	action.back().push_back(std::bind(&DLL::NodeAppear, this, NewNode, std::placeholders::_1));

	if (!Head->nxt) {
		action.push_back(vector<function<void(int) > >());

		action.back().push_back(std::bind(&DLL::drawList, this, placeholders::_1));
		action.back().push_back(std::bind(&DLL::MoveHighlight, this, 0, 1, placeholders::_1));

		action.push_back(vector<function<void(int) > >());

		action.back().push_back(std::bind(&DLL::drawList, this, placeholders::_1));
		action.back().push_back(std::bind(&DLL::MoveHighlight, this, 1, 4, placeholders::_1));
		action.back().push_back(std::bind(&DLL::TitleAppear, this, NewNode, New, placeholders::_1));
		return;
	}

	//TextHighlight->setSize(Line2->getSize());
	//TextHighlight->setPosition(Line2->getPosition());

	//Connect NewNode to Head
	action.push_back(vector<function<void(int) > >());

	action.back().push_back(std::bind(&DLL::MoveHighlight, this, 0, 1, placeholders::_1));
	action.back().push_back(std::bind(&DLL::ConnectNode, this, NewNode, NewNode->nxt, placeholders::_1));
	action.back().push_back(std::bind(&DLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(std::bind(&DLL::drawNode, this, NewNode, placeholders::_1));

	action.push_back(vector<function<void(int) > >());

	action.back().push_back(std::bind(&DLL::drawList, this, placeholders::_1));
	action.back().push_back(std::bind(&DLL::MoveHighlight, this, 1, 3, placeholders::_1));

	//TextHighlight->setSize(Line3->getSize());
	//TextHighlight->setPosition(Line3->getPosition());
	if (NewNode->nxt) {
		action.push_back(vector<function<void(int) > >());
		action.back().push_back(std::bind(&DLL::MoveHighlight, this, 3, 4, placeholders::_1));
		action.back().push_back(std::bind(&DLL::TitleAppear, this, NewNode, New, placeholders::_1));
		action.back().push_back(std::bind(&DLL::MoveNode, this, NewNode, DefaultPosX, DefaultPosY + 100, DefaultPosX, DefaultPosY, placeholders::_1));
		action.back().push_back(std::bind(&DLL::SlideNodes, this, NewNode->nxt, DefaultPosX, DefaultPosY, DefaultPosX + 95, DefaultPosY, placeholders::_1));
		action.back().push_back(std::bind(&DLL::drawList, this, placeholders::_1));
	}

	initProgress();
}

void DLL::insertAtEnd(Node*& NewNode)
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

	action.back().push_back(bind(&DLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(bind(&DLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(bind(&DLL::TitleAppear, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, Cur, Normal, Selecting, placeholders::_1));


	while (Cur->nxt != NewNode) {

		action.push_back(vector<function<void(int)> >());

		if (Cur == Head)
			action.back().push_back(bind(&DLL::MoveHighlight, this, 0, 1, placeholders::_1));
		else
			action.back().push_back(bind(&DLL::MoveHighlight, this, 2, 1, placeholders::_1));
		action.back().push_back(bind(&DLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
		action.back().push_back(bind(&DLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
		action.back().push_back(bind(&DLL::setNodeState, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::TitleDisappear, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::drawListExcept, this, NewNode, placeholders::_1));
		action.back().push_back(bind(&DLL::drawArrowFlow, this, Cur, placeholders::_1));

		action.push_back(vector<function<void(int)> >());

		action.back().push_back(bind(&DLL::MoveHighlight, this, 1, 2, placeholders::_1));
		action.back().push_back(bind(&DLL::drawListExcept, this, NewNode, placeholders::_1));
		action.back().push_back(bind(&DLL::TitleAppear, this, Cur->nxt, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::ChangeState, this, Cur, Selecting, Visited, placeholders::_1));
		action.back().push_back(bind(&DLL::ChangeState, this, Cur->nxt, Normal, Selecting, placeholders::_1));

		Cur = Cur->nxt;
	}

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&DLL::MoveHighlight, this, 2, 3, placeholders::_1));
	action.back().push_back(bind(&DLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
	action.back().push_back(bind(&DLL::setNodeState, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&DLL::ConnectNode, this, NewNode->prev, NewNode, placeholders::_1));
	action.back().push_back(bind(&DLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(bind(&DLL::TitleAppear, this, NewNode, New, placeholders::_1));
	action.back().push_back(bind(&DLL::NodeAppear, this, NewNode, placeholders::_1));


	initProgress();
}

bool DLL::insertNode(int i, int v)
{
	initList();

	if (i > NodeNumber || i < 0 || NodeNumber == maxNodeNumber) {
		Signal = Pending;
		return 0;
	}

	Node* Cur = Head;
	Node* NewNode = new Node(v);
	NewNode->NodeType = 1;
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

	TextArea->setText(tgui::String("Node Cur = Head\nfor(k = 0; k < i - 1; k++)\n     Cur = Cur.next\nNode Aft = Cur.next\nNode NewNode = new Node(v)\nNewNode.next = Aft, Aft.prev = NewNode\nCur.next = NewNode, NewNode.prev = Cur"));

	//run to node
	Cur = Head;
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&DLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(bind(&DLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(bind(&DLL::TitleAppear, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, Cur, Normal, Selecting, placeholders::_1));


	while (Cur->nxt != NewNode) {
		action.push_back(vector<function<void(int)> >());

		if (Cur == Head)
			action.back().push_back(bind(&DLL::MoveHighlight, this, 0, 1, placeholders::_1));
		else
			action.back().push_back(bind(&DLL::MoveHighlight, this, 2, 1, placeholders::_1));

		action.back().push_back(bind(&DLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
		action.back().push_back(bind(&DLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
		action.back().push_back(bind(&DLL::setNodeState, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::TitleDisappear, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::drawListExcept, this, NewNode, placeholders::_1));
		action.back().push_back(bind(&DLL::drawArrowFlow, this, Cur, placeholders::_1));

		action.push_back(vector<function<void(int)> >());

		action.back().push_back(bind(&DLL::MoveHighlight, this, 1, 2, placeholders::_1));
		action.back().push_back(bind(&DLL::drawListExcept, this, NewNode, placeholders::_1));
		action.back().push_back(bind(&DLL::TitleAppear, this, Cur->nxt, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::ChangeState, this, Cur, Selecting, Visited, placeholders::_1));
		action.back().push_back(bind(&DLL::ChangeState, this, Cur->nxt, Normal, Selecting, placeholders::_1));

		Cur = Cur->nxt;
	}

	//Change Aft State
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&DLL::MoveHighlight, this, 2, 3, placeholders::_1));
	action.back().push_back(bind(&DLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
	action.back().push_back(bind(&DLL::setNodeState, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&DLL::drawListExcept, this, NewNode, placeholders::_1));

	action.back().push_back(bind(&DLL::TitleAppear, this, NewNode->nxt, Next, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, NewNode->nxt, Normal, Next, placeholders::_1));

	//NewNode appears
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&DLL::MoveHighlight, this, 3, 4, placeholders::_1));
	action.back().push_back(bind(&DLL::setNodeState, this, NewNode, New, placeholders::_1));
	action.back().push_back(bind(&DLL::setNodeState, this, NewNode->nxt, Next, placeholders::_1));
	action.back().push_back(bind(&DLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(bind(&DLL::NodeAppear, this, NewNode, placeholders::_1));

	//Connect Nodes	
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&DLL::MoveHighlight, this, 4, 5, placeholders::_1));
	action.back().push_back(bind(&DLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
	action.back().push_back(bind(&DLL::setNodeState, this, NewNode, New, placeholders::_1));
	action.back().push_back(bind(&DLL::ConnectNode, this, NewNode, NewNode->nxt, placeholders::_1));
	action.back().push_back(bind(&DLL::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(bind(&DLL::drawNode, this, NewNode, placeholders::_1));

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&DLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
	action.back().push_back(bind(&DLL::MoveHighlight, this, 5, 6, placeholders::_1));
	action.back().push_back(bind(&DLL::setNodeState, this, NewNode, New, placeholders::_1));
	action.back().push_back(bind(&DLL::DisconnectNode, this, Cur, NewNode->nxt, placeholders::_1));
	action.back().push_back(bind(&DLL::ConnectNode, this, Cur, NewNode, placeholders::_1));
	action.back().push_back(bind(&DLL::drawListPartial, this, Head, NewNode->prev, placeholders::_1));
	action.back().push_back(bind(&DLL::drawListPartial, this, NewNode, Tail, placeholders::_1));
	action.back().push_back(bind(&DLL::drawNode, this, NewNode, placeholders::_1));

	//Move NewNode up
	action.push_back(vector<function<void(int) > >());
	action.back().push_back(bind(&DLL::TitleAppear, this, NewNode, New, placeholders::_1));
	action.back().push_back(bind(&DLL::setNodeState, this, NewNode, New, placeholders::_1));
	action.back().push_back(bind(&DLL::MoveNode, this, NewNode, Cur->Pos.x + 95, Cur->Pos.y + 100, Cur->Pos.x + 95, Cur->Pos.y, placeholders::_1));
	action.back().push_back(bind(&DLL::SlideNodes, this, NewNode->nxt, Cur->Pos.x + 95, Cur->Pos.y, Cur->Pos.x + 95 + 95, DefaultPosY, placeholders::_1));
	action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));

	initProgress();

	return 1;
}

void DLL::removeAtBeginning() {
	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("Node Cur = Head\nHead = Cur.next\nif(Head)\n	Head.prev = null\ndelete Cur"));

	Node* Dell = Head;
	Dell->NumberInList = 0;

	for (Node* tmp = Dell->nxt; tmp; tmp = tmp->nxt)
		tmp->NumberInList--;

	//tmp = Head
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&DLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, Dell, Normal, Selecting, placeholders::_1));
	action.back().push_back(bind(&DLL::TitleAppear, this, Dell, Selecting, placeholders::_1));

	//Next = Head

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&DLL::MoveHighlight, this, 0, 1, placeholders::_1));
	action.back().push_back(bind(&DLL::SetNodesNormal, this, Dell, Tail, placeholders::_1));
	action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, Dell, Selecting, Remove, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, Dell->nxt, Normal, Next, placeholders::_1));
	action.back().push_back(bind(&DLL::TitleAppear, this, Dell->nxt, Next, placeholders::_1));
	action.back().push_back(bind(&DLL::TitleDisappear, this, Dell, Selecting, placeholders::_1));

	if (Dell->nxt) {
		action.push_back(vector<function<void(int)> >());
		action.back().push_back(bind(&DLL::MoveHighlight, this, 1, 3, placeholders::_1));
		action.back().push_back(bind(&DLL::DisconnectNode, this, Dell, Dell->nxt, placeholders::_1));
		action.back().push_back(bind(&DLL::drawListExcept, this, Dell, placeholders::_1));
		action.back().push_back(bind(&DLL::drawListPartial, this, Dell, Dell, placeholders::_1));
		action.back().push_back(bind(&DLL::TitleDisappear, this, Dell->nxt, Next, placeholders::_1));
	}

	//Update Nodes position
	action.push_back(vector<function<void(int)> >());

	if (Dell->nxt) {
		action.back().push_back(bind(&DLL::MoveHighlight, this, 3, 4, placeholders::_1));
	}
	else
		action.back().push_back(bind(&DLL::MoveHighlight, this, 1, 4, placeholders::_1));
	action.back().push_back(bind(&DLL::SetNodesNormal, this, Dell, Tail, placeholders::_1));
	action.back().push_back(bind(&DLL::setNodeState, this, Dell, Remove, placeholders::_1));
	action.back().push_back(bind(&DLL::drawListExcept, this, Dell, placeholders::_1));
	action.back().push_back(bind(&DLL::NodeDisappear, this, Dell, placeholders::_1));
	action.back().push_back(bind(&DLL::SlideNodes, this, Dell->nxt, Dell->Pos.x + 95, DefaultPosY, Dell->Pos.x, DefaultPosY, placeholders::_1));

	//Delete
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&DLL::DeleteNode, this, Dell, placeholders::_1));

	initProgress();

	return;
}

void DLL::removeAtEnd() {
	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("Node Cur = Head, Aft = Head.next\nwhile(Aft.next != null)\n    Cur = Cur.next, Aft = Aft.next\nCur.next = null\ndelete Aft"));

	//Run to node
	Node* Cur = Head;
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&DLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&DLL::TitleAppear, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&DLL::TitleAppear, this, Cur->nxt, Next, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, Cur, Normal, Selecting, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, Cur->nxt, Normal, Next, placeholders::_1));

	while (Cur->nxt->nxt != nullptr) {
		action.push_back(vector<function<void(int)> >());

		if (Cur == Head)
			action.back().push_back(bind(&DLL::MoveHighlight, this, 0, 1, placeholders::_1));
		else
			action.back().push_back(bind(&DLL::MoveHighlight, this, 2, 1, placeholders::_1));

		action.back().push_back(bind(&DLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
		action.back().push_back(bind(&DLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
		action.back().push_back(bind(&DLL::setNodeState, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&DLL::drawArrowFlow, this, Cur, placeholders::_1));
		action.back().push_back(bind(&DLL::TitleDisappear, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::TitleDisappear, this, Cur->nxt, Next, placeholders::_1));

		action.push_back(vector<function<void(int)> >());

		action.back().push_back(bind(&DLL::MoveHighlight, this, 1, 2, placeholders::_1));
		action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&DLL::ChangeState, this, Cur, Selecting, Visited, placeholders::_1));
		action.back().push_back(bind(&DLL::ChangeState, this, Cur->nxt, Next, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::ChangeState, this, Cur->nxt->nxt, Normal, Next, placeholders::_1));
		action.back().push_back(bind(&DLL::TitleAppear, this, Cur->nxt, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::TitleAppear, this, Cur->nxt->nxt, Next, placeholders::_1));

		Cur = Cur->nxt;
	}

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&DLL::MoveHighlight, this, 2, 3, placeholders::_1));
	action.back().push_back(bind(&DLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
	action.back().push_back(bind(&DLL::DisconnectNode, this, Cur, Cur->nxt, placeholders::_1));
	action.back().push_back(bind(&DLL::drawListExcept, this, Cur->nxt, placeholders::_1));
	action.back().push_back(bind(&DLL::drawNode, this, Cur->nxt, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, Cur->nxt, Next, Remove, placeholders::_1));

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&DLL::MoveHighlight, this, 3, 4, placeholders::_1));
	action.back().push_back(bind(&DLL::drawListExcept, this, Cur->nxt, placeholders::_1));
	action.back().push_back(bind(&DLL::NodeDisappear, this, Cur->nxt, placeholders::_1));

	//Delete
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&DLL::DeleteNode, this, Cur->nxt, placeholders::_1));

	initProgress();

	return;
}

bool DLL::removeNode(int i)
{
	initList();
	if (i >= NodeNumber || i < 0) {
		Signal = Pending;
		return 0;
	}

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

	TextArea->setText(tgui::String("Node Cur = Head\nfor (k = 0; k < i - 1; k++)\n    Cur = Cur.next\nNode Del = Cur.next\nNode Aft = Del.next\nCur.next = Aft, Aft.prev = Cur\ndelete Del"));

	for (Node* tmp = Dell->nxt; tmp; tmp = tmp->nxt)
		tmp->NumberInList--;

	//Run to node
	Cur = Head;
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&DLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&DLL::TitleAppear, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, Cur, Normal, Selecting, placeholders::_1));

	while (Cur->nxt != Dell) {
		action.push_back(vector<function<void(int)> >());

		if (Cur == Head)
			action.back().push_back(bind(&DLL::MoveHighlight, this, 0, 1, placeholders::_1));
		else
			action.back().push_back(bind(&DLL::MoveHighlight, this, 2, 1, placeholders::_1));

		action.back().push_back(bind(&DLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
		action.back().push_back(bind(&DLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
		action.back().push_back(bind(&DLL::setNodeState, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::TitleDisappear, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&DLL::drawArrowFlow, this, Cur, placeholders::_1));


		action.push_back(vector<function<void(int)> >());

		action.back().push_back(bind(&DLL::MoveHighlight, this, 1, 2, placeholders::_1));
		action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&DLL::TitleAppear, this, Cur->nxt, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::ChangeState, this, Cur, Selecting, Visited, placeholders::_1));
		action.back().push_back(bind(&DLL::ChangeState, this, Cur->nxt, Normal, Selecting, placeholders::_1));

		Cur = Cur->nxt;
	}

	//Change remove node
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&DLL::MoveHighlight, this, 2, 3, placeholders::_1));
	action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, Dell, Normal, Remove, placeholders::_1));

	//Aft = Del.next
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&DLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
	action.back().push_back(bind(&DLL::MoveHighlight, this, 3, 4, placeholders::_1));
	action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, Dell->nxt, Normal, Next, placeholders::_1));
	action.back().push_back(bind(&DLL::TitleAppear, this, Dell->nxt, Next, placeholders::_1));

	//Move Cur down
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&DLL::MoveHighlight, this, 4, 5, placeholders::_1));
	action.back().push_back(bind(&DLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
	action.back().push_back(bind(&DLL::setNodeState, this, Dell, Remove, placeholders::_1));
	action.back().push_back(bind(&DLL::MoveNode, this, Dell, Dell->prev->Pos.x + 95, DefaultPosY, Dell->prev->Pos.x + 95, DefaultPosY + 100, placeholders::_1));
	action.back().push_back(bind(&DLL::ConnectNode, this, Dell->prev, Dell->nxt, placeholders::_1));
	action.back().push_back(bind(&DLL::DisconnectNode, this, Dell->prev, Dell, placeholders::_1));
	action.back().push_back(bind(&DLL::drawListPartial, this, Head, Dell->prev, placeholders::_1));
	action.back().push_back(bind(&DLL::drawListPartial, this, Dell, Tail, placeholders::_1));

	//Update Nodes position
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&DLL::MoveHighlight, this, 5, 6, placeholders::_1));
	action.back().push_back(bind(&DLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
	action.back().push_back(bind(&DLL::setNodeState, this, Dell, Remove, placeholders::_1));
	action.back().push_back(bind(&DLL::DisconnectNode, this, Dell, Dell->nxt, placeholders::_1));
	action.back().push_back(bind(&DLL::drawListExcept, this, Dell, placeholders::_1));
	action.back().push_back(bind(&DLL::NodeDisappear, this, Dell, placeholders::_1));
	action.back().push_back(bind(&DLL::SlideNodes, this, Dell->nxt, Dell->Pos.x + 95, DefaultPosY, Dell->Pos.x, DefaultPosY, placeholders::_1));

	//Delete
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&DLL::DeleteNode, this, Dell, placeholders::_1));

	initProgress();

	return 1;
}



void DLL::searchNode(tgui::String v)
{
	initList();
	if (!Head)
		return;

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("index = 0, Cur = Head\nwhile(Cur.value != v)\n		index++, Cur = Cur.next\nif (Cur == null)\n		return NOT_FOUND\nreturn index"));

	string s = v.toStdString();

	//Run to node
	Node* Cur = Head;
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&DLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&DLL::TitleAppear, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, Cur, Normal, Selecting, placeholders::_1));

	while (Cur != nullptr && Cur->Val != s) {

		action.push_back(vector<function<void(int)> >());

		if (Cur == Head)
			action.back().push_back(bind(&DLL::MoveHighlight, this, 0, 1, placeholders::_1));
		else
			action.back().push_back(bind(&DLL::MoveHighlight, this, 2, 1, placeholders::_1));

		action.back().push_back(bind(&DLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
		action.back().push_back(bind(&DLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
		action.back().push_back(bind(&DLL::setNodeState, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::TitleDisappear, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&DLL::drawArrowFlow, this, Cur, placeholders::_1));


		if (Cur->nxt) {
			action.push_back(vector<function<void(int)> >());
			action.back().push_back(bind(&DLL::MoveHighlight, this, 1, 2, placeholders::_1));
			action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
			action.back().push_back(bind(&DLL::TitleAppear, this, Cur->nxt, Selecting, placeholders::_1));
			action.back().push_back(bind(&DLL::ChangeState, this, Cur, Selecting, Visited, placeholders::_1));
			action.back().push_back(bind(&DLL::ChangeState, this, Cur->nxt, Normal, Selecting, placeholders::_1));
		}

		Cur = Cur->nxt;
	}

	if (!Cur) {
		action.push_back(vector<function<void(int)> >());
		action.back().push_back(bind(&DLL::MoveHighlight, this, 1, 4, placeholders::_1));
		action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&DLL::ChangeState, this, Tail, Selecting, Visited, placeholders::_1));

		initProgress();
		return;
	}

	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&DLL::MoveHighlight, this, 2, 5, placeholders::_1));
	action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
	//action.back().push_back(bind(&DLL::setNodeState, this, Cur, New, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, Cur, Selecting, New, placeholders::_1));

	initProgress();
}

void DLL::updateNode(int i, int v)
{
	initList();

	if (i < 0 || i >= NodeNumber)
		return;

	Node* Cur = Head;
	for (int j = 0; j < i; j++)
		Cur = Cur->nxt;

	String preVal = Cur->Val;

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("Node Cur = Head\nfor(k = 0; k < i; k++)\n	Cur = Cur.next\nCur.value = v"));

	Cur = Head;
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&DLL::HighlightAppear, this, placeholders::_1));
	action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&DLL::TitleAppear, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeState, this, Cur, Normal, Selecting, placeholders::_1));


	for (int j = 0; j < i; j++) {
		action.push_back(vector<function<void(int)> >());

		if (j == 0)
			action.back().push_back(bind(&DLL::MoveHighlight, this, 0, 1, placeholders::_1));
		else
			action.back().push_back(bind(&DLL::MoveHighlight, this, 2, 1, placeholders::_1));

		action.back().push_back(bind(&DLL::SetNodesNormal, this, Cur, Tail, placeholders::_1));
		action.back().push_back(bind(&DLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
		action.back().push_back(bind(&DLL::setNodeState, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::TitleDisappear, this, Cur, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&DLL::drawArrowFlow, this, Cur, placeholders::_1));

		action.push_back(vector<function<void(int)> >());

		action.back().push_back(bind(&DLL::MoveHighlight, this, 1, 2, placeholders::_1));
		action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
		action.back().push_back(bind(&DLL::TitleAppear, this, Cur->nxt, Selecting, placeholders::_1));
		action.back().push_back(bind(&DLL::ChangeState, this, Cur, Selecting, Visited, placeholders::_1));
		action.back().push_back(bind(&DLL::ChangeState, this, Cur->nxt, Normal, Selecting, placeholders::_1));

		Cur = Cur->nxt;

	}

	//Change Aft State
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&DLL::MoveHighlight, this, 2, 3, placeholders::_1));
	action.back().push_back(bind(&DLL::setNodeState, this, Cur->prev, Visited, placeholders::_1));
	action.back().push_back(bind(&DLL::drawList, this, placeholders::_1));
	action.back().push_back(bind(&DLL::TitleAppear, this, Cur, Selecting, placeholders::_1));
	action.back().push_back(bind(&DLL::ChangeValue, this, Cur, preVal, String(to_string(v)), placeholders::_1));

	initProgress();
}

void DLL::initProgress() {
	tgui::Panel::Ptr EditPanel = gui.get<tgui::Panel>("EditPanel");

	tgui::Slider::Ptr ProgressThumb = EditPanel->get<tgui::Slider>("ProgressThumb");
	tgui::ProgressBar::Ptr Progress = EditPanel->get<tgui::ProgressBar>("ProgressStep");

	if (Signal == Removing) {
		ProgressThumb->setMaximum(action.size() - 1);
		Progress->setMaximum(action.size() - 1);
	}
	else {
		ProgressThumb->setMaximum(action.size());
		Progress->setMaximum(action.size());
	}


	ProgressThumb->setValue(0);
	Progress->setValue(0);

	ShowDirection = 0;
	timer.restart();
	Last = 0;
	Elapsed = 0;
}

void DLL::initButtons()
{
	tgui::Picture::Ptr Background = gui.get<tgui::Picture>("Background");
	Background->setRenderer(theme.getRenderer("Background"));

	tgui::ComboBox::Ptr StructList = gui.get<tgui::ComboBox>("ComboBox1");
	StructList->setRenderer(theme.getRenderer("ComboBox"));

	tgui::Button::Ptr InsertEx = gui.get<tgui::Button>("InsertEx");
	InsertEx->setRenderer(theme.getRenderer("ExButton"));
	tgui::Button::Ptr InsertMode = gui.get<tgui::Button>("InsertMode");
	InsertMode->setRenderer(theme.getRenderer("SemiButton"));
	tgui::Button::Ptr InsertButton = gui.get<tgui::Button>("InsertButton");
	InsertButton->setRenderer(theme.getRenderer("Button"));
	tgui::EditBox::Ptr InsertPos = gui.get<tgui::EditBox>("InsertPos");
	InsertPos->setRenderer(theme.getRenderer("EditBox"));
	tgui::EditBox::Ptr InsertVal = gui.get<tgui::EditBox>("InsertVal");
	InsertVal->setRenderer(theme.getRenderer("EditBox"));

	tgui::Button::Ptr DeleteEx = gui.get<tgui::Button>("DeleteEx");
	DeleteEx->setRenderer(theme.getRenderer("ExButton"));
	tgui::Button::Ptr DeleteMode = gui.get<tgui::Button>("DeleteMode");
	DeleteMode->setRenderer(theme.getRenderer("SemiButton"));
	tgui::Button::Ptr DeleteButton = gui.get<tgui::Button>("DeleteButton");
	DeleteButton->setRenderer(theme.getRenderer("Button"));
	tgui::EditBox::Ptr DeletePos = gui.get<tgui::EditBox>("DeletePos");
	DeletePos->setRenderer(theme.getRenderer("EditBox"));

	tgui::Button::Ptr CreateButton = gui.get<tgui::Button>("CreateButton");
	CreateButton->setRenderer(theme.getRenderer("Button"));
	tgui::Button::Ptr InputGen = gui.get<tgui::Button>("InputGen");
	InputGen->setRenderer(theme.getRenderer("SemiButton"));
	tgui::EditBox::Ptr UserInput = gui.get<tgui::EditBox>("EditBox1");
	UserInput->setRenderer(theme.getRenderer("EditBox"));
	tgui::Button::Ptr UserInputEx = gui.get<tgui::Button>("UserInputEx");
	UserInputEx->setRenderer(theme.getRenderer("ExButton"));

	tgui::Button::Ptr SearchNode = gui.get<tgui::Button>("SearchNode");
	SearchNode->setRenderer(theme.getRenderer("Button"));
	tgui::Button::Ptr SearchEx = gui.get<tgui::Button>("SearchEx");
	SearchEx->setRenderer(theme.getRenderer("ExButton"));
	tgui::EditBox::Ptr SearchVal = gui.get<tgui::EditBox>("SearchVal");
	SearchVal->setRenderer(theme.getRenderer("EditBox"));

	tgui::Button::Ptr UpdateNode = gui.get<tgui::Button>("UpdateNode");
	UpdateNode->setRenderer(theme.getRenderer("Button"));
	tgui::Button::Ptr UpdateEx = gui.get<tgui::Button>("UpdateEx");
	UpdateEx->setRenderer(theme.getRenderer("ExButton"));
	tgui::EditBox::Ptr UpdateVal = gui.get<tgui::EditBox>("UpdateVal");
	UpdateVal->setRenderer(theme.getRenderer("EditBox"));
	tgui::EditBox::Ptr UpdatePos = gui.get<tgui::EditBox>("UpdatePos");
	UpdatePos->setRenderer(theme.getRenderer("EditBox"));


	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	PseudoCode->loadWidgetsFromFile("assets/themes/CodeWindow.txt");
	PseudoCode->setRenderer(theme.getRenderer("CodeWindow"));
	tgui::TextArea::Ptr CodeLines = PseudoCode->get<tgui::TextArea>("TextArea1");
	CodeLines->setRenderer(theme.getRenderer("CodeLines"));
	tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");
	TextHighlight->setRenderer(theme.getRenderer("TextHighlight"));


	tgui::Button::Ptr SlideIn = gui.get<tgui::Button>("SlideIn");
	tgui::Button::Ptr SlideOut = gui.get<tgui::Button>("SlideOut");
	tgui::Panel::Ptr EditPanel = gui.get<tgui::Panel>("EditPanel");
	EditPanel->loadWidgetsFromFile("assets/themes/ControlPanel.txt");
	EditPanel->setRenderer(theme.getRenderer("EditPanel"));

	tgui::Slider::Ptr Speed = EditPanel->get<tgui::Slider>("Speed");
	Speed->setRenderer(theme.getRenderer("SpeedSlider"));
	tgui::Slider::Ptr ProgressThumb = EditPanel->get<tgui::Slider>("ProgressThumb");
	ProgressThumb->setRenderer(theme.getRenderer("ProgressThumb"));
	tgui::ProgressBar::Ptr Progress = EditPanel->get<tgui::ProgressBar>("ProgressStep");
	Progress->setRenderer(theme.getRenderer("ProgressBar"));

	tgui::Button::Ptr Begin = EditPanel->get<tgui::Button>("Begin");
	Begin->setRenderer(theme.getRenderer("BackwardToBegin"));
	tgui::Button::Ptr Final = EditPanel->get<tgui::Button>("Final");
	Final->setRenderer(theme.getRenderer("ForwardToFinal"));
	tgui::Button::Ptr Forward = EditPanel->get<tgui::Button>("Forward");
	Forward->setRenderer(theme.getRenderer("Forward"));
	tgui::Button::Ptr Backward = EditPanel->get<tgui::Button>("Backward");
	Backward->setRenderer(theme.getRenderer("Backward"));
	tgui::Button::Ptr Play = EditPanel->get<tgui::Button>("PlayButton");
	Play->setRenderer(theme.getRenderer("PlayButton"));

	tgui::Label::Ptr SpeedIndicator = EditPanel->get<tgui::Label>("SpeedIndicator");
	tgui::Label::Ptr SpeedLabel = EditPanel->get<tgui::Label>("Label1");
	tgui::Label::Ptr ThemeLabel = EditPanel->get<tgui::Label>("Label2");
	tgui::Label::Ptr Color1 = EditPanel->get<tgui::Label>("Color1");
	tgui::Label::Ptr Color2 = EditPanel->get<tgui::Label>("Color2");
	SpeedIndicator->setRenderer(theme.getRenderer("LightText"));
	SpeedLabel->setRenderer(theme.getRenderer("LightText"));
	ThemeLabel->setRenderer(theme.getRenderer("LightText"));
	Color1->setRenderer(theme.getRenderer("LightText"));
	Color2->setRenderer(theme.getRenderer("LightText"));

	tgui::RadioButton::Ptr Theme1 = EditPanel->get<tgui::RadioButton>("Theme1");
	Theme1->setRenderer(theme.getRenderer("RadioButton"));
	tgui::RadioButton::Ptr Theme2 = EditPanel->get<tgui::RadioButton>("Theme2");
	Theme2->setRenderer(theme.getRenderer("RadioButton"));

	if (ThemeNum == 0) {
		Theme1->setChecked(1);
	}
	else {
		Theme2->setChecked(1);
	}

	InsertModes = 0;
	DeleteModes = 0;
	GenModes = 0;
	Speed->setValue(2);
	EditPanel->setVisible(ControlVisible);
	SlideIn->setVisible(ControlVisible);

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

	InputGen->onPress([=] {
		GenModes = (GenModes + 1) % 3;

		switch (GenModes) {
		case 0:
			InputGen->setText(tgui::String("Manual Input"));

			UserInput->setVisible(1);
			UserInputEx->setPosition({ "EditBox1.right + 10", "EditBox1.top" });
			break;

		case 1:
			InputGen->setText(tgui::String("Random Input"));

			UserInput->setVisible(0);
			UserInputEx->setPosition({ "InputGen.right + 10", "InputGen.top + 5" });
			break;

		case 2:
			InputGen->setText(tgui::String("Browse"));

			UserInput->setVisible(0);
			UserInputEx->setPosition({ "InputGen.right + 10", "InputGen.top + 5" });
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


	UserInputEx->onPress([=] {
		ClearAction();
		Signal = Pending;

		switch (GenModes) {
		case 0:
		{
			tgui::String s = UserInput->getText();
			genList(s);
			break;
		}
		case 1:
		{
			genList();
			break;
		}
		case 2:
		{
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			ofn.lpstrDefExt = L"txt";

			tgui::String S;

			if (GetOpenFileName(&ofn) == TRUE) {
				// User selected a file
				std::ifstream file(ofn.lpstrFile);
				std::string line;
				while (std::getline(file, line)) {
					S.append(line);
				}

				file.close();
			}
			else {
				// User cancelled the dialog
			}

			genList(S);

			break;
		}
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
		Elapsed = 0;
		Last = 0;

		int Pos = InsertPos->getText().toInt();
		int Val = InsertVal->getText().toInt();

		if (InsertModes == 0)
			insertNode(Pos, Val);
		else if (InsertModes == 1)
			insertNode(0, Val);
		else if (InsertModes == 2)
			insertNode(NodeNumber, Val);
		});

	DeleteEx->onPress([=] {
		if (NodeNumber == 0)
			return;

		Signal = Removing;
		timer.restart();

		ClearAction();
		ShowDirection = 0;
		CurStep = 0;
		Last = 0;
		Elapsed = 0;

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
		Last = 0;
		Elapsed = 0;

		tgui::String Val = SearchVal->getText();

		searchNode(Val);
		});

	UpdateEx->onPress([=] {
		Signal = Searching;
		timer.restart();

		ClearAction();
		ShowDirection = 0;
		CurStep = 0;
		Last = 0;
		Elapsed = 0;

		tgui::String Val = UpdateVal->getText();
		tgui::String Pos = UpdatePos->getText();

		updateNode(Pos.toInt(), Val.toInt());
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
		ControlVisible = 1;
		EditPanel->showWithEffect(tgui::ShowAnimationType::SlideFromRight, 500);
		SlideIn->showWithEffect(tgui::ShowAnimationType::SlideFromRight, 500);
		});

	SlideIn->onClick([=] {
		ControlVisible = 0;
		EditPanel->hideWithEffect(tgui::ShowAnimationType::SlideToRight, 500);
		SlideIn->hideWithEffect(tgui::ShowAnimationType::SlideToRight, 500);
		});

	Speed->onValueChange([=] {
		tgui::String s(0.5f + 0.25f * Speed->getValue());

		SpeedIndicator->setText(tgui::String("x") + s);
		double tmpDur = Duration;
		Duration = (int)(700 / (0.5f + 0.25f * Speed->getValue()));
		Elapsed = (int)(((double)Elapsed / tmpDur) * Duration);
		Last = Elapsed;
		});

	ProgressThumb->onValueChange([=] {
		if (action.empty()) {
			ProgressThumb->setValue(0);
			return;
		}

		int NewProgress = ProgressThumb->getValue();

		int CurProgress = CurStep;
		if (ShowDirection == 0)
			CurProgress++;

		Progress->setValue(ProgressThumb->getValue());

		//timer.restart();
		//Last = 0;

		if (CurProgress < NewProgress) {
			ShowDirection = 0;
			for (; CurStep < NewProgress - 1; CurStep++)
				for (int i = 0; i < action[CurStep].size(); i++) {
					action[CurStep][i](Duration);
				}

			Last = 0;
			timer.restart();
		}

		if (CurProgress > NewProgress) {
			for (; CurStep > NewProgress; CurStep--)
				for (int i = 0; i < action[CurStep].size(); i++) {
					action[CurStep][i](0);
				}

			if (ShowMode) {
				ShowDirection = 1;
				Last = Duration;
				Elapsed = Duration;
				timer.restart();
			}
			else {
				ShowDirection = 0;
				Last = 0;
				Elapsed = 0;
				timer.restart();
			}
		}

		});

	Play->onPress([=] {
		ShowMode = (1 - ShowMode);

		Last = Elapsed;
		timer.restart();

		if (ShowMode)
			Play->setRenderer(theme.getRenderer("PlayButton"));
		else {
			ShowDirection = 0;
			Play->setRenderer(theme.getRenderer("PauseButton"));
		}
		});

	Begin->onPress([=] {
		ProgressThumb->setValue(0);
		});

	Final->onPress([=] {
		ProgressThumb->setValue(ProgressThumb->getMaximum());
		});

	Forward->onPress([=] {
		ShowMode = 1;
		ProgressThumb->setValue(ProgressThumb->getValue() + 1);
		});

	Backward->onPress([=] {
		ShowMode = 1;
		ProgressThumb->setValue(ProgressThumb->getValue() - 1);
		});

	Theme1->onCheck([=] {
		MainColor = &VSPurple;
		ThemeNum = 0;
		theme.load("assets/themes/CyberPurple.txt");
		});

	Theme2->onCheck([=] {
		MainColor = &Fulvous;
		ThemeNum = 1;
		theme.load("assets/themes/ForestGreen.txt");
		});

	StructList->setSelectedItem(tgui::String("DLL"));

	StructList->onItemSelect([=] {
		tgui::String s = StructList->getSelectedItem();

		if (s != tgui::String("DLL")) {
			ClearAction();

			while (Head) {
				Node* tmp = Head->nxt;
				delete Head;
				Head = tmp;
			}
		}

		if (s == tgui::String("DLL"))
			State = _DLList;

		if (s == tgui::String("CLL"))
			State = _CLL;

		if (s == tgui::String("Stack"))
			State = _Stack;

		if (s == tgui::String("Queue"))
			State = _Queue;

		if (s == tgui::String("Array"))
			State = _Array;

		if (s == tgui::String("Dynamic Array"))
			State = _DArray;
		});
}

void DLL::HandleEvent(Event& e)
{
	switch (Signal) {
	case Searching:
	case Inserting:
		if (e.type == Event::KeyPressed) {

			switch (e.key.code) {
			case Keyboard::Right:
				if (ShowMode == 0)
					ShowMode = 1;

				if (ShowDirection == 1) {
					ShowDirection = 0;
					Last = Elapsed;
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
						Last = 0;
						timer.restart();
					}
				}

				break;

			case Keyboard::Left:
				if (ShowMode == 0)
					ShowMode = 1;

				if (ShowDirection == 0) {
					ShowDirection = 1;
					Last = Elapsed;
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
						Last = Duration;
						timer.restart();
					}
				}
				break;

			default:
				break;
			}
		}
		break;

	case Removing:

		if (e.type == Event::KeyPressed) {

			switch (e.key.code) {
			case Keyboard::Right:
				if (ShowMode == 0)
					ShowMode = 1;

				if (ShowDirection == 1) {
					ShowDirection = 0;
					Last = Elapsed;
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
						Last = 0;
						timer.restart();
					}
				}

				break;

			case Keyboard::Left:
				if (ShowMode == 0)
					ShowMode = 1;

				if (ShowDirection == 0) {
					ShowDirection = 1;
					Last = Elapsed;
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
						Last = Duration;
						timer.restart();
					}
				}
				break;

			default:
				break;
			}
		}
		break;
	}
}


void DLL::interactDLL()
{
	app.clear();
	gui.draw();

	tgui::Panel::Ptr EditPanel = gui.get<tgui::Panel>("EditPanel");

	tgui::Slider::Ptr ProgressThumb = EditPanel->get<tgui::Slider>("ProgressThumb");
	tgui::ProgressBar::Ptr Progress = EditPanel->get<tgui::ProgressBar>("ProgressStep");
	tgui::Button::Ptr Play = EditPanel->get<tgui::Button>("PlayButton");

	Event e;

	Elapsed = timer.getElapsedTime().asMilliseconds();

	if (ShowDirection == 1) {
		Elapsed = max(0, Last - Elapsed);
		ProgressThumb->setValue(CurStep);
	}
	else {
		ProgressThumb->setValue(CurStep + 1);
		Elapsed = min(Duration, Last + Elapsed);
	}

	if (ShowMode)
		Play->setRenderer(theme.getRenderer("PlayButton"));
	else
		Play->setRenderer(theme.getRenderer("PauseButton"));

	while (app.pollEvent(e)) {
		if (e.type == Event::Closed) {
			app.close();
			State = _EndProgram;
			return;
		}
		gui.handleEvent(e);
		HandleEvent(e);
	}

	if (State != _DLList)
		return;

	switch (Signal) {
	case Pending:
		drawList(1);
		break;

	default:
		if (ShowMode == 0) {

			if (ShowDirection == 0 && Elapsed >= Duration) {

				if ((CurStep + 2 == (int)action.size() && Signal == Removing) || (CurStep + 1 == (int)action.size()))
					Elapsed = Duration;
				else {
					timer.restart();
					Last = 0;
					Elapsed = 0;

					CurStep++;
				}
			}
		}


		for (int i = 0; i < (int)action[CurStep].size(); i++) {
			action[CurStep][i](Elapsed);
		}

		break;

	}

	app.display();
}