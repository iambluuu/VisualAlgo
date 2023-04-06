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
	tgui::Button::Ptr Hitbox;

	Nodestate NodeState;
	Nodestate ArrowState;

	string Val;
	Node* nxt;
	Node* prev;

	Vector2f Pos;

	Texture Circle;
	Sprite NodeShape;
	CircleShape NodeBack;

	Font font;
	Text Value;

	Texture ArrowTexture;
	Sprite Arrow;
	Sprite TmpArrow;
	
	int NumberInList;
	Text Title;

	Node(int v) {
		Hitbox = tgui::Button::create();
		Hitbox->setSize({ 46, 46 });
		Hitbox->getRenderer()->setRoundedBorderRadius(23);
		gui.add(Hitbox);

		Hitbox->onPress([=] {
			tgui::EditBox::Ptr InsertPos = gui.get<tgui::EditBox>("InsertPos");
			tgui::EditBox::Ptr DeletePos = gui.get<tgui::EditBox>("DeletePos");

			InsertPos->setText(tgui::String(to_string(NumberInList)));
			DeletePos->setText(tgui::String(to_string(NumberInList)));
			});

		ArrowState = Normal;
		NodeState = Normal;

		Pos = { 0, 0 };
		Val = "0";
		nxt = nullptr;
		prev = nullptr;

		font.loadFromFile("assets/SpaceGrotesk-Bold.ttf");
		Value.setFont(font);
		Value.setFillColor(Color(0, 0, 0));
		changeNodeValue(v);

		Circle.loadFromFile("assets/Circle.png");
		Circle.setSmooth(true);
		NodeShape.setTexture(Circle);
		NodeShape.setColor(Color(0, 0, 0));
		NodeBack.setRadius(21);
		NodeBack.setFillColor(Color(255, 255, 255));

		ArrowTexture.loadFromFile("assets/arrow.png");
		ArrowTexture.setSmooth(true);

		Arrow.setTexture(ArrowTexture);
		Arrow.setPosition(Pos.x + 40, Pos.y + 15);
		TmpArrow.setTexture(ArrowTexture);
		TmpArrow.setColor(Color(193, 148, 243));

		changeNodePosition(DefaultPosX, DefaultPosY);

		NumberInList = -1;
		Title.setFont(font);
		Title.setCharacterSize(15);
		Title.setFillColor(Color(229, 9, 20));
		Title.setOutlineColor(Color::Black);
	}

	Node(String v) {
		Hitbox = tgui::Button::create();
		Hitbox->setSize({ 46, 46 });
		Hitbox->getRenderer()->setRoundedBorderRadius(23);
		gui.add(Hitbox);

		Hitbox->onPress([=] {
			tgui::EditBox::Ptr InsertPos = gui.get<tgui::EditBox>("InsertPos");
			tgui::EditBox::Ptr DeletePos = gui.get<tgui::EditBox>("DeletePos");

			InsertPos->setText(tgui::String(to_string(NumberInList)));
			DeletePos->setText(tgui::String(to_string(NumberInList)));
			});

		ArrowState = Normal;
		NodeState = Normal;

		Pos = { 0, 0 };
		Val = "0";
		nxt = nullptr;
		prev = nullptr;

		font.loadFromFile("assets/SpaceGrotesk-Bold.ttf");
		Value.setFont(font);
		Value.setFillColor(Color(0, 0, 0));
		changeNodeValue(v);

		Circle.loadFromFile("assets/Circle.png");
		Circle.setSmooth(true);
		NodeShape.setTexture(Circle);
		NodeShape.setColor(Color(0, 0, 0));
		NodeBack.setRadius(21);
		NodeBack.setFillColor(Color(255, 255, 255));

		ArrowTexture.loadFromFile("assets/arrow.png");
		ArrowTexture.setSmooth(true);

		Arrow.setTexture(ArrowTexture);
		Arrow.setPosition(Pos.x + 40, Pos.y + 15);
		TmpArrow.setTexture(ArrowTexture);
		TmpArrow.setColor(Color(193, 148, 243));

		changeNodePosition(DefaultPosX, DefaultPosY);

		NumberInList = -1;
		Title.setFont(font);
		Title.setCharacterSize(15);
		Title.setFillColor(Color(229, 9, 20));
		Title.setOutlineColor(Color::Black);
	}

	~Node() {
		gui.remove(Hitbox);
	}
	
	void changeNodePosition(float x, float y);
	void updateArrow(Node* Nxt);
	void changeNodeValue(int val);
	void changeNodeValue(String val);
	void drawNode(RenderWindow& app, int Opacity);
	void drawArrow(RenderWindow& app);
	void drawArrow(RenderWindow& app, int Opacity);

};