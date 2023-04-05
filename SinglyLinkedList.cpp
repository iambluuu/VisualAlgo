#include "SinglyLinkedList.h"
#include <math.h>

using namespace std;
using namespace sf;

int SinglyLinkedList::getSize()
{
	return NodeNumber;
}

void SinglyLinkedList::drawList(RenderWindow& app)
{
	for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
		tmp->drawArrow(app);
		tmp->drawNode(app, 255);
	}
}

void SinglyLinkedList::initList(RenderWindow& app) {
	int i = 0;

	for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
		tmp->NumberInList = i++;

		tmp->NodeState = Normal;
		tmp->ArrowState = Normal;
	}

	drawList(app);
}

void SinglyLinkedList::genList(RenderWindow& app, tgui::Gui& gui)
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
				tmp->drawArrow(app);
			}

			tmp->drawNode(app, 255);		
		}

		app.display();

		Elapsed = clock.getElapsedTime().asMilliseconds();
	}

}

void SinglyLinkedList::genList(RenderWindow& app, tgui::Gui& gui, const tgui::String s)
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
				tmp->drawArrow(app);
			}

			tmp->drawNode(app, 255);
		}

		app.display();

		Elapsed = clock.getElapsedTime().asMilliseconds();
	}

}

void SinglyLinkedList::changeState(RenderWindow& app, tgui::Gui& gui, Node*& Cur, Nodestate NextState)
{
	Node* tmp = new Node(Cur->Val);
	tmp->NodeState = NextState;
	tmp->changeNodePosition(Cur->Pos.x, Cur->Pos.y);

	int Elapsed = 0;
	int duration = 700;
	Clock clock;

	while (Elapsed <= duration) {
		app.clear();
		gui.draw();

		drawList(app);

		cerr << (int)(255 * (1 - (double)Elapsed / duration)) << "\n";

		tmp->drawNode(app, (int)(255 * (double)Elapsed / duration));
		Cur->drawNode(app, (int)(255 * (1 - (double)Elapsed / duration)));

		Elapsed = clock.getElapsedTime().asMilliseconds();

		app.display();
	}

	Cur->NodeState = NextState;
	Cur->drawNode(app, 255);

	delete tmp;
}

void SinglyLinkedList::drawArrowFlow(RenderWindow& app, tgui::Gui& gui, Node* Cur)
{
	if (!Cur->nxt)
		return;

	Cur->TmpArrow.setPosition(Cur->Arrow.getPosition());
	Cur->TmpArrow.setRotation(Cur->Arrow.getRotation());

	int ArrowLength = (int)(Util::DistanceBetweenNodes(Cur->Pos, Cur->nxt->Pos) - 40);
	int Elapsed = 0;
	Clock clock;

	while (Elapsed <= Duration) {
		app.clear();
		gui.draw();

		drawList(app);

		Cur->TmpArrow.setTextureRect(IntRect(100 - ArrowLength, 0, (int)(ArrowLength * (double)Elapsed / Duration), 10));
		app.draw(Cur->TmpArrow);

		Elapsed = clock.getElapsedTime().asMilliseconds();
		app.display();
	}

	Cur->ArrowState = Visited;
}

void SinglyLinkedList::NodeAppear(RenderWindow& app, tgui::Gui& gui, Node* Cur)
{
	int Elapsed = 0;
	Clock clock;

	while (Elapsed <= Duration) {

		app.clear(Color::White);
		gui.draw();

		Cur->drawNode(app, (int)(255 * (double)Elapsed / Duration));

		drawList(app);

		app.display();
		Elapsed = clock.getElapsedTime().asMilliseconds();
	}
}

void SinglyLinkedList::ConnectNode(RenderWindow& app, tgui::Gui& gui, Node* A, Node* B)
{
	A->updateArrow(B);

	int Elapsed = 0;
	Clock clock;

	while (Elapsed <= Duration) {
		app.clear(Color::White);
		gui.draw();

		int Length = (int)((Util::DistanceBetweenNodes(A->Pos, B->Pos) - 40) * Elapsed / Duration);
		A->Arrow.setTextureRect(IntRect(100 - Length, 0, Length, 10));
		A->drawArrow(app);

		drawList(app);

		app.display();
		Elapsed = clock.getElapsedTime().asMilliseconds();
	}
}

