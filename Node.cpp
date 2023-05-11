#include "Node.h"

#include <iostream>

using namespace std;
using namespace sf;

void Node::changeNodeValue(int val)
{
	Val = to_string(val);

	Value.setString(Val);
}

void Node::changeNodeValue(String val)
{
	Val = val;
	
	Value.setString(Val);
}

void Node::drawNode(int Opacity)
{
	Color Main = *MainColor;
	Main.a = Opacity;
	SGreen.a = Opacity;
	NRed.a = Opacity;
	FBlue.a = Opacity;

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
		NodeShape.setColor(Main);
		NodeBack.setFillColor(Color(255, 255, 255, Opacity));
		Value.setFillColor(Main);

		if (NumberInList == 0)
			Title.setString(String("Head"));
		else 
			Title.setString(String(""));

		break;

	case Selecting:
		NodeShape.setColor(Main);
		NodeBack.setFillColor(Main);
		Value.setFillColor(Color(255, 255, 255, Opacity));

		if (NumberInList == 0)
			Title.setString(String("Cur/Head"));
		else
			Title.setString(String("Cur/") + String(to_string(NumberInList)));
		break;

	case New:
		NodeShape.setColor(SGreen);
		NodeBack.setFillColor(SGreen);
		Value.setFillColor(Color(255, 255, 255, Opacity));

		if (NumberInList == 0)
			Title.setString(String("New/Head"));
		else if (NumberInList > 0)
			Title.setString(String("New/") + String(to_string(NumberInList)));
		else
			Title.setString(String("New"));
		break;

	case Remove:
		NodeShape.setColor(NRed);
		NodeBack.setFillColor(NRed);
		Value.setFillColor(Color(255, 255, 255, Opacity));

		break;
		
	case Next:
		NodeShape.setColor(FBlue);
		NodeBack.setFillColor(FBlue);
		Value.setFillColor(Color(255, 255, 255, Opacity));
	
		if (NumberInList == 0)
			Title.setString(String("Head"));
		else
			Title.setString(String("Aft"));

		break;
	}

	//app.draw(Title);
	app.draw(NodeBack);
	app.draw(NodeShape);
	app.draw(Value);
}

void Node::drawArrow()
{
	if (!nxt)
		return;

	switch (ArrowState) {
	case Normal:
		Arrow.setColor(Color(0, 0, 0));
		break;
	case Visited:
		Arrow.setColor(*MainColor);
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

void Node::drawArrow(int Opacity)
{
	Color Main = *MainColor;
	Main.a = Opacity;
	switch (ArrowState) {
	case Normal:
		Arrow.setColor(Color(0, 0, 0, Opacity));
		break;
	case Visited:
		Arrow.setColor(Main);
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

void Node::drawDArrow()
{
	if (!nxt)
		return;

	Color Main = *MainColor;
	switch (ArrowState) {
	case Normal:
		Arrow.setColor(Color(0, 0, 0));
		break;
	case Visited:
		Arrow.setColor(Main);
		break;
	case New:
		Arrow.setColor(Color(30, 215, 96));
		break;
	case Remove:
		Arrow.setColor(Color(229, 9, 20));
		break;
	}

	double Distance = Arrow.getLocalBounds().width;

	Vector2f OriginPos = Arrow.getPosition();
	float Angle = Arrow.getRotation() * PI / 180;

	Arrow.setPosition(OriginPos.x + 7 * sin(Angle) - 5 * cos(Angle), OriginPos.y - 7 * cos(Angle) - 5 * sin(Angle));
	app.draw(Arrow);

	switch (ArrowStateU) {
	case Normal:
		Arrow.setColor(Color(0, 0, 0));
		break;
	case Visited:
		Arrow.setColor(Main);
		break;
	case New:
		Arrow.setColor(Color(30, 215, 96));
		break;
	case Remove:
		Arrow.setColor(Color(229, 9, 20));
		break;
	}

	Arrow.setRotation(Angle * 180 / PI + 180);
	Arrow.setOrigin(Arrow.getLocalBounds().left, Arrow.getLocalBounds().top + Arrow.getLocalBounds().height);
	Arrow.setPosition(OriginPos.x - 7 * sin(Angle) + (Distance - 5) * cos(Angle), OriginPos.y + 7 * cos(Angle) - 5 * sin(Angle) + Distance * sin(Angle));
	app.draw(Arrow);

	Arrow.setOrigin(Arrow.getLocalBounds().left, Arrow.getLocalBounds().top);
	Arrow.setPosition(OriginPos);
	Arrow.setRotation(Angle * 180 / PI);
}

void Node::updateArrow(Node* Nxt)
{
	if (!Nxt)
		return;

	Arrow.setPosition(Pos.x + 46, Pos.y + 17);

	int ArrowLength = (int)(Util::DistanceBetweenNodes(Pos, Nxt->Pos) - 46);

	if (NodeType == 1) {
		ArrowLength += 10;
	}
	
	//if (NodeState == New || Nxt->NodeState == New) {
	//	ArrowState = New;
	//	ArrowStateU = New;
	//}

	//if (NodeState == Remove || Nxt->NodeState == Remove) {
	//	ArrowState = Remove;
	//	ArrowStateU = Remove;
	//}
	
	float Angle = Util::AngleOfArrow(Pos, Nxt->Pos);

	Arrow.setTextureRect(IntRect(100 - ArrowLength, 0, ArrowLength, 10));

	if (abs(Angle) > 0.000001f) {
		Arrow.setPosition(Pos.x + 23 + 23 * cos(Angle) + 5 * sin(Angle), Pos.y + 23 - 5 * cos(Angle) + 23 * sin(Angle));
	}

	Arrow.setRotation(Angle / PI * 180);
}

void Node::changeNodePosition(float x, float y)
{
	Pos.x = x;
	Pos.y = y;

	//change hitbox position
	Hitbox->setPosition({ x , y });

	//change circle position
	NodeShape.setPosition(x, y);
	NodeBack.setPosition(x + 2, y + 2);

	//change arrow position
	updateArrow(nxt);

	//update previous note
	if (prev)
		prev->updateArrow(prev->nxt);

	//change value position
	Value.setOrigin(Value.getLocalBounds().left + Value.getLocalBounds().width / 2, Value.getLocalBounds().top + Value.getLocalBounds().height / 2);
	Value.setPosition(Pos.x + 23, Pos.y + 23);

	//change title position
	Title.setPosition(Vector2f(Pos.x + 5, Pos.y + 50));

}
