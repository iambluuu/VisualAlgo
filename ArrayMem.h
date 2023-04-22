#pragma once

#include "SFML/Graphics.hpp"
#include "Global.h"
#include <string>

#define FontOfNum "assets/CONSOLAB.TTF"
#define FontOfTitle "assets/SpaceGrotesk-Bold.ttf"

using namespace std;
using namespace sf;

struct ArrayMem {
	tgui::Button::Ptr Hitbox;

	Vector2f Pos;

	Texture Square;
	Sprite Box;
	RectangleShape BoxBack;

	Font font;
	Text Value;
	Text Index;
	
	String Val;

	bool Selecting;

	ArrayMem(int i) {
		font.loadFromFile(FontOfNum);

		Pos = { 0, 0 };

		Value.setFont(font);
		Index.setFont(font);
		Value.setCharacterSize(25);
		Index.setCharacterSize(20);
		Index.setFillColor(NRed);

		Value.setString(String(""));
		Index.setString(String(to_string(i)));
		Square.loadFromFile("assets/Box.png");
		Box.setTexture(Square);
		Box.setColor(Color(0, 0, 0));

		BoxBack.setSize(Vector2f(46, 46));
		BoxBack.setFillColor(Color(255, 255, 255, 178.5));

		Selecting = 0;
	}

	void changeMemPosition(float x, float y);
	void drawMem();
	void drawMem(int Opacity);
	void changeMemValue(int v);
	void changeMemValue(String v);
};