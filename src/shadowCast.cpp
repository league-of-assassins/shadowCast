

#include "shadowCast.hpp"


shadowCast::shadowCast() {
	initShadowBorder(0, 800, 800, 0);
}


void shadowCast::initShadowBorder(float topBorder, float rightBorder, float bottomBorder, float leftBorder) {

	//ERROR CATCHING
	if (leftBorder >= rightBorder) { 
		leftBorder = 0; rightBorder = 800;
		cout << "\n shadowCast.cpp: Left border position cant be greater than right. Reverted to default";
	}

	if (topBorder >= bottomBorder) {
		topBorder = 0; bottomBorder = 800;
		cout << "\n shadowCast.cpp: top border position cant be greater than bottom. Reverted to default";
	}

	//SET
	border[0][0] = leftBorder;
	border[0][1] = rightBorder;
	border[1][0] = topBorder;
	border[1][1] = bottomBorder;

	borderCube.setFillColor(Color::Black);
	borderCube.setPosition(Vector2f(border[0][0], border[1][0]));
	borderCube.setSize(Vector2f(border[0][1] - border[0][0], border[1][1] - border[1][0]));
	borderCube.setOutlineColor(Color::Red);
	borderCube.setOutlineThickness(-1);
}


void shadowCast::drawShadows(RenderWindow& window) {
	window.draw(borderCube);

	for (int i = 0; i < shadowTotal; i++) {
		window.draw(shadow[i]);
	}
}


void shadowCast::addShadow(Vector2f startPos, Vector2f endPos, Color shadowColor = Color::White) {
	
	fixedShadowPosErrorCatch(startPos, endPos, shadowTotal);
	
	//SHADOW

	for (int i = 0; i < 7; i++) {
		shadowPos.push_back(Vector2f());
	}

	shadow.push_back(ConvexShape(7));
	shadow[shadowTotal].setFillColor(shadowColor);


	//FIXED BORDER POINTS

	fixedBorderPos.push_back(Vector2f());
	fixedBorderPos.push_back(Vector2f());

	fixedBorderSide.push_back(Vector2i());
	fixedBorderSide.push_back(Vector2i());

	setShadowFixedPos(startPos, endPos, shadowTotal);

	shadowTotal++;
}


void shadowCast::changeShadowPos(Vector2f startPos, Vector2f endPos, int changeNo) {
	
	if (shadowTotal > 0 && changeNo < shadowTotal && changeNo >= 0) {

		setShadowFixedPos(startPos, endPos, changeNo);
	}

	else {
		cout << "\n\n shadowCast.cpp: changeNo is out of scope";
	}
}


void shadowCast::setShadowFixedPos(Vector2f& startPos, Vector2f& endPos, int No) {

	fixedShadowPosErrorCatch(startPos, endPos, No);

	shadowPos[No * 7] = startPos;
	shadowPos[No * 7 + 1] = endPos;

	for (int j = 0; j < 2; j++) {
		shadow[No].setPoint(j, shadowPos[No * 7 + j]);
	}


	//FIND FIXED BORDER POINTS
	float h, v, m, b;

	for (int n = 0; n < 2; n++) {

		findSlope(shadowPos[No * 7 + !n], shadowPos[No * 7 + n], h, v, m, b);

		for (int j = 0; j < 2; j++) {

			if (findBorderIntersection(fixedBorderPos[No * 2 + n], fixedBorderSide[No * 2 + n], h, v, m, b, j)) break;
		}
	}
}


void shadowCast::fixedShadowPosErrorCatch(Vector2f& StartPos, Vector2f& EndPos, int No) {

	float pos[2][2] = {
		{StartPos.x, StartPos.y},
		{EndPos.x, EndPos.y}
	};

	string whichPos[2] = { "start", "end" };
	string side[2][2] = { {"x", "y"}, { "lower", "greater" } };

	for (int n = 0; n < 2; n++) {
		for (int i = 0; i < 2; i++) {
			if (pos[n][i] <= border[i][0]) {
				pos[n][i] = border[i][0] + 1;
				cout << "\n\n Warning: shadow[" << No << "] " << whichPos[n] << "Pos." << side[0][i] << " is " << side[1][0] << " than border. It was fixed to border";
			}

			if (pos[n][i] >= border[i][1]) {
				pos[n][i] = border[i][1] - 1;
				cout << "\n\n Warning: shadow[" << No << "] " << whichPos[n] << "Pos." << side[0][i] << " is " << side[1][1] << " than border. It was fixed to border";
			}
		}
	}

	StartPos.x = pos[0][0];
	StartPos.y = pos[0][1];

	EndPos.x = pos[1][0];
	EndPos.y = pos[1][1];
}


void shadowCast::removeShadow(int removeNo) {

	if (shadowTotal > 0 && removeNo < shadowTotal && removeNo >= 0) {
		for (int i = 0; i < 7; i++) {
			shadowPos.erase(shadowPos.begin() + removeNo * 7);
		}

		shadow.erase(shadow.begin() + removeNo);

		for (int i = 0; i < 2; i++) {
			fixedBorderPos.erase(fixedBorderPos.begin() + removeNo * 2);
			fixedBorderSide.erase(fixedBorderSide.begin() + removeNo * 2);
		}

		shadowTotal--;
	}

	else {
		cout << "\n\n shadowCast.cpp: removeNo is out of scope";
	}
}




