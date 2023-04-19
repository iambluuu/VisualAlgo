#include "Stack.h"
#include <math.h>
#include <functional>

using namespace std;
using namespace sf;

int Stack::getSize()
{
	return NodeNumber;
}

void Stack::changePosition(Node* Cur, float x, float y)
{
	Cur->changeNodePosition(x, y);
}

void Stack::NodeAppear(Node* Cur, int Elapsed)
{
	Cur->drawNode((int)(255 * (double)Elapsed / Duration));
}

void Stack::NodeDisappear(Node* Cur, int Elapsed)
{
	NodeAppear(Cur, Duration - Elapsed);
}

void Stack::drawNode(Node* Cur, int Dummy)
{
	Cur->drawNode(255);
}

void Stack::setNodeState(Node* Cur, Nodestate NodeState, int Dummy)
{
	if (!Cur)
		return;

	if (NodeState == Visited)
		Cur->ArrowState = Visited;

	if (NodeState == New)
		Cur->ArrowState = New;

	Cur->NodeState = NodeState;
}

void Stack::MoveNode(Node* Cur, float CurX, float CurY, float NxtX, float NxtY, int Elapsed)
{
	if (!Cur)
		return;

	Cur->changeNodePosition(CurX + (NxtX - CurX) * (double)Elapsed / Duration, CurY + (NxtY - CurY) * (double)Elapsed / Duration);
}

void Stack::SlideNodes(Node* Cur, float CurX, float CurY, float NxtX, float NxtY, int Elapsed)
{
	if (!Cur)
		return;

	MoveNode(Cur, CurX, CurY, NxtX, NxtY, Elapsed);

	for (Node* tmp = Cur->nxt; tmp; tmp = tmp->nxt)
		tmp->changeNodePosition(tmp->prev->Pos.x, tmp->prev->Pos.y + 100);

}

void Stack::SetNodesNormal(Node* A, Node* B, int Dummy)
{
	if (!A || !B)
		return;

	for (Node* tmp = A; tmp != B; tmp = tmp->nxt) {
		tmp->ArrowState = Normal;
		//tmp->NodeState = Normal;
	}

	B->ArrowState = Normal;
}

void Stack::ClearAction()
{
	for (int i = 0; i < action.size(); i++)
		for (int j = 0; j < action[i].size(); j++)
			action[i][j](Duration);

	while (!action.empty())
		action.pop_back();
}

void Stack::drawList(int Dummy)
{
	//if (Head)
	//	app.draw(Head->Title);

	for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
		tmp->updateArrow(tmp->nxt);
		tmp->drawArrow();
		tmp->drawNode(255);
	}
}

void Stack::TitleAppear(Node* Cur, Nodestate NodeState, int Elapsed)
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

void Stack::TitleDisappear(Node* Cur, Nodestate NodeState, int Elapsed)
{
	TitleAppear(Cur, NodeState, Duration - Elapsed);
}

void Stack::HighlightAppear(int Line, int Elapsed)
{
	if (Elapsed >= Duration / 2)
		Elapsed = Duration / 2;

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");

	TextHighlight->setPosition({ 0, 4 * Line });
	TextHighlight->setVisible(1);
	TextHighlight->setInheritedOpacity(2 * (float)Elapsed / Duration);
}

void Stack::MoveHighlight(int CurLine, int NextLine, int Elapsed)
{
	if (Elapsed >= Duration / 2)
		Elapsed = Duration / 2;

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");

	int Distance = (int)(2 * (double)Elapsed / Duration * (NextLine - CurLine) * 26);

	TextHighlight->setPosition({ 0, 4 + CurLine * 26 + Distance });
}

void Stack::drawListPartial(Node* A, Node* B, int Dummy)
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

void Stack::drawListExcept(Node* ExceptNode, int Dummy)
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

void Stack::DeleteNode(Node*& Cur, int Dummy)
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

