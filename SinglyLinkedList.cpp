#include "SinglyLinkedList.h"
#include <math.h>

using namespace std;
using namespace sf;

void SinglyLinkedList::drawList(RenderWindow& app)
{
	for (Node* tmp = Head; tmp; tmp = tmp->nxt) {

		if (tmp->nxt)
			tmp->drawArrow(app);

		tmp->drawNode(app, 255);
	}
}

void SinglyLinkedList::initList(RenderWindow& app) {
	for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
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

	Head = new Node;
	Head->changeNodeValue(rand() % 100);
	Head->changeNodePosition(DefaultPosX, DefaultPosY);
	
	Node* Cur = Head;

	for (int i = 1; i < NodeNumber; i++) {
		Cur->nxt = new Node;
		Node* tmp = Cur;

		Cur = Cur->nxt;
		Cur->prev = tmp;
		
		Cur->changeNodeValue(rand() % 100);
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
	Head = new Node;
	Head->changeNodeValue(parts[0].toInt());
	Head->changeNodePosition(DefaultPosX, DefaultPosY);

	Node* Cur = Head;

	for (int i = 1; i < NodeNumber; i++) {
		Cur->nxt = new Node;
		Node* tmp = Cur;

		Cur = Cur->nxt;
		Cur->prev = tmp;

		Cur->changeNodeValue(parts[i].toInt());
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

		this->drawList(app);

		Cur->TmpArrow.setTextureRect(IntRect(100 - ArrowLength, 0, (int)(ArrowLength * (double)Elapsed / Duration), 10));
		app.draw(Cur->TmpArrow);

		Elapsed = clock.getElapsedTime().asMilliseconds();
		app.display();
	}

	Cur->ArrowState = Visited;
}

void SinglyLinkedList::insertAtEnd(RenderWindow& app, tgui::Gui& gui, Node* & NewNode)
{
	if (!Head) {
		Head = NewNode;
		return;
	}

	Node* Cur = Head;

	//Run To Node
	int Elapsed = 0;
	Clock clock;

	while (Cur && Cur->nxt) {
		Cur->NodeState = Selecting;
		
		drawList(app);
		drawArrowFlow(app, gui, Cur);

		Cur->NodeState = Visited;
		Cur = Cur->nxt;
	}
	
	Cur->NodeState = Selecting;
	drawList(app);
	
	Cur->ArrowState = New;
	NewNode->changeNodePosition(Cur->Pos.x + 95, DefaultPosY);
	
	//NewNode apppears
	Elapsed = 0;
	clock.restart();

	while (Elapsed <= Duration) {
		app.clear(Color::White);
		gui.draw();

		drawList(app);

		NewNode->drawNode(app, (int)(255 * ((double)Elapsed / Duration)));

		app.display();

		Elapsed = clock.getElapsedTime().asMilliseconds();
	}
	
	if (Cur)
		Cur->nxt = NewNode;
	NewNode->prev = Cur;

	//Connect NewNode
	Elapsed = 0;
	clock.restart();

	NewNode->changeNodePosition(NewNode->Pos.x, NewNode->Pos.y);

	while (Elapsed <= Duration) {

		app.clear(Color::White);
		gui.draw();

		if (NewNode->prev) {
			int Length = (int)((Util::DistanceBetweenNodes(NewNode->prev->Pos, NewNode->Pos) - 40) * Elapsed / Duration);
			NewNode->prev->Arrow.setTextureRect(IntRect(100 - Length, 0, Length, 10));
			NewNode->prev->drawArrow(app);
		}

		for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
			if (tmp->nxt && tmp != NewNode && tmp != NewNode->prev)
				tmp->drawArrow(app);

			tmp->drawNode(app, 255);			
		}

		app.display();

		Elapsed = clock.getElapsedTime().asMilliseconds();
	}

}

