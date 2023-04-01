#pragma once
#include <iostream>
#include <algorithm>
#include <math.h>
#include <string.h>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

using namespace sf;
using namespace std;

const double PI = 3.14159265359;
const float DefaultPosX = 350;
const float DefaultPosY = 270;

const int maxNodeNumber = 10;
const int waitTime = 700;


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
