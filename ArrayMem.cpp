#include "ArrayMem.h"

#include <iostream>

using namespace std;
using namespace sf;

void ArrayMem::changeMemPosition(float x, float y)
{
	Pos = { x, y };
	
	Box.setPosition(x, y);
	BoxBack.setPosition(x, y);

	Value.setOrigin(Value.getLocalBounds().left + Value.getLocalBounds().width / 2, Value.getLocalBounds().top + Value.getLocalBounds().height / 2);
	Value.setPosition(x + 23, y + 23);

	Index.setOrigin(Index.getLocalBounds().left + Index.getLocalBounds().width / 2, Index.getLocalBounds().top + Index.getLocalBounds().height / 2);
	Index.setPosition(x + 23, y - 23);
}

void ArrayMem::drawMem()
{
	Color Main = *MainColorArr;

	if (Selecting) {
		Box.setColor(Main);
		BoxBack.setFillColor(Color(255, 255, 255, 255 * 0.7));
		Value.setFillColor(Main);
		Index.setFillColor(NRed);
	}
	else {
		Box.setColor(Color::Black);
		BoxBack.setFillColor(Color(255, 255, 255, 255 * 0.7));
		Value.setFillColor(Color::Black);
		Index.setFillColor(NRed);
	}

	app.draw(BoxBack);
	app.draw(Box);
	app.draw(Value);
	app.draw(Index);
}

void ArrayMem::drawMem(int Opacity)
{
	Color Main = *MainColorArr;
	Color Red = NRed;

	Main.a = Opacity;
	Red.a = Opacity;

	if (Selecting) {
		Box.setColor(Main);
		Value.setFillColor(Main);
		Index.setFillColor(Red);
		BoxBack.setFillColor(Color(255, 255, 255, Opacity * 0.7));
	}
	else {
		Box.setColor(Color(0, 0, 0, Opacity));
		Value.setFillColor(Color(0, 0, 0, Opacity));
		Index.setFillColor(Red);
		BoxBack.setFillColor(Color(255, 255, 255, Opacity * 0.7));
	}

	app.draw(BoxBack);
	app.draw(Box);
	app.draw(Value);
	app.draw(Index);
}

void ArrayMem::changeMemValue(int v)
{
	Val = String(to_string(v));
	Value.setString(Val);

	Value.setOrigin(Value.getLocalBounds().left + Value.getLocalBounds().width / 2, Value.getLocalBounds().top + Value.getLocalBounds().height / 2);
	Value.setPosition(Pos.x + 23, Pos.y + 23);
}

void ArrayMem::changeMemValue(String v)
{
	Val = v;
	Value.setString(v);

	Value.setOrigin(Value.getLocalBounds().left + Value.getLocalBounds().width / 2, Value.getLocalBounds().top + Value.getLocalBounds().height / 2);
	Value.setPosition(Pos.x + 23, Pos.y + 23);
}