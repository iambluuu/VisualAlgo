#include "Node.h"
#include "Global.h"

#include <iostream>
#include <string>

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
		NodeShape.setOutlineColor(Color(0, 0, 0, Opacity));
		NodeShape.setFillColor(Color(255, 255, 255, Opacity));

		Value.setFillColor(Color(0, 0, 0, Opacity));
		break;

	case Visited:
		NodeShape.setOutlineColor(Color(193, 148, 243, Opacity));
		NodeShape.setFillColor(Color(255, 255, 255, Opacity));

		Value.setFillColor(Color(193, 148, 243, Opacity));
		break;

	case Selecting:
		NodeShape.setOutlineColor(Color(193, 148, 243, Opacity));
		NodeShape.setFillColor(Color(193, 148, 243, Opacity));

		Value.setFillColor(Color(255, 255, 255, Opacity));
		break;

	case New:
		NodeShape.setOutlineColor(Color(30, 215, 96, Opacity));
		NodeShape.setFillColor(Color(30, 215, 96, Opacity));

		Value.setFillColor(Color(255, 255, 255, Opacity));

	case Remove:
		NodeShape.setOutlineColor(Color(229, 9, 20, Opacity));
		NodeShape.setFillColor(Color(229, 9, 20, Opacity));

		Value.setFillColor(Color(255, 255, 255, Opacity));
		
	}

	app.draw(NodeShape);
	app.draw(Value);
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

	//change arrow position
	Arrow.setPosition(Pos.x + 40, Pos.y + 15);

	updateArrow(x, y);

	//update previous note
	if (prev)
		prev->updateArrow(prev->Pos.x, prev->Pos.y);

	//change value position
	switch (Val.length()) {
	case 1:
		Value.setPosition(Pos.x + 12, Pos.y + 3);
		Value.setCharacterSize(28);
		break;
	case 2:
		Value.setPosition(Pos.x + 7, Pos.y + 4);
		Value.setCharacterSize(25);
		break;
	case 3:
		Value.setPosition(Pos.x + 1, Pos.y + 4);
		Value.setCharacterSize(22);
		break;
	}
}
