#pragma once
#include "SFML/Graphics.hpp"

using namespace std;
using namespace sf;

struct Button {
	Vector2i tL = Vector2i(0, 0);
	Vector2i bR = Vector2i(0, 0);
	Texture t;
	Sprite ButtonTexture;

	bool ButtonToggle(Vector2i pos) {
		int x = pos.x;
		int y = pos.y;

		return ((tL.x <= x && tL.y <= y && x <= bR.x && y <= bR.y));
	}

	void setButtonPos(int Top, int Left, int Bottom, int Right) {
		tL = Vector2i(Top, Left);
		bR = Vector2i(Bottom, Right);
	}

	void setButtonTexture(string s) {
		float x = tL.x;
		float y = tL.y;

		t.loadFromFile(s);
		ButtonTexture.setTexture(t);
		ButtonTexture.setPosition(Vector2f(x, y));
	}

	void DrawButton(RenderWindow& app) {

		Vector2i pos = Mouse::getPosition(app);
		if (ButtonToggle(pos)) {
			int x = pos.x;
			int y = pos.y;

			RectangleShape Border;
			Border.setSize(Vector2f(bR.x - tL.x + 2, bR.y - tL.y + 2));
			Border.setOutlineColor(Color(255, 255, 255));
			Border.setOutlineThickness(2);
			Border.setPosition(tL.x - 1, tL.y - 1);

			app.draw(Border);
		}

		app.draw(ButtonTexture);
	}
};