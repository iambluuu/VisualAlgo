#include "Array.h"

using namespace std;
using namespace sf;

void Array::reset()
{
	for (int i = 0; i < maxCap; i++) {
		Arr[i]->changeMemPosition(DefaultPosX + 95 * (i - 1), DefaultPosY);
		DummyArr[i]->changeMemValue(String(""));

		Arr[i]->Selecting = 0;
		DummyArr[i]->Selecting = 0;
	}
}

void Array::ClearAction()
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

void Array::initProgress() {
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

void Array::genList()
{
	reset();

	srand(time(0));

	//Gen new list

	size = rand() % maxCap + 1;
	capacity = size;

	for (int i = 0; i < size; i++) {
		int x = rand() % 100;
		Arr[i]->changeMemValue(String(to_string(x)));
	}

	for (int i = size; i < maxCap; i++) {
		Arr[i]->changeMemValue(String(""));
	}

	int Elapsed = 0;
	Clock clock;

	while (Elapsed <= Duration) {
		app.clear(Color::White);

		gui.draw();

		LabelAppear("arr", Arr[0], Elapsed);
		ArrAppear(Arr, 0, size, Elapsed);

		app.display();

		Elapsed = clock.getElapsedTime().asMilliseconds();
	}
}

void Array::genList(tgui::String s)
{
	vector<tgui::String> parts = tgui::String(s).split(',', true);

	if (parts.size() == 0)
		return;

	capacity = min((int)parts.size(), maxCap);
	size = capacity;

	for (int i = 0; i < parts.size(); i++) {
		int tmp;

		if (!parts[i].attemptToInt(tmp) || tmp >= 1000)
			return;
	}

	//Gen new list
	for (int i = 0; i < size; i++) {
		Arr[i]->changeMemValue(parts[i].toInt());
	}

	for (int i = size; i < maxCap; i++)
		Arr[i]->changeMemValue(String(""));


	int Elapsed = 0;
	Clock clock;

	while (Elapsed <= Duration) {
		app.clear(Color::White);

		gui.draw();

		LabelAppear("arr", Arr[0], Elapsed);
		ArrAppear(Arr, 0, size, Elapsed);

		app.display();

		Elapsed = clock.getElapsedTime().asMilliseconds();
	}
}

void Array::HighlightAppear(int Line, int Elapsed)
{
	if (Elapsed >= Duration / 2)
		Elapsed = Duration / 2;

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");

	TextHighlight->setPosition({ 0, 4 + 26 * Line });
	TextHighlight->setVisible(1);
	TextHighlight->setInheritedOpacity(2 * (float)Elapsed / Duration);
}

void Array::MoveHighlight(int CurLine, int NextLine, int Elapsed)
{
	if (Elapsed >= Duration / 2)
		Elapsed = Duration / 2;

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::Panel::Ptr TextHighlight = PseudoCode->get<tgui::Panel>("TextHighlight");

	int Distance = (int)(2 * (double)Elapsed / Duration * (NextLine - CurLine) * 26);

	TextHighlight->setPosition({ 0, 4 + CurLine * 26 + Distance });
}

void Array::drawLabel(String Name, ArrayMem* A, int Dummy)
{
	Vector2f Pos = A->Pos;

	ArrLabel.setOrigin(ArrLabel.getLocalBounds().left, ArrLabel.getLocalBounds().top + ArrLabel.getLocalBounds().height / 2);
	ArrLabel.setString(Name);
	ArrLabel.setPosition(Vector2f(Pos.x - 35, Pos.y + 23));
	ArrLabel.setFillColor(NRed);

	app.draw(ArrLabel);
}

void Array::LabelAppear(String Name, ArrayMem* A, int Elapsed)
{
	Vector2f Pos = A->Pos;
	Color Clor = NRed;
	Clor.a = (int)(255 * (double)Elapsed / Duration);

	ArrLabel.setOrigin(ArrLabel.getLocalBounds().left, ArrLabel.getLocalBounds().top + ArrLabel.getLocalBounds().height / 2);
	ArrLabel.setString(Name);
	ArrLabel.setPosition(Vector2f(Pos.x - 35, Pos.y + 23));
	ArrLabel.setFillColor(Clor);

	app.draw(ArrLabel);
}

void Array::LabelDisappear(String Name, ArrayMem* A, int Elapsed)
{
	LabelAppear(Name, A, Duration - Elapsed);
}

void Array::setArrNormal(vector<ArrayMem*>& A, int Dummy)
{
	for (int i = 0; i < maxCap; i++)
		A[i]->Selecting = 0;
}

void Array::ChangeValue(ArrayMem* Cur, sf::String preVal, sf::String nextVal, int Elapsed)
{
	if (!Cur)
		return;

	ArrayMem* tmp = new ArrayMem(0);
	tmp->changeMemValue(nextVal);
	tmp->Selecting = 1;
	tmp->Index.setString(Cur->Index.getString());
	tmp->changeMemPosition(Cur->Pos.x, Cur->Pos.y);

	tmp->drawMem((int)(255 * (double)Elapsed / Duration));
	Cur->drawMem((int)(255 * (1 - (double)Elapsed / Duration)));

	if ((double)Elapsed / Duration >= 0.8) {
		Cur->changeMemValue(nextVal);
	}
	else {
		Cur->changeMemValue(preVal);
	}

	delete tmp;
}

void Array::ChangeState(ArrayMem* Cur, bool CurState, bool NextState, int Elapsed)
{
	if (!Cur)
		return;

	ArrayMem* tmp = new ArrayMem(0);
	tmp->changeMemValue(Cur->Val);
	tmp->Index.setString(Cur->Index.getString());
	tmp->Selecting = NextState;
	tmp->changeMemPosition(Cur->Pos.x, Cur->Pos.y);

	Cur->Selecting = CurState;

	tmp->drawMem((int)(255 * (double)Elapsed / Duration));
	Cur->drawMem((int)(255 * (1 - (double)Elapsed / Duration)));

	if ((double)Elapsed / Duration >= 0.8) {
		Cur->Selecting = NextState;
	}
	else {
		Cur->Selecting = CurState;
	}

	delete tmp;
}

void Array::ArrAppear(vector<ArrayMem*>& A, int l, int r, int Elapsed)
{
	for (int i = l; i < r; i++)
		A[i]->drawMem((int)(255 * (double)Elapsed / Duration));
}

void Array::ArrDisappear(vector<ArrayMem*>& A, int l, int r, int Elapsed)
{
	ArrAppear(A, l, r, Duration - Elapsed);
}

void Array::drawArr(vector<ArrayMem*>& A, int l, int r, int Dummy)
{
	if (r < 0 || l < 0 || l >= maxCap || r > maxCap)
		return;

	for (int i = l; i < r; i++)
		A[i]->drawMem();
}

void Array::MoveArrDown(int Elapsed)
{
	int NewY = DefaultPosY + (int)(100 * (double)Elapsed / Duration);

	for (int i = 0; i < maxCap; i++)
		Arr[i]->changeMemPosition(DefaultPosX + 95 * (i - 1), NewY);
}

void Array::pushBack(int v)
{
	if (size == capacity) {
		Signal = Pending;
		return;
	}

	reset();

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("arr[size++] = v"));

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&Array::HighlightAppear, this, 0, placeholders::_1));
	
	action.back().push_back(bind(&Array::drawLabel, this, String("arr"), Arr[0], placeholders::_1));
	action.back().push_back(bind(&Array::drawArr, this, Arr, 0, size, placeholders::_1));
	action.back().push_back(bind(&Array::drawArr, this, Arr, size + 1, capacity, placeholders::_1));
	action.back().push_back(bind(&Array::ChangeValue, this, Arr[size], String(""), String(to_string(v)), placeholders::_1));

	size++;

	initProgress();
}