void Stack::initList() {
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

void Stack::genList()
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

	NodeNumber = rand() % maxNodeNumber + 1;

	while (NodeNumber == 0)
		NodeNumber = rand() % 10;

	Head = new Node(rand() % 100);
	Head->NumberInList = 0;
	Head->changeNodePosition(DefaultPosX, DefaultPosY - 100 * (NodeNumber - 1));

	Node* Cur = Head;

	for (int i = 1; i < NodeNumber; i++) {
		Cur->nxt = new Node(rand() % 100);
		Node* tmp = Cur;

		Cur = Cur->nxt;
		Cur->prev = tmp;
		Cur->NumberInList = i;
		Cur->changeNodePosition(tmp->Pos.x, tmp->Pos.y + 100);

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

void Stack::genList(const tgui::String s)
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
		Cur->changeNodePosition(tmp->Pos.x, tmp->Pos.y + 100);

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

void Stack::ChangeState(Node* Cur, Nodestate CurState, Nodestate NextState, int Elapsed)
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

void Stack::ChangeValue(Node* Cur, sf::String preVal, sf::String nextVal, int Elapsed)
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

void Stack::drawArrowFlow(Node* Cur, int Elapsed)
{
	if (!Cur->nxt)
		return;

	Cur->TmpArrow.setPosition(Cur->Arrow.getPosition());
	Cur->TmpArrow.setRotation(Cur->Arrow.getRotation());
	Cur->TmpArrow.setColor(*MainColor);

	int ArrowLength = (int)(Util::DistanceBetweenNodes(Cur->Pos, Cur->nxt->Pos) - 46);

	Cur->TmpArrow.setTextureRect(IntRect(100 - ArrowLength, 0, (int)(ArrowLength * (double)Elapsed / Duration), 10));
	app.draw(Cur->TmpArrow);

	if ((double)Elapsed / Duration >= 4.5 / 5) {
		Cur->ArrowState = Visited;
	}
}

void Stack::ConnectNode(Node* A, Node* B, int Elapsed)
{
	if (!B)
		return;

	A->updateArrow(B);
	A->ArrowState = New;

	int Length = (int)((Util::DistanceBetweenNodes(A->Pos, B->Pos) - 46) * Elapsed / Duration);
	A->Arrow.setTextureRect(IntRect(100 - Length, 0, Length, 10));
	A->drawArrow();

}

void Stack::DisconnectNode(Node* A, Node* B, int Elapsed)
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

void Stack::insertAtBeginning(Node*& NewNode)
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
		changePosition(NewNode, DefaultPosX, NewNode->nxt->Pos.y - 100);
	}
	else {
		changePosition(NewNode, DefaultPosX, DefaultPosY);
	}

	//NewNode appears

	action.push_back(vector<function<void(int) > >());

	action.back().push_back(bind(&Stack::HighlightAppear, this, 0, placeholders::_1));
	action.back().push_back(std::bind(&Stack::drawListExcept, this, NewNode, std::placeholders::_1));
	action.back().push_back(std::bind(&Stack::NodeAppear, this, NewNode, std::placeholders::_1));

	if (!Head->nxt) {
		action.push_back(vector<function<void(int) > >());

		action.back().push_back(std::bind(&Stack::drawList, this, placeholders::_1));
		action.back().push_back(std::bind(&Stack::MoveHighlight, this, 0, 2, placeholders::_1));
		action.back().push_back(std::bind(&Stack::TitleAppear, this, NewNode, New, placeholders::_1));
		return;
	}


	//Connect NewNode to Head
	action.push_back(vector<function<void(int) > >());

	action.back().push_back(std::bind(&Stack::MoveHighlight, this, 0, 1, placeholders::_1));
	action.back().push_back(std::bind(&Stack::drawListExcept, this, NewNode, placeholders::_1));
	action.back().push_back(std::bind(&Stack::drawNode, this, NewNode, placeholders::_1));
	action.back().push_back(std::bind(&Stack::ConnectNode, this, NewNode, NewNode->nxt, placeholders::_1));

	initProgress();

}

bool Stack::insertNode(int i, int v)
{
	initList();

	if (i > NodeNumber || i < 0 || NodeNumber == maxNodeNumber) {
		Signal = Pending;
		return 0;
	}

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

	return 1;
}

void Stack::removeAtBeginning() {
	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("Node Cur = Head\nHead = Cur.next\ndelete Cur"));

	Node* Dell = Head;
	Dell->NumberInList = 0;

	for (Node* tmp = Dell->nxt; tmp; tmp = tmp->nxt)
		tmp->NumberInList--;

	//tmp = Head
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&Stack::HighlightAppear, this, 0, placeholders::_1));
	action.back().push_back(bind(&Stack::drawList, this, placeholders::_1));
	action.back().push_back(bind(&Stack::ChangeState, this, Dell, Normal, Selecting, placeholders::_1));
	action.back().push_back(bind(&Stack::TitleAppear, this, Dell, Selecting, placeholders::_1));

	//Next = Head

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&Stack::MoveHighlight, this, 0, 1, placeholders::_1));
	action.back().push_back(bind(&Stack::SetNodesNormal, this, Dell, Tail, placeholders::_1));
	action.back().push_back(bind(&Stack::drawList, this, placeholders::_1));
	action.back().push_back(bind(&Stack::ChangeState, this, Dell, Normal, Remove, placeholders::_1));
	action.back().push_back(bind(&Stack::ChangeState, this, Dell->nxt, Normal, Next, placeholders::_1));
	action.back().push_back(bind(&Stack::TitleAppear, this, Dell->nxt, Next, placeholders::_1));
	action.back().push_back(bind(&Stack::TitleDisappear, this, Dell, Selecting, placeholders::_1));

	//Del disappear
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&Stack::MoveHighlight, this, 1, 2, placeholders::_1));
	action.back().push_back(bind(&Stack::SetNodesNormal, this, Dell, Tail, placeholders::_1));
	action.back().push_back(bind(&Stack::setNodeState, this, Dell, Remove, placeholders::_1));

	action.back().push_back(bind(&Stack::drawListExcept, this, Dell, placeholders::_1));
	action.back().push_back(bind(&Stack::DisconnectNode, this, Dell, Dell->nxt, placeholders::_1));
	action.back().push_back(bind(&Stack::NodeDisappear, this, Dell, placeholders::_1));
	
	//Delete
	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&Stack::DeleteNode, this, Dell, placeholders::_1));

	initProgress();

	return;
}