void SinglyLinkedList::InsertNode(RenderWindow& app, tgui::Gui& gui, Node* A)
{
	int Elapsed = 0;
	Clock clock;

	while (Elapsed <= Duration) {
		app.clear(Color::White);
		gui.draw();

		drawList(app);

		app.display();

		A->changeNodePosition(A->Pos.x, DefaultPosY + 100 * (1 - ((double)Elapsed / Duration)));
		
		if (A->nxt) {
			A->nxt->changeNodePosition(A->Pos.x + 100 * (double)Elapsed / Duration, DefaultPosY);

			for (Node* tmp = A->nxt->nxt; tmp; tmp = tmp->nxt)
				tmp->changeNodePosition(tmp->prev->Pos.x + 95, DefaultPosY);
		}
		

		Elapsed = clock.getElapsedTime().asMilliseconds();
	}
}

void SinglyLinkedList::insertAtBeginning(RenderWindow& app, tgui::Gui& gui, Node*& NewNode)
{
	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");

	PseudoCode->removeAllWidgets();
	PseudoCode->loadWidgetsFromFile("C:/Users/Admin/source/repos/iambluuu/VisualAlgo/assets/Description/SLLInsertAtBeginning.txt");

	tgui::TextArea::Ptr Line1 = PseudoCode->get<tgui::TextArea>("Line1");
	tgui::TextArea::Ptr Line2 = PseudoCode->get<tgui::TextArea>("Line2");
	tgui::TextArea::Ptr Line3 = PseudoCode->get<tgui::TextArea>("Line3");
	tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");

	TextHighlight->setVisible(1);

	TextHighlight->setSize(Line1->getSize());
	TextHighlight->setPosition(Line1->getPosition());

	if (Head) {
		NewNode->changeNodePosition(DefaultPosX, DefaultPosY + 100);
		NodeAppear(app, gui, NewNode);

		NewNode->nxt = Head;
		Head->prev = NewNode;

		Head->NumberInList++;
	} else {
		NewNode->changeNodePosition(DefaultPosX, DefaultPosY);

		NodeAppear(app, gui, NewNode);
	}
		
	Head = NewNode;

	TextHighlight->setSize(Line2->getSize());
	TextHighlight->setPosition(Line2->getPosition());
	if (NewNode->nxt)
		ConnectNode(app, gui, NewNode, NewNode->nxt);
	else
		Util::Wait();

	TextHighlight->setSize(Line3->getSize());
	TextHighlight->setPosition(Line3->getPosition());
	if (NewNode->nxt) {
		NewNode->NumberInList = 0;
		InsertNode(app, gui, NewNode);
	}

	
}

void SinglyLinkedList::insertAtEnd(RenderWindow& app, tgui::Gui& gui, Node* & NewNode)
{
	Node* Cur = Head;

	//Run To Node
	int Elapsed = 0;
	Clock clock;

	while (Cur && Cur->nxt) {
		Cur->NodeState = Selecting;
		
		drawArrowFlow(app, gui, Cur);

		Cur->NodeState = Visited;
		Cur = Cur->nxt;
	}
	
	Cur->NodeState = Selecting;
	
	Cur->ArrowState = New;
	NewNode->changeNodePosition(Cur->Pos.x + 95, DefaultPosY);
	
	//NewNode apppears
	NodeAppear(app, gui, NewNode);
	
	Cur->nxt = NewNode;
	NewNode->prev = Cur;

	//Connect NewNode
	NewNode->NumberInList = NodeNumber - 1;
	ConnectNode(app, gui, NewNode->prev, NewNode);
}