void Array::insertAt(int i, int v)
{
	if (i < 0 || i > size || size == capacity) {
		Signal = Pending;
		return;
	}

	if (i == size) {
		pushBack(v);
		return;
	}

	reset();

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("for(k = size - 1; k >= i; k++)\n		arr[k + 1] = arr[k]\narr[i] = v, size++"));


	for (int j = size - 1; j >= i; j--) {
		String tmp = Arr[j]->Val;
		String tmp2 = Arr[j + 1]->Val;

		action.push_back(vector<function<void(int)> >());

		action.back().push_back(bind(&Array::drawLabel, this, String("arr"), Arr[0], placeholders::_1));
		action.back().push_back(bind(&Array::setArrNormal, this, Arr, placeholders::_1));
		action.back().push_back(bind(&Array::drawArr, this, Arr, 0, j, placeholders::_1));

		if (j < size - 1) {
			action.back().push_back(bind(&Array::MoveHighlight, this, 1, 0, placeholders::_1));
			action.back().push_back(bind(&Array::ChangeState, this, Arr[j + 2], 1, 0, placeholders::_1));
			action.back().push_back(bind(&Array::ChangeState, this, Arr[j + 1], 1, 0, placeholders::_1));
			action.back().push_back(bind(&Array::drawArr, this, Arr, j + 3, capacity, placeholders::_1));
		}
		else {
			action.back().push_back(bind(&Array::HighlightAppear, this, 0, placeholders::_1));
			action.back().push_back(bind(&Array::drawArr, this, Arr, j + 1, capacity, placeholders::_1));
		}

		action.back().push_back(bind(&Array::ChangeState, this, Arr[j], 0, 1, placeholders::_1));

		action.push_back(vector<function<void(int)> >());
		action.back().push_back(bind(&Array::drawLabel, this, String("arr"), Arr[0], placeholders::_1));
		action.back().push_back(bind(&Array::MoveHighlight, this, 0, 1, placeholders::_1));
		action.back().push_back(bind(&Array::drawArr, this, Arr, 0, j + 1, placeholders::_1));
		action.back().push_back(bind(&Array::drawArr, this, Arr, j + 2, capacity, placeholders::_1));
		action.back().push_back(bind(&Array::ChangeValue, this, Arr[j + 1], tmp2, tmp, placeholders::_1));

	}

	String tmp = Arr[i]->Val;

	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&Array::MoveHighlight, this, 1, 2, placeholders::_1));
	action.back().push_back(bind(&Array::drawLabel, this, String("arr"), Arr[0], placeholders::_1));
	action.back().push_back(bind(&Array::drawArr, this, Arr, 0, i, placeholders::_1));
	action.back().push_back(bind(&Array::drawArr, this, Arr, i + 1, capacity, placeholders::_1));
	action.back().push_back(bind(&Array::ChangeValue, this, Arr[i], tmp, String(to_string(v)), placeholders::_1));

	size++;

	initProgress();
}