bool SinglyLinkedList::insertNode(RenderWindow& app, tgui::Gui& gui, int i, int v)
{
	if (i > NodeNumber || i < 0 || NodeNumber == maxNodeNumber)
		return 0;

	initList(app);

	Node* Cur = Head;

	Node* NewNode = new Node;

	NodeNumber++;

	NewNode->changeNodeValue(v);
	NewNode->NodeState = New;

	if (i == NodeNumber - 1) {
		insertAtEnd(app, gui, NewNode);
		return 1;
	}

	//run to node

	Clock clock;
	int Elapsed = 0;

	for (int j = 0; j < i; j++) {
		Cur->NodeState = Selecting;

		drawList(app);

		if (j < i - 1) {
			drawArrowFlow(app, gui, Cur);
			Cur->NodeState = Visited;
		}
		Cur = Cur->nxt;
	}
	
	NewNode->changeNodePosition(Cur->Pos.x, Cur->Pos.y + 100);

	//NewNode appears
	Elapsed = 0;
	clock.restart();

	while (Elapsed <= Duration) {

		app.clear(Color::White);
		gui.draw();

		NewNode->drawNode(app, (int)(255 * (double)Elapsed / Duration));

		drawList(app);

		app.display();
		Elapsed = clock.getElapsedTime().asMilliseconds();
	}

	//Connect Nodes	
	NewNode->nxt = Cur;
	NewNode->ArrowState = New;

	NewNode->prev = Cur->prev;

	if (Cur->prev) {
		Cur->prev->nxt = NewNode;
		Cur->prev->ArrowState = New;
	}
	else {
		Head = NewNode;
	}

	Cur->prev = NewNode;

	NewNode->changeNodePosition(NewNode->Pos.x, NewNode->Pos.y); //update arrows of the new added node and its previous node
	
	Elapsed = 0;
	clock.restart();

	while (Elapsed <= Duration) {
		app.clear(Color::White);
		gui.draw();

		if (NewNode->prev) {
			int Length1 = (int)((Util::DistanceBetweenNodes(NewNode->prev->Pos, NewNode->Pos) - 40) * Elapsed / Duration);
			NewNode->prev->Arrow.setTextureRect(IntRect(100 - Length1, 0, Length1, 10));
			NewNode->prev->drawArrow(app);
		}
		
		int Length2 = (int)((Util::DistanceBetweenNodes(NewNode->Pos, NewNode->nxt->Pos) - 40) * Elapsed / Duration);
		NewNode->Arrow.setTextureRect(IntRect(100 - Length2, 0, Length2, 10));
		NewNode->drawArrow(app);

		drawList(app);

		app.display();
		Elapsed = clock.getElapsedTime().asMilliseconds();
	}

	//Move NewNode up
	Elapsed = 0;
	clock.restart();

	while (Elapsed <= Duration) {
		app.clear(Color::White);
		gui.draw();

		drawList(app);

		app.display();

		NewNode->changeNodePosition(NewNode->Pos.x, DefaultPosY + 100 * (1 - ((double)Elapsed / Duration) ) );
		NewNode->nxt->changeNodePosition(NewNode->Pos.x + 100 * (double)Elapsed / Duration, DefaultPosY);

		for (Node* tmp = NewNode->nxt->nxt; tmp; tmp = tmp->nxt) 
			tmp->changeNodePosition(tmp->prev->Pos.x + 95, DefaultPosY);
		
		Elapsed = clock.getElapsedTime().asMilliseconds();
	}
	return 1;
}

void SinglyLinkedList::removeAtBeginning(RenderWindow& app, tgui::Gui& gui) {
	int Elapsed = 0;
	Clock clock;

	Node* Cur = Head;
	Cur->ArrowState = Remove;

	while (Elapsed <= Duration) {
		app.clear(Color::White);
		gui.draw();

		for (Node* tmp = Head; tmp; tmp = tmp->nxt) {
			if (tmp->nxt)
				tmp->drawArrow(app, (int)(255 * (1 - (double)Elapsed / Duration)));

			if (tmp == Cur)
				tmp->drawNode(app, (int)(255 * (1 - (double)Elapsed / Duration)));
			else
				tmp->drawNode(app, 255);
		
		}

		app.display();
		Elapsed = clock.getElapsedTime().asMilliseconds();
 	}

	Elapsed = 0;
	clock.restart();

	Head = Cur->nxt;
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
	Dell->NodeState = Remove;

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
	if (Cur->prev)
		Cur->prev->nxt = Cur->nxt;
	
	if (Cur->nxt)
		Cur->nxt->prev = Cur->prev;

	Cur->prev->changeNodePosition(Cur->prev->Pos.x, Cur->prev->Pos.y);

	Elapsed = 0;
	clock.restart();
	
	while (Elapsed <= Duration) {
		app.clear(Color::White);
		gui.draw();

		int Length = (int)((Util::DistanceBetweenNodes(Cur->prev->Pos, Cur->nxt->Pos) - 40) * Elapsed / Duration);

		Cur->prev->Arrow.setTextureRect(IntRect(100 - Length, 0, Length, 10));

		drawList(app);

		Cur->drawArrow(app);
		Cur->drawNode(app, 255);

		Elapsed = clock.getElapsedTime().asMilliseconds();
		app.display();
	}

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