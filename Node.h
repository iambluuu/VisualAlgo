#pragma once

#include "SFML/Graphics.hpp"
#include "Global.h"
#include <string>

using namespace std;
using namespace sf;

enum Nodestate {
	Normal, Visited, Selecting, Next, New, Remove
};


struct Node {

	Nodestate NodeState;

	string Val;
	Node* nxt;
	Node* prev;

	Vector2f Pos;

	CircleShape NodeShape;

	Font font;
	Text Value;

	Texture ArrowTexture;
	Sprite Arrow;
	 
	Node() {
		NodeState = Normal;
		Pos = { 0, 0 };
		Val = "0";
		nxt = nullptr;
		prev = nullptr;

		font.loadFromFile("assets/SpaceGrotesk-Bold.ttf");
		Value.setFont(font);
		Value.setFillColor(Color(0, 0, 0));
		Value.setString(Val);

		NodeShape.setRadius(20);
		NodeShape.setFillColor(Color(255, 255, 255));
		NodeShape.setOutlineThickness(3);

		//NodeShape.setFillColor(Color(102, 255, 102));
		NodeShape.setOutlineColor(Color(0, 0, 0));

		ArrowTexture.loadFromFile("assets/arrow.png");
		ArrowTexture.setSmooth(true);

		Arrow.setTexture(ArrowTexture);
		Arrow.setPosition(Pos.x + 40, Pos.y + 15);

		changeNodePosition(DefaultPosX, DefaultPosY);
	}
	
	void changeNodePosition(float x, float y);
	void updateArrow(float x, float y);
	void changeNodeValue(int val);
	void drawNode(RenderWindow& app, int Opacity);
};