void Array::popBack()
{
	if (size == 0) {
		Signal = Pending;
		return;
	}

	reset();

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("arr[size--] = 0"));

	String tmp = Arr[size - 1]->Val;
	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&Array::HighlightAppear, this, 0, placeholders::_1));
	action.back().push_back(bind(&Array::drawLabel, this, String("arr"), Arr[0], placeholders::_1));
	action.back().push_back(bind(&Array::drawArr, this, Arr, 0, size - 1, placeholders::_1));
	action.back().push_back(bind(&Array::drawArr, this, Arr, size, capacity, placeholders::_1));
	action.back().push_back(bind(&Array::ChangeValue, this, Arr[size - 1], tmp, String(""), placeholders::_1));

	size--;

	initProgress();
}

void Array::deleteAt(int i)
{
	if (size == 0 || i >= size) {
		Signal = Pending;
		return;
	}

	if (i == size - 1) {
		popBack();
		return;
	}

	reset();

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("for(k = i; k < size - 1; k++)\n		arr[k] = arr[k + 1]\narr[size--] = 0"));

	for (int j = i; j < size - 1; j++) {
		String tmp = Arr[j]->Val;
		String tmp2 = Arr[j + 1]->Val;

		action.push_back(vector<function<void(int)> >());

		if (j > i) {
			action.back().push_back(bind(&Array::MoveHighlight, this, 1, 0, placeholders::_1));
		}
		else
			action.back().push_back(bind(&Array::HighlightAppear, this, 0, placeholders::_1));

		action.back().push_back(bind(&Array::drawLabel, this, String("arr"), Arr[0], placeholders::_1));
		action.back().push_back(bind(&Array::setArrNormal, this, Arr, placeholders::_1));
		if (j > i) {
			action.back().push_back(bind(&Array::ChangeState, this, Arr[j - 1], 1, 0, placeholders::_1));
			action.back().push_back(bind(&Array::drawArr, this, Arr, 0, j - 1, placeholders::_1));
		}
		else
			action.back().push_back(bind(&Array::drawArr, this, Arr, 0, j, placeholders::_1));

		action.back().push_back(bind(&Array::drawArr, this, Arr, j + 1, capacity, placeholders::_1));
		action.back().push_back(bind(&Array::ChangeState, this, Arr[j], 0, 1, placeholders::_1));

		action.push_back(vector<function<void(int)> >());
		action.back().push_back(bind(&Array::drawLabel, this, String("arr"), Arr[0], placeholders::_1));
		action.back().push_back(bind(&Array::MoveHighlight, this, 0, 1, placeholders::_1));
		action.back().push_back(bind(&Array::drawArr, this, Arr, 0, j, placeholders::_1));
		action.back().push_back(bind(&Array::drawArr, this, Arr, j + 1, capacity, placeholders::_1));
		action.back().push_back(bind(&Array::ChangeValue, this, Arr[j], tmp, tmp2, placeholders::_1));

	}

	String tmp = Arr[size - 1]->Val;

	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&Array::MoveHighlight, this, 1, 2, placeholders::_1));
	action.back().push_back(bind(&Array::drawLabel, this, String("arr"), Arr[0], placeholders::_1));
	action.back().push_back(bind(&Array::setArrNormal, this, Arr, placeholders::_1));
	action.back().push_back(bind(&Array::drawArr, this, Arr, 0, size - 2, placeholders::_1));
	action.back().push_back(bind(&Array::drawArr, this, Arr, size, capacity, placeholders::_1));
	if (size >= 2) {
		action.back().push_back(bind(&Array::ChangeState, this, Arr[size - 2], 1, 0, placeholders::_1));
	}
	action.back().push_back(bind(&Array::ChangeValue, this, Arr[size - 1], tmp, String(""), placeholders::_1));

	size--;

	initProgress();
}

