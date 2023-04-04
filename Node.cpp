#include "Node.h"

#include <iostream>

using namespace std;
using namespace sf;

void Node::changeNodeValue(int val)
{
	Val = to_string(val);

	Value.setString(Val);
}

void Node::drawNode(RenderWindow& app, int Opacity)
{
	switch (NodeState) {
	case Normal:
		NodeShape.setColor(Color(0, 0, 0, Opacity));
		NodeBack.setFillColor(Color(255, 255, 255, Opacity));

		Value.setFillColor(Color(0, 0, 0, Opacity));

		if (NumberInList == 0)
			Title.setString(String("Head"));
		else
			Title.setString(String(""));

		break;

	case Visited:
		NodeShape.setColor(Color(193, 148, 243, Opacity));
		NodeBack.setFillColor(Color(255, 255, 255, Opacity));

		Value.setFillColor(Color(193, 148, 243, Opacity));

		if (NumberInList == 0)
			Title.setString(String("Head"));
		else 
			Title.setString(String(""));

		break;

	case Selecting:
		NodeShape.setColor(Color(193, 148, 243, Opacity));
		NodeBack.setFillColor(Color(193, 148, 243, Opacity));

		Value.setFillColor(Color(255, 255, 255, Opacity));

		if (NumberInList == 0)
			Title.setString(String("Cur/Head"));
		else
			Title.setString(String("Cur/") + String(to_string(NumberInList)));
		break;

	case New:
		NodeShape.setColor(Color(30, 215, 96, Opacity));
		NodeBack.setFillColor(Color(30, 215, 96, Opacity));

		Value.setFillColor(Color(255, 255, 255, Opacity));

		if (NumberInList == 0)
			Title.setString(String("New/Head"));
		else if (NumberInList > 0)
			Title.setString(String("New/") + String(to_string(NumberInList)));
		else
			Title.setString(String("New"));
		break;

	case Remove:
		NodeShape.setColor(Color(229, 9, 20, Opacity));
		NodeBack.setFillColor(Color(229, 9, 20, Opacity));

		Value.setFillColor(Color(255, 255, 255, Opacity));
		break;
		
	}

	app.draw(Title);
	app.draw(NodeBack);
	app.draw(NodeShape);
	app.draw(Value);
}

void Node::drawArrow(RenderWindow& app)
{
	if (!nxt)
		return;

	switch (ArrowState) {
	case Normal:
		Arrow.setColor(Color(0, 0, 0));
		break;
	case Visited:
		Arrow.setColor(Color(193, 148, 243));
		break;
	case New:
		Arrow.setColor(Color(30, 215, 96));
		break;
	case Remove:
		Arrow.setColor(Color(229, 9, 20));
		break;
	}

	app.draw(Arrow);
}

void Node::drawArrow(RenderWindow& app, int Opacity)
{
	switch (ArrowState) {
	case Normal:
		Arrow.setColor(Color(0, 0, 0, Opacity));
		break;
	case Visited:
		Arrow.setColor(Color(193, 148, 243, Opacity));
		break;
	case New:
		Arrow.setColor(Color(30, 215, 96, Opacity));
		break;
	case Remove:
		Arrow.setColor(Color(229, 9, 20, Opacity));
		break;
	}

	app.draw(Arrow);
}

void Node::updateArrow(float x, float y)
{
	if (!nxt)
		return;

	int ArrowLength = (int)(Util::DistanceBetweenNodes(Pos, nxt->Pos) - 40);
	float Angle = Util::AngleOfArrow(Pos, nxt->Pos);

	Arrow.setTextureRect(IntRect(100 - ArrowLength, 0, ArrowLength, 10));

	if (abs(Angle) > 0.000001f) {
		Arrow.setPosition(Pos.x + 20 + 20 * cos(Angle) + 5 * sin(Angle), Pos.y + 20 - 5 * cos(Angle) + 20 * sin(Angle));
	}

	Arrow.setRotation(Angle / PI * 180);
}

void Node::changeNodePosition(float x, float y)
{
	Pos.x = x;
	Pos.y = y;

	//change circle position
	NodeShape.setPosition(x, y);
	NodeBack.setPosition(x + 2, y + 2);

	//change arrow position
	Arrow.setPosition(Pos.x + 40, Pos.y + 15);

	updateArrow(x, y);

	//update previous note
	if (prev)
		prev->updateArrow(prev->Pos.x, prev->Pos.y);

	//change value position
	switch (Val.length()) {
	case 1:
		Value.setPosition(Pos.x + 13, Pos.y + 7);
		Value.setCharacterSize(28);
		break;
	case 2:
		Value.setPosition(Pos.x + 7, Pos.y + 7);
		Value.setCharacterSize(25);
		break;
	case 3:
		Value.setPosition(Pos.x + 1, Pos.y + 7);
		Value.setCharacterSize(22);
		break;
	}

	//change title position
	Title.setPosition(Vector2f(Pos.x + 5, Pos.y + 50));

}
