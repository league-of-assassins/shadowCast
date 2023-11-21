
#pragma once

#include "main.hpp"



///////////////// ESSENTIALS /////////////////



void shadowCast::setWindow() {
	window.create(VideoMode(width, height), "shadowCast", Style::None);
	window.setFramerateLimit(60);

	windowGap.x = window.getPosition().x;
	windowGap.y = window.getPosition().y;
}

void shadowCast::events() {
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed) window.close();

		else if (event.type == Event::KeyPressed) {

			if (event.key.code == Keyboard::Escape) {
				window.close();
			}
		}
	}
}


shadowCast::shadowCast() {
	objects();
	setWindow();


	while (window.isOpen())
	{
		events();

		general();

		displays();
	}
}

void shadowCast::general() {

	mouseLogic();

	shadowMain();

	setPositions();
}

void shadowCast::displays() {

	window.clear(Color::Black);


	for (int i = 0; i < wallTotal; i++) {
		window.draw(wall[i]);
	}

	for (int i = 0; i < wallTotal; i++) {
		window.draw(shadow[i]);
	}


	window.display();
}



void shadowCast::objects() {


	//WALL

	for (int i = 0; i < wallTotal; i++) {
		wall[i].setFillColor(Color::Black);

		wallSize[i].x = wallValues[i][0];
		wallSize[i].y = wallValues[i][1];
		wall[i].setSize(wallSize[i]);

		wall[i].setOrigin(0, wallSize[i].y / 2.0);

		wallPos[i][0].x = wallValues[i][2];
		wallPos[i][0].y = wallValues[i][3];
		wall[i].setPosition(wallPos[i][0]);

		wallAngle[i] = wallValues[i][4];
		wall[i].setRotation(wallAngle[i]);
	}


	//FIND WALL END POINT
	for (int i = 0; i < wallTotal; i++) {
		wallPos[i][1].x = wallPos[i][0].x + wallSize[i].x * cos(wallAngle[i] * radian);
		wallPos[i][1].y = wallPos[i][0].y + wallSize[i].x * sin(wallAngle[i] * radian);
	}


	//FIND WALL BORDER POINTS
	float h, v, m, b;

	for (int i = 0; i < wallTotal; i++) {
		findSlope(wallPos[i][0], wallPos[i][1], h, v, m, b);

		for (int n = 0; n < 2; n++) {

			h *= -1;
			v *= -1;

			for (int j = 0; j < 2; j++) {

				if (findBorder(wallBorderPos[i][n], h, v, m, b, j, wallBorderSide[i][n])) break;
			}
		}
	}



	//SHADOW

	for (int i = 0; i < wallTotal; i++) {

		shadow[i].setPointCount(pointsTotal);
		shadow[i].setFillColor(Color::White);

		for (int j = 0; j < 2; j++) {
			shadowPos[i][j] = wallPos[i][j];
			shadow[i].setPoint(j, shadowPos[i][j]);
		}
	}
}



///////////////// SHADOW CAST /////////////////



int shadowCast::findSide(float s) {

	if (s < 0) return 1;
	else return 0;
}



void shadowCast::findSlope(Vector2f posA, Vector2f posB, float& h, float& v, float& m, float& b) {
	h = posA.x - posB.x;
	v = posA.y - posB.y;

	m = v / h;

	b = posA.y - posA.x * m;


	if (abs(h) < 1) {
		undefined = true;

		m = 0;
		b = posB.x;
	}
}



bool shadowCast::findBorder(Vector2f& pos, float h, float v, float m, float b, int side, int sideArray[2]) {


	if (undefined) side = 1;

	float distance[2] = { h, v };

	sideArray[0] = side;
	sideArray[1] = findSide(distance[side]);

	pos.x = border[side][sideArray[1]];

	if (side == 0 || undefined) {
		if (undefined) undefined = false;
		pos.y = pos.x * m + b;
	}

	else {
		pos.y = (pos.x - b) / m;
	}

	if (side == 1) {
		float temp = pos.x;
		pos.x = pos.y;
		pos.y = temp;
	}

	if (pos.y >= border[side][0] && pos.y <= border[side][1]) {
		return true;
	}

	else return false;
}



void shadowCast::findCorner(Vector2f pointPos[pointsTotal], Vector2f wallBorderPos[2], int pointSide[2][2], int wallSide[2][2]) {

	float point[3][2] = {
		{pointPos[6].x, pointPos[6].y},
		{pointPos[2].x, pointPos[2].y}
	};

	float wall[2][2] = {
	{wallBorderPos[0].x, wallBorderPos[0].y},
	{wallBorderPos[1].x, wallBorderPos[1].y}
	};

	int side, temp, count = 0, k = 5;

	bool midEnabled = false;


	for (int i = 0; i < 2; i++) {

		if (!(pointSide[0][0] == pointSide[1][0] &&
			pointSide[0][1] == pointSide[1][1])) {

			side = wallSide[i][0];

			if (pointSide[i][0] == wallSide[i][0]) {
				if (pointSide[i][1] == wallSide[i][1]) {

					if (point[i][!side] > wall[i][!side]) temp = 1;
					else temp = 0;

					point[i][!side] = border[!side][temp];

					count++;
				}
			}

			else {
				midEnabled = true;

				point[2][!side] = border[!side][pointSide[i][1]];
				point[2][side] = border[side][!wallSide[i][1]];
			}

			if (count == 2 && wallSide[0][0] != wallSide[1][0]) {
				midEnabled = true;

				point[2][!side] = point[1][!side];
				point[2][side] = point[0][side];
			}
		}

		pointPos[k].x = point[i][0];
		pointPos[k].y = point[i][1];

		k = 3;
	}

	if (!midEnabled) {
		point[2][0] = point[0][0];
		point[2][1] = point[0][1];
	}

	pointPos[4].x = point[2][0];
	pointPos[4].y = point[2][1];

}



void shadowCast::shadowMain() {

	float h, v, m, b;

	int pointSide[2][2] = {};

	for (int i = 0; i < wallTotal; i++) {

		//---FIND POINTS 2, 6 (OUTER)---//

		for (int p = 0; p < 2; p++) {

			findSlope(mousePos, shadowPos[i][p], h, v, m, b);

			for (int j = 0; j < 2; j++) {
				if (findBorder(shadowPos[i][6 - (p * 4)], h, v, m, b, j, pointSide[p])) break;
			}
		}


		//---FIND POINTS 3, 4, 5 (INNER)---//

		findCorner(shadowPos[i], wallBorderPos[i], pointSide, wallBorderSide[i]);
	}
}



///////////////// OTHERS /////////////////



void shadowCast::mouseLogic() {

	//FIND MOUSE POS
	mousePos.x = Mouse::getPosition().x - windowGap.x;
	mousePos.y = Mouse::getPosition().y - windowGap.y;

}



void shadowCast::setPositions() {

	//SHADOW POINTS
	for (int i = 0; i < wallTotal; i++) {
		for (int j = 2; j < pointsTotal; j++) {
			shadow[i].setPoint(j, shadowPos[i][j]);
		}
	}
}





int main() {

	shadowCast shadowCast;

	return 0;
}