int shadowCast::findSide(float s) {

	if (s < 0) return 1;
	return 0;
}


void shadowCast::findSlope(Vector2f posA, Vector2f posB, float& h, float& v, float& m, float& b) {
	h = posA.x - posB.x;
	v = posA.y - posB.y;

	m = v / h;

	b = posA.y - posA.x * m;


	if (h == 0) {
		undefined = true;

		m = 0;
		b = posB.x;
	}
}


bool shadowCast::findBorderIntersection(Vector2f& posV, Vector2i& borderSide, float h, float v, float m, float b, int side) {

	float pos[2] = { posV.x, posV.y };

	float distance[2] = { h, v };

	if (undefined) side = 1;


	borderSide.x = side;
	borderSide.y = findSide(distance[side]);

	pos[side] = border[side][borderSide.y];

	if (side == 0 || undefined) {
		undefined = false;
		pos[!side] = pos[side] * m + b;
	}

	else {
		pos[!side] = (pos[side] - b) / m;
	}


	if (pos[!side] >= border[!side][0] && pos[!side] <= border[!side][1]) {
		posV.x = pos[0];
		posV.y = pos[1];

		return true;
	}

	return false;
}


void shadowCast::findCorner(vector<Vector2f>& ShadowPos, vector<Vector2f>& FixedBorderPos, vector<Vector2i>& PointBorderSide, vector<Vector2i>& FixedBorderSide, int m) {

	float outerPoint[2][2] = {
		{ShadowPos[m * 7 + 6].x, ShadowPos[m * 7 + 6].y},
		{ShadowPos[m * 7 + 2].x, ShadowPos[m * 7 + 2].y}
	};

	float fixedPoint[2][2] = {
		{FixedBorderPos[m * 2].x, FixedBorderPos[m * 2].y},
		{FixedBorderPos[m * 2 + 1].x, FixedBorderPos[m * 2 + 1].y}
	};

	int side, temp, count = 0, k = 5;

	bool midEnabled = false;


	for (int i = 0; i < 2; i++) {

		if (!(PointBorderSide[0].x == PointBorderSide[1].x &&
			PointBorderSide[0].y == PointBorderSide[1].y)) {

			side = FixedBorderSide[m * 2 + i].x;

			if (PointBorderSide[i].x == FixedBorderSide[m * 2 + i].x) {
				if (PointBorderSide[i].y == FixedBorderSide[m * 2 + i].y) {

					if (outerPoint[i][!side] > fixedPoint[i][!side]) temp = 1;
					else temp = 0;

					outerPoint[i][!side] = border[!side][temp];

					count++;
				}
			}

			else if (FixedBorderSide[m * 2].x != FixedBorderSide[m * 2 + 1].x) {
				midEnabled = true;
			}
		}

		ShadowPos[m * 7 + k].x = outerPoint[i][0];
		ShadowPos[m * 7 + k].y = outerPoint[i][1];

		k = 3;
	}


	if (count == 2 && FixedBorderSide[m * 2].x != FixedBorderSide[m * 2 + 1].x && ShadowPos[m * 7 + 3] != ShadowPos[m * 7 + 5]) {
		midEnabled = true;
	}

	if (midEnabled) {

		outerPoint[0][!side] = border[!side][!FixedBorderSide[m * 2].y];
		outerPoint[0][side] = border[side][!FixedBorderSide[m * 2 + 1].y];

		ShadowPos[m * 7 + 4].x = outerPoint[0][0];
		ShadowPos[m * 7 + 4].y = outerPoint[0][1];
	}

	else {
		ShadowPos[m * 7 + 4] = ShadowPos[m * 7 + 3];
	}
}


void shadowCast::updateShadows(Vector2f basePos) {

	//	POINT PARENTS:
	//	FIXED->OUTER->INNER | MID
	//	0->6->5 | 4 | 1->2->3

	float h, v, m, b;

	vector<Vector2i> pointBorderSide = { Vector2i(), Vector2i() };

	for (int i = 0; i < shadowTotal; i++) {

		//---FIND POINTS 6, 2 (OUTER)---//
		for (int n = 0; n < 2; n++) {

			findSlope(basePos, shadowPos[i * 7 + n], h, v, m, b);

			for (int j = 0; j < 2; j++) {
				if (findBorderIntersection(shadowPos[i * 7 + 6 - (n * 4)], pointBorderSide[n], h, v, m, b, j)) break;
			}
		}


		//---FIND POINTS 5, 3 (INNER), 4 (MID)---//

		findCorner(shadowPos, fixedBorderPos, pointBorderSide, fixedBorderSide, i);
	}

	setPositions();
}


void shadowCast::setPositions() {

	//SHADOW POINTS
	for (int i = 0; i < shadowTotal; i++) {
		for (int j = 2; j < 7; j++) {
			shadow[i].setPoint(j, shadowPos[i * 7 + j]);
		}
	}
}
