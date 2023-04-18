#pragma once
#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <string.h>
#include <functional>

#include "TGUI/TGUI.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

using namespace sf;
using namespace std;

__declspec(selectany) tgui::Gui gui;
__declspec(selectany) RenderWindow app(VideoMode(1600, 900), "VisualAlgo", Style::Close);

const double PI = 3.14159265359;
const float DefaultPosX = 250;
const float DefaultPosY = 250;

const int maxNodeNumber = 15;
const int waitTime = 700;

static Color VSPurple(193, 148, 243);
static Color SGreen(30, 215, 96);
static Color NRed(229, 9, 20);
static Color FBlue(66, 103, 178);
static Color CPink(228, 113, 122);
static Color Fulvous(220, 124, 0);

enum ScreenState {
	_SLL, _DLList, _CLL, _Stack, _Queue, _DArray, _Array, _EndProgram
};	

__declspec(selectany) ScreenState State = _SLL;

namespace Util {

	inline void Wait() {
		int Elapsed = 0;
		Clock clock;

		while (Elapsed <= waitTime)
			Elapsed = clock.getElapsedTime().asMilliseconds();

	}

	inline double DistanceBetweenNodes(Vector2f A, Vector2f B) {
		return sqrt( (A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y) );
	}

	inline double AngleOfArrow(Vector2f A, Vector2f B) {
		if (A.x - B.x == 0)
			return (A.y > B.y) ? -PI/2 : PI/2;

		return atan((A.y - B.y) / (A.x - B.x));
	}
}