void Array::accessMem(int i)
{
	if (i >= size || i < 0) {
		Signal = Pending;
		return;
	}

	reset();

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("return arr[i]"));

	action.push_back(vector<function<void(int)> >());

	action.back().push_back(bind(&Array::HighlightAppear, this, 0, placeholders::_1));
	action.back().push_back(bind(&Array::drawLabel, this, String("arr"), Arr[0], placeholders::_1));
	action.back().push_back(bind(&Array::drawArr, this, Arr, 0, i, placeholders::_1));
	action.back().push_back(bind(&Array::drawArr, this, Arr, i + 1, capacity, placeholders::_1));
	action.back().push_back(bind(&Array::ChangeState, this, Arr[i], 0, 1, placeholders::_1));

	initProgress();
}

void Array::searchMem(int v)
{
	if (size == 0) {
		Signal = Pending;
		return;
	}

	reset();

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("for(k = 0; k < size; k++)\n		if(arr[k] == v)\n			return k\nreturn -1"));

	int j = 0;

	while (j < size) {
		action.push_back(vector<function<void(int)> >());

		if (j == 0)
			action.back().push_back(bind(&Array::HighlightAppear, this, 0, placeholders::_1));
		else
			action.back().push_back(bind(&Array::MoveHighlight, this, 1, 0, placeholders::_1));
		action.back().push_back(bind(&Array::drawLabel, this, String("arr"), Arr[0], placeholders::_1));
		action.back().push_back(bind(&Array::setArrNormal, this, Arr, placeholders::_1));
		if (j > 0) {
			action.back().push_back(bind(&Array::drawArr, this, Arr, 0, j - 1, placeholders::_1));
			action.back().push_back(bind(&Array::ChangeState, this, Arr[j - 1], 1, 0, placeholders::_1));
		}
		action.back().push_back(bind(&Array::drawArr, this, Arr, j + 1, capacity, placeholders::_1));
		action.back().push_back(bind(&Array::ChangeState, this, Arr[j], 0, 1, placeholders::_1));

		action.push_back(vector<function<void(int)> >());
		action.back().push_back(bind(&Array::drawArr, this, Arr, 0, capacity, placeholders::_1));
		action.back().push_back(bind(&Array::drawLabel, this, String("arr"), Arr[0], placeholders::_1));
		action.back().push_back(bind(&Array::MoveHighlight, this, 0, 1, placeholders::_1));

		if (Arr[j]->Val == String(to_string(v))) {
			action.back().push_back(bind(&Array::drawArr, this, Arr, 0, capacity, placeholders::_1));
			action.back().push_back(bind(&Array::drawLabel, this, String("arr"), Arr[0], placeholders::_1));
			action.back().push_back(bind(&Array::MoveHighlight, this, 0, 2, placeholders::_1));
			break;
		}

		j++;
	}

	if (j == size) {
		action.push_back(vector<function<void(int)> >());
		action.back().push_back(bind(&Array::MoveHighlight, this, 1, 3, placeholders::_1));
		action.back().push_back(bind(&Array::drawLabel, this, String("arr"), Arr[0], placeholders::_1));
		action.back().push_back(bind(&Array::setArrNormal, this, Arr, placeholders::_1));
		action.back().push_back(bind(&Array::drawArr, this, Arr, 0, size - 1, placeholders::_1));
		action.back().push_back(bind(&Array::drawArr, this, Arr, size, capacity, placeholders::_1));
		action.back().push_back(bind(&Array::ChangeState, this, Arr[size - 1], 1, 0, placeholders::_1));
	}

	initProgress();
}