bool Stack::removeNode(int i)
{
	initList();
	if (i >= NodeNumber || i < 0) {
		Signal = Pending;
		return 0;
	}

	NodeNumber--;

	if (i == 0) {
		removeAtBeginning();
		return 1;
	}

	return 1;
}


void Stack::Peek()
{
	initList();

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("if empty, return NOT_FOUND\nreturn Head.value"));

	if (!Head) {
		action.push_back(vector<function<void(int)> >());

		action.back().push_back(bind(&Stack::HighlightAppear, this, 0, placeholders::_1));
		initProgress();
	}

	//Run to node
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&Stack::HighlightAppear, this, 1, placeholders::_1));
	action.back().push_back(bind(&Stack::drawList, this, placeholders::_1));
	action.back().push_back(bind(&Stack::TitleAppear, this, Head, Selecting, placeholders::_1));
	action.back().push_back(bind(&Stack::ChangeState, this, Head, Normal, New, placeholders::_1));

	initProgress();
}

void Stack::initProgress() {
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

void Stack::initButtons()
{
	tgui::Picture::Ptr Background = gui.get<tgui::Picture>("Background");
	Background->setRenderer(theme.getRenderer("Background"));

	tgui::ComboBox::Ptr StructList = gui.get<tgui::ComboBox>("ComboBox1");
	StructList->setRenderer(theme.getRenderer("ComboBox"));

	tgui::Button::Ptr InsertEx = gui.get<tgui::Button>("InsertEx");
	InsertEx->setRenderer(theme.getRenderer("ExButton"));
	tgui::Button::Ptr InsertButton = gui.get<tgui::Button>("InsertButton");
	InsertButton->setRenderer(theme.getRenderer("Button"));
	tgui::EditBox::Ptr InsertVal = gui.get<tgui::EditBox>("InsertVal");
	InsertVal->setRenderer(theme.getRenderer("EditBox"));

	tgui::Button::Ptr DeleteButton = gui.get<tgui::Button>("DeleteButton");
	DeleteButton->setRenderer(theme.getRenderer("Button"));

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



	Speed->setValue(2);

	InsertButton->onPress([=] {
		InsertVal->setVisible(1 - InsertVal->isVisible());

		InsertEx->setVisible(1 - InsertEx->isVisible());
		});

	DeleteButton->onPress([=] {
		Signal = Removing;
		timer.restart();

		ClearAction();
		ShowDirection = 0;
		CurStep = 0;
		Elapsed = 0;
		Last = 0;

		removeNode(0);
		});

	SearchNode->onPress([=] {
		Signal = Searching;
		timer.restart();

		ClearAction();
		ShowDirection = 0;
		CurStep = 0;
		Elapsed = 0;
		Last = 0;

		Peek();
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
		Elapsed = 0;
		Last = 0;

		int Val = InsertVal->getText().toInt();

		insertNode(0, Val);
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

	Speed->onValueChange([=] {
		tgui::String s(0.5f + 0.25f * Speed->getValue());

		SpeedIndicator->setText(tgui::String("x") + s);
		Duration = (int)(700 / (0.5f + 0.25f * Speed->getValue()));
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
		ProgressThumb->setValue(ProgressThumb->getValue() + 1);
		});

	Backward->onPress([=] {
		ProgressThumb->setValue(ProgressThumb->getValue() - 1);
		});

	Theme1->onCheck([=] {
		MainColor = &VSPurple;
		theme.load("assets/themes/CyberPurple.txt");
		});

	Theme2->onCheck([=] {
		MainColor = &Fulvous;
		theme.load("assets/themes/ForestGreen.txt");
		});

	StructList->setSelectedItem(tgui::String("Stack"));

	StructList->onItemSelect([=] {
		tgui::String s = StructList->getSelectedItem();

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

void Stack::HandleEvent(Event& e)
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
						Last = 0;
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


void Stack::interactStack()
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

	switch (Signal) {
	case Pending:
		drawList(1);
		break;

	default:
		if (ShowMode == 0) {
			if (ShowDirection == 0 && Elapsed >= Duration) {

				if (CurStep + 1 == (int)action.size())
					Elapsed = Duration;
				else {
					timer.restart();
					Last = 0;
					Elapsed = 0;
					CurStep++;
				}
			}
			else if (ShowDirection == 1 && Elapsed <= 0) {

				if (CurStep == 0)
					Elapsed = 0;
				else {
					timer.restart();
					Last = Duration;
					Elapsed = Duration;
					CurStep--;
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