bool SinglyLinkedList::insertNode(RenderWindow& app, tgui::Gui& gui, int i, int v)
{
	if (i > NodeNumber || i < 0 || NodeNumber == maxNodeNumber)
		return 0;

	initList(app);

	Node* Cur = Head;
	Node* NewNode = new Node(v);

	NodeNumber++;

	NewNode->changeNodeValue(v);
	NewNode->NodeState = New;
	NewNode->ArrowState = New;

	if (i == 0) {
		insertAtBeginning(app, gui, NewNode);
		return 1;
	}

	if (i == NodeNumber - 1) {
		insertAtEnd(app, gui, NewNode);
		return 1;
	}

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");

	PseudoCode->removeAllWidgets();
	PseudoCode->loadWidgetsFromFile("C:/Users/Admin/source/repos/iambluuu/VisualAlgo/assets/Description/SLLInsert.txt");

	tgui::TextArea::Ptr Line1 = PseudoCode->get<tgui::TextArea>("Line1");

	tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");
	TextHighlight->setVisible(1);

	int Height = TextHighlight->getSize().y;

	//run to node

	Clock clock;
	int Elapsed = 0;

	for (int j = 0; j < i; j++) {
		if (j > 0) 
			TextHighlight->setPosition({0, Height * 1});

		PseudoCode->draw();
			
		Cur->NodeState = Selecting;

		drawList(app);

		if (j < i - 1) {
			TextHighlight->setPosition({ 0, Height * 2 });
			drawArrowFlow(app, gui, Cur);
			Cur->NodeState = Visited;
		}
		Cur = Cur->nxt;
	}
	
	TextHighlight->setPosition({ 0, Height * 3 });
	changeState(app, gui, Cur, Next);
	NewNode->changeNodePosition(Cur->Pos.x, Cur->Pos.y + 100);

	//NewNode appears

	TextHighlight->setPosition({ 0, Height * 4 });
	NodeAppear(app, gui, NewNode);

	//Connect Nodes	
	NewNode->nxt = Cur;

	NewNode->prev = Cur->prev;
	Cur->prev->nxt = NewNode;
	Cur->prev = NewNode;

	TextHighlight->setPosition({ 0, Height * 5 });
	ConnectNode(app, gui, NewNode, Cur);

	NewNode->prev->ArrowState = New;
	NewNode->NumberInList = i;
	NewNode->nxt->NumberInList++;

	TextHighlight->setPosition({ 0, Height * 6 });
	ConnectNode(app, gui, NewNode->prev, NewNode);
	
	//Move NewNode up
	InsertNode(app, gui, NewNode);

	return 1;
}

void SinglyLinkedList::removeAtBeginning(RenderWindow& app, tgui::Gui& gui) {
	int Elapsed = 0;
	Clock clock;

	Node* Cur = Head;
	Cur->ArrowState = Remove;

	Head = Cur->nxt;
	if (Head)
		Head->NumberInList = 0;

	while (Elapsed <= Duration) {
		app.clear(Color::White);
		gui.draw();

		drawList(app);
		Cur->drawNode(app, (int)(255 * (1 - (double)Elapsed / Duration)));
		Cur->drawArrow(app, (int)(255 * (1 - (double)Elapsed / Duration)));
		

		app.display();
		Elapsed = clock.getElapsedTime().asMilliseconds();
 	}

	Elapsed = 0;
	clock.restart();

	
	delete Cur;

	if (Head)
		Head->prev = nullptr;
	else
		return;

	while (Elapsed <= Duration) {
		app.clear(Color::White);
		gui.draw();

		Head->changeNodePosition(DefaultPosX + (int)(95 * (1 - (double)Elapsed / Duration)), DefaultPosY);
		
		
		for (Node* tmp = Head->nxt; tmp; tmp = tmp->nxt)
			tmp->changeNodePosition(tmp->prev->Pos.x + 95, DefaultPosY);

		drawList(app);

		app.display();
		Elapsed = clock.getElapsedTime().asMilliseconds();
	}

	return;
}