void Array::updateMem(int i, int v)
{
	if (i < 0 || i >= size) {
		Signal = Pending;
		return;
	}

	reset();

	tgui::ChildWindow::Ptr PseudoCode = gui.get<tgui::ChildWindow>("PseudoCode");
	tgui::TextArea::Ptr TextArea = PseudoCode->get<tgui::TextArea>("TextArea1");

	TextArea->setText(tgui::String("arr[i] = v"));

	String preVal = Arr[i]->Val;
	String nexVal(to_string(v));

	action.push_back(vector<function<void(int)> >());
	action.back().push_back(bind(&Array::HighlightAppear, this, 0, placeholders::_1));
	action.back().push_back(bind(&Array::drawLabel, this, String("arr"), Arr[0], placeholders::_1));
	action.back().push_back(bind(&Array::drawArr, this, Arr, 0, i, placeholders::_1));
	action.back().push_back(bind(&Array::drawArr, this, Arr, i + 1, capacity, placeholders::_1));
	action.back().push_back(bind(&Array::ChangeValue, this, Arr[i], preVal, nexVal, placeholders::_1));

	initProgress();
}

void Array::initButtons()
{
	tgui::Picture::Ptr Background = gui.get<tgui::Picture>("Background");
	Background->setRenderer(theme.getRenderer("Background"));

	tgui::ComboBox::Ptr StructList = gui.get<tgui::ComboBox>("ComboBox1");
	StructList->setRenderer(theme.getRenderer("ComboBox"));

	tgui::Button::Ptr ShrinkButton = gui.get<tgui::Button>("ShrinkButton");
	ShrinkButton->setRenderer(theme.getRenderer("Button"));
	tgui::Button::Ptr GrowButton = gui.get<tgui::Button>("GrowButton");
	GrowButton->setRenderer(theme.getRenderer("Button"));

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
	tgui::EditBox::Ptr ArraySize = gui.get<tgui::EditBox>("ArraySize");
	ArraySize->setRenderer(theme.getRenderer("EditBox"));

	tgui::Button::Ptr AccessButton = gui.get<tgui::Button>("AccessButton");
	AccessButton->setRenderer(theme.getRenderer("Button"));
	tgui::Button::Ptr AccessEx = gui.get<tgui::Button>("AccessEx");
	AccessEx->setRenderer(theme.getRenderer("ExButton"));
	tgui::EditBox::Ptr AccessPos = gui.get<tgui::EditBox>("AccessPos");
	AccessPos->setRenderer(theme.getRenderer("EditBox"));

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
		MainColorArr = &DeepBlue;
		Theme1->setChecked(1);
	}
	else {
		MainColorArr = &Fulvous;
		Theme2->setChecked(1);
	}

	Speed->setValue(2);
	ShrinkButton->setVisible(0);
	GrowButton->setVisible(0);

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
			InsertMode->setText(tgui::String("Insert At"));
			InsertPos->setVisible(1);
			InsertPos->setText(tgui::String(""));
			InsertPos->setPosition({ "InsertMode.right + 10", "InsertMode.top" });
			InsertVal->setPosition({ "InsertPos.right + 10", "InsertMode.top" });
			InsertEx->setPosition({ "InsertVal.right + 10", "InsertMode.top" });
			break;

		case 1:
			InsertMode->setText(tgui::String("Push Front"));
			InsertPos->setVisible(0);
			InsertVal->setPosition({ "InsertMode.right + 10", "InsertMode.top" });
			InsertEx->setPosition({ "InsertVal.right + 10", "InsertMode.top" });
			break;

		case 2:
			InsertMode->setText(tgui::String("Push Back"));
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

		});

	DeleteMode->onPress([=] {
		DeleteModes = (DeleteModes + 1) % 3;

		switch (DeleteModes) {
		case 0:
			DeleteMode->setText(tgui::String("Remove At"));
			DeletePos->setVisible(1);
			DeletePos->setText(tgui::String(""));
			DeletePos->setPosition({ "DeleteMode.right + 10", "DeleteMode.top" });
			DeleteEx->setPosition({ "DeletePos.right + 10", "DeleteMode.top" });
			break;

		case 1:
			DeleteMode->setText(tgui::String("Pop Front"));
			DeletePos->setVisible(0);
			DeleteEx->setPosition({ "DeleteMode.right + 10", "DeleteMode.top" });
			break;

		case 2:
			DeleteMode->setText(tgui::String("Pop Back"));
			DeletePos->setVisible(0);
			DeleteEx->setPosition({ "DeleteMode.right + 10", "DeleteMode.top" });
			break;
		}
	});

	InputGen->onPress([=] {
		GenModes = (GenModes + 1) % 4;

		switch (GenModes) {
		case 0:
			InputGen->setText(tgui::String("Manual Input"));

			ArraySize->setVisible(0);
			UserInput->setVisible(1);
			UserInputEx->setPosition({ "EditBox1.right + 10", "EditBox1.top" });
			break;

		case 1:
			InputGen->setText(tgui::String("Random Input"));

			ArraySize->setVisible(0);
			UserInput->setVisible(0);
			UserInputEx->setPosition({ "InputGen.right + 10", "InputGen.top + 5" });
			break;

		case 2:
			InputGen->setText(tgui::String("Browse"));

			ArraySize->setVisible(0);
			UserInput->setVisible(0);
			UserInputEx->setPosition({ "InputGen.right + 10", "InputGen.top + 5" });
			break;

		case 3:
			InputGen->setText(tgui::String("Empty"));

			ArraySize->setVisible(1);
			UserInput->setVisible(0);
			ArraySize->setPosition({ "InputGen.right + 10", "InputGen.top + 5" });
			UserInputEx->setPosition({ "ArraySize.right + 10", "ArraySize.top" });
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
		reset();
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

		case 3:
		{
			for (int i = 0; i < maxCap; i++) {
				Arr[i]->changeMemValue(String(""));
			}

			int tmp;
			tgui::String s = ArraySize->getText();
			if (!s.attemptToInt(tmp))
				break;

			if (tmp <= maxCap && tmp > 0)
				capacity = tmp;
			break;
		}
		}
		});

	InsertEx->onPress([=] {
		/*if (size == maxNodeNumber)
			return;*/

		int Pos = InsertPos->getText().toInt();
		int Val = InsertVal->getText().toInt();

		if (InsertModes == 1)
			Pos = 0;
		else if (InsertModes == 2)
			Pos = size;

		if (size == capacity || Pos < 0 || Pos > size) {
			return;
		}

		Signal = Inserting;
		timer.restart();

		ClearAction();
		ShowDirection = 0;
		CurStep = 0;
		Elapsed = 0;
		Last = 0;

		insertAt(Pos, Val);
		});

	DeleteEx->onPress([=] {
		//if (NodeNumber == 0)
		//	return;

		int Pos = DeletePos->getText().toInt();

		if (DeleteModes == 1)
			Pos = 0;
		else if (DeleteModes == 2)
			Pos = size - 1;


		if (size == 0 || Pos < 0 || Pos >= size) {
			return;
		}

		Signal = Removing;
		timer.restart();

		ClearAction();
		ShowDirection = 0;
		CurStep = 0;
		Last = 0;
		Elapsed = 0;

		if (DeleteModes == 0) {
			deleteAt(Pos);
		}
		else if (DeleteModes == 1)
			popBack();

		});

	AccessEx->onPress([=] {
		tgui::String Pos = AccessPos->getText();

		int i = Pos.toInt();
		if (i < 0 || i >= size)
			return;

		Signal = Searching;
		timer.restart();

		ClearAction();
		ShowDirection = 0;
		CurStep = 0;
		Last = 0;
		Elapsed = 0;

		accessMem(i);
		});


	SearchEx->onPress([=] {
		if (size == 0)
			return;

		Signal = Searching;
		timer.restart();

		ClearAction();
		ShowDirection = 0;
		CurStep = 0;
		Last = 0;
		Elapsed = 0;

		tgui::String Val = SearchVal->getText();

		searchMem(Val.toInt());
		});

	UpdateEx->onPress([=] {
		tgui::String Val = UpdateVal->getText();
		tgui::String Pos = UpdatePos->getText();

		int i = Pos.toInt();
		if (i < 0 || i >= size)
			return;

		Signal = Searching;
		timer.restart();

		ClearAction();
		ShowDirection = 0;
		CurStep = 0;
		Last = 0;
		Elapsed = 0;

		updateMem(Pos.toInt(), Val.toInt());
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
		MainColorArr = &DeepBlue;
	ThemeNum = 0;
	theme.load("assets/themes/CyberPurple.txt");
		});

	Theme2->onCheck([=] {
		MainColorArr = &Fulvous;
	ThemeNum = 1;
	theme.load("assets/themes/ForestGreen.txt");
		});

	StructList->setSelectedItem(tgui::String("Array"));

	StructList->onItemSelect([=] {
		tgui::String s = StructList->getSelectedItem();

	if (s != tgui::String("Array")) {
		ClearAction();

		Array::size = 0;
		capacity = 0;

		for (int i = 0; i < 12; i++) {
			Arr[i]->changeMemValue(String(""));
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

	if (s == tgui::String("Dynamic Array"))
		State = _DArray;

	if (s == tgui::String("SLL"))
		State = _SLL;
		});
}


void Array::HandleEvent(Event& e)
{
	switch (Signal) {
	case Searching:
	case Removing:
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
					if (CurStep + 1 == (int)action.size() || (double)Elapsed / Duration < 0.8) {
						Last = Duration;
						Elapsed = Duration;
					}
					else {
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
					if (CurStep == 0 || (double)Elapsed / Duration > 0.2) {
						Last = 0;
						Elapsed = 0;
					}
					else {
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

void Array::interactArr()
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
		if (capacity > 0)
			drawLabel(String("arr"), Arr[0], 1);
		drawArr(Arr, 0, capacity, 1);
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
		}


		for (int i = 0; i < (int)action[CurStep].size(); i++) {
			action[CurStep][i](Elapsed);
		}

		break;

	}

	app.display();
}