void SinglyLinkedList::removeAtEnd(RenderWindow& app, tgui::Gui& gui, Node*& Cur) {
	int Elapsed = 0;
	Clock clock;

	while (Elapsed <= Duration) {
		app.clear(Color::White);
		gui.draw();

		int Length = (int)((Util::DistanceBetweenNodes(Cur->prev->Pos, Cur->Pos) -40) * (1 - (double)Elapsed / Duration));
		Cur->prev->Arrow.setTextureRect(IntRect(100 - Length, 0, Length, 10));

		for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
			if (tmp->nxt)
				tmp->drawArrow(app);

			if (tmp == Cur) 
				tmp->drawNode(app, (int)(255 * (1 - (double)Elapsed / Duration)));
			else
				tmp->drawNode(app, 255);

		}

		app.display();
		Elapsed = clock.getElapsedTime().asMilliseconds();
	}


	Cur->prev->nxt = nullptr;

	delete Cur;

	return;
}

bool SinglyLinkedList::removeNode(RenderWindow& app, tgui::Gui& gui, int i)
{
	if (i >= NodeNumber || i < 0)
		return 0;

	initList(app);

	NodeNumber--;

	//Run to node
	Node* Cur = Head;

	for (int j = 0; j < i; j++) {
		Cur->NodeState = Selecting;
		drawList(app);

		if (j < i - 1) {
			drawArrowFlow(app, gui, Cur);
			Cur->NodeState = Visited;
		}
		Cur = Cur->nxt;
	}

	Node* Dell = Cur;
	Dell->NumberInList = -1;
	changeState(app, gui, Dell, Remove);

	if (i == 0) {
		removeAtBeginning(app, gui);
		return 1;
	}

	if (i == NodeNumber) {
		removeAtEnd(app, gui, Dell);
		return 1;
	}
	
	//Move Cur down
	int Elapsed = 0;
	Clock clock;

	while (Elapsed <= Duration) {
		app.clear(Color::White);
		gui.draw();

		Cur->changeNodePosition(Cur->Pos.x, DefaultPosY + (int)(100 * (double)Elapsed / Duration));

		drawList(app);

		Elapsed = clock.getElapsedTime().asMilliseconds();
		app.display();
	}

	//Update Next
	ConnectNode(app, gui, Cur->prev, Cur->nxt);
	Cur->prev->nxt = Cur->nxt;
	Cur->nxt->prev = Cur->prev;

	//Update Nodes position
	Elapsed = 0;
	clock.restart();

	Cur->ArrowState = Remove;

	while (Elapsed <= Duration) {
		app.clear(Color::White);
		gui.draw();

		Cur->drawArrow(app, (int)(255 * (1 - (double)Elapsed / Duration)));

		Cur->drawNode(app, (int)(255 * (1 - (double)Elapsed / Duration)));

		Cur->nxt->changeNodePosition(Cur->Pos.x + (int)(95 * (1 - (double)Elapsed / Duration)), DefaultPosY);
		
		for (Node* tmp = Cur->nxt->nxt; tmp; tmp = tmp->nxt) 
			tmp->changeNodePosition(tmp->prev->Pos.x + 95, DefaultPosY);

		Cur->changeNodePosition(Cur->Pos.x, Cur->Pos.y);

		drawList(app);

		Elapsed = clock.getElapsedTime().asMilliseconds();
		app.display();
	}

	//Delete
	delete Cur;

	return 1;
}


void SinglyLinkedList::initButtons(RenderWindow& app, tgui::Gui& gui)
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
		genList(app, gui);
		});

	UserInputEx->onPress([=, &app, &gui] {
		tgui::String s = UserInput->getText();

		genList(app, gui, s);
		});

	InsertEx->onPress([=, &app, &gui]  {
		int Pos = InsertPos->getText().toInt();
		int Val = InsertVal->getText().toInt();

		insertNode(app, gui, Pos, Val);
		});

	DeleteEx->onPress([=, &app, &gui] {
		int Pos = DeletePos->getText().toInt();

		removeNode(app, gui, Pos);
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


void SinglyLinkedList::interactSLL(RenderWindow& app, tgui::Gui& gui)
{
	Event e;

	while (app.pollEvent(e)) {
		if (e.type == Event::Closed) {
			app.close();
			State = EndProgram;
			return;
		}

		gui.handleEvent(e);
		
	}

	app.clear(Color::White);
	gui.draw();
	drawList(app);
	app.display();
}