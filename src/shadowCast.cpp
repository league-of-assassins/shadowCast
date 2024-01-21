
#include "shadowCast.hpp"


shadowCast::shadowCast() {
	initShadowBorder(0, 800, 800, 0);
}


void shadowCast::initShadowBorder(float topBorder, float rightBorder, float bottomBorder, float leftBorder) {

	//ERROR CATCHING
	if (leftBorder >= rightBorder) { 
		leftBorder = 0; rightBorder = 800;
		cerr << "\n shadowCast.cpp: Left border position cant be greater than right. Reverted to default";
	}

	if (topBorder >= bottomBorder) {
		topBorder = 0; bottomBorder = 800;
		cerr << "\n shadowCast.cpp: top border position cant be greater than bottom. Reverted to default";
	}

	//SET
	border[AXIS_X][BORDER_MIN] = leftBorder;
	border[AXIS_X][BORDER_MAX] = rightBorder;
	border[AXIS_Y][BORDER_MIN] = topBorder;
	border[AXIS_Y][BORDER_MAX] = bottomBorder;

	borderCube.setFillColor(Color::Black);
	borderCube.setPosition(Vector2f(border[AXIS_X][BORDER_MIN], border[AXIS_Y][BORDER_MIN]));
	borderCube.setSize(Vector2f(border[AXIS_X][BORDER_MAX] - border[AXIS_X][BORDER_MIN], border[AXIS_Y][BORDER_MAX] - border[AXIS_Y][BORDER_MIN]));
	borderCube.setOutlineColor(Color::Red);
	borderCube.setOutlineThickness(-1);
}


void shadowCast::drawShadows(RenderWindow& window) {
	window.draw(borderCube);

	for (int i = 0; i < shadowTotal; i++) {
		window.draw(shadow[i]);
	}
}


void shadowCast::addShadow(Vector2f startPoint, Vector2f endPoint, Color shadowColor = Color::White) {

	for (int i = 0; i < POINT_FIXED_TOTAL; i++) {
		fixedPoint.push_back(Vector2f());
		fixedBorderHitPos.push_back(Vector2f());
		fixedBorderHitSide.push_back(Vector2i());
	}

	shadow.push_back(ConvexShape(POINT_TOTAL));
	shadow[shadowTotal].setFillColor(shadowColor);

	setFixedPoint(startPoint, endPoint, shadowTotal);

	shadowTotal++;
}


void shadowCast::changeShadowPos(Vector2f startPoint, Vector2f endPoint, int changeNo) {
	
	if (shadowTotal > 0 && changeNo < shadowTotal && changeNo >= 0) {

		setFixedPoint(startPoint, endPoint, changeNo);
	}

	else {
		cerr << "\n\n shadowCast.cpp: changeNo is out of scope";
	}
}


void shadowCast::setFixedPoint(Vector2f& startPoint, Vector2f& endPoint, int No) {

	fixedPointErrorCatch(startPoint, endPoint, No);

	fixedPoint[No * POINT_FIXED_TOTAL + POINT_START] = startPoint;
	fixedPoint[No * POINT_FIXED_TOTAL + POINT_END] = endPoint;

	for (int j = 0; j < POINT_FIXED_TOTAL; j++) {
		shadow[No].setPoint(j, fixedPoint[No * POINT_FIXED_TOTAL + j]);
	}


	//FIND FIXED BORDER POINTS
	float h, v, m, b;

	for (int n = 0; n < POINT_FIXED_TOTAL; n++) {

		findSlope(fixedPoint[No * POINT_FIXED_TOTAL + !n], fixedPoint[No * POINT_FIXED_TOTAL + n], h, v, m, b);

		for (int j = 0; j < AXIS_TOTAL; j++) {

			if (findBorderIntersection(fixedBorderHitPos[No * POINT_FIXED_TOTAL + n], fixedBorderHitSide[No * POINT_FIXED_TOTAL + n], h, v, m, b, j)) break;
		}
	}
}


void shadowCast::fixedPointErrorCatch(Vector2f& startPoint, Vector2f& endPoint, int No) {

	float pos[2][2] = {
		{startPoint.x, startPoint.y},
		{endPoint.x, endPoint.y}
	};

	string whichPos[2] = { "start", "end" };
	string axis[2][2] = { {"x", "y"}, { "lower", "greater" } };

	for (int n = 0; n < POINT_FIXED_TOTAL; n++) {
		for (int i = 0; i < AXIS_TOTAL; i++) {
			if (pos[n][i] <= border[i][BORDER_MIN]) {
				pos[n][i] = border[i][BORDER_MIN] + 1;
				cerr << "\n\n Warning: shadow[" << No << "] " << whichPos[n] << "Pos." << axis[0][i] << " is " << axis[1][BORDER_MIN] << " than border. It was fixed to border";
			}

			if (pos[n][i] >= border[i][BORDER_MAX]) {
				pos[n][i] = border[i][BORDER_MAX] - 1;
				cerr << "\n\n Warning: shadow[" << No << "] " << whichPos[n] << "Pos." << axis[0][i] << " is " << axis[1][BORDER_MAX] << " than border. It was fixed to border";
			}
		}
	}

	startPoint.x = pos[POINT_START][AXIS_X];
	startPoint.y = pos[POINT_START][AXIS_Y];

	endPoint.x = pos[POINT_END][AXIS_X];
	endPoint.y = pos[POINT_END][AXIS_Y];
}


void shadowCast::removeShadow(int removeNo) {

	if (shadowTotal > 0 && removeNo < shadowTotal && removeNo >= 0) {

		for (int i = 0; i < POINT_FIXED_TOTAL; i++) {
			fixedPoint.erase(fixedPoint.begin() + removeNo * POINT_FIXED_TOTAL);
			fixedBorderHitPos.erase(fixedBorderHitPos.begin() + removeNo * POINT_FIXED_TOTAL);
			fixedBorderHitSide.erase(fixedBorderHitSide.begin() + removeNo * POINT_FIXED_TOTAL);
		}

		shadow.erase(shadow.begin() + removeNo);


		shadowTotal--;
	}

	else {
		cerr << "\n\n shadowCast.cpp: removeNo is out of scope";
	}
}




int shadowCast::findSide(float distance) {

	if (distance < 0) return 1;
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
		b = posA.x;
	}
}


bool shadowCast::findBorderIntersection(Vector2f& posV, Vector2i& borderSide, float h, float v, float m, float b, int axis) {

	float pos[2] = { posV.x, posV.y };

	float distances[2] = { h, v };

	if (undefined) axis = AXIS_Y;


	borderSide.x = axis;
	borderSide.y = findSide(distances[axis]);


	pos[axis] = border[axis][borderSide.y];

	if (axis == AXIS_X || undefined) {
		undefined = false;
		pos[!axis] = pos[axis] * m + b;
	}

	else {
		pos[!axis] = (pos[axis] - b) / m;
	}


	if (pos[!axis] >= border[!axis][BORDER_MIN] && pos[!axis] <= border[!axis][BORDER_MAX]) {
		posV.x = pos[AXIS_X];
		posV.y = pos[AXIS_Y];

		return true;
	}

	return false;
}


void shadowCast::findCorners(Vector2f outerPoint[2], Vector2f innerPoint[2], Vector2f& midPoint, vector<Vector2i>& outerBorderHitSide, int m) {

	float outerPointArr[2][2] = {
		{outerPoint[POINT_START].x, outerPoint[POINT_START].y},
		{outerPoint[POINT_END].x, outerPoint[POINT_END].y}
	};

	float fixedBorderIntersectPosArr[2][2] = {
		{fixedBorderHitPos[m * POINT_FIXED_TOTAL + POINT_START].x, fixedBorderHitPos[m * POINT_FIXED_TOTAL + POINT_START].y},
		{fixedBorderHitPos[m * POINT_FIXED_TOTAL + POINT_END].x, fixedBorderHitPos[m * POINT_FIXED_TOTAL + POINT_END].y}
	};

	int axis, temp, count = 0;

	bool midEnabled = false;


	for (int i = 0; i < 2; i++) {

		if (!(outerBorderHitSide[POINT_START].x == outerBorderHitSide[POINT_END].x &&
			outerBorderHitSide[POINT_START].y == outerBorderHitSide[POINT_END].y)) {

			axis = fixedBorderHitSide[m * POINT_FIXED_TOTAL + i].x;

			if (outerBorderHitSide[i].x == fixedBorderHitSide[m * POINT_FIXED_TOTAL + i].x) {
				if (outerBorderHitSide[i].y == fixedBorderHitSide[m * POINT_FIXED_TOTAL + i].y) {

					if (outerPointArr[i][!axis] > fixedBorderIntersectPosArr[i][!axis]) temp = 1;
					else temp = 0;

					outerPointArr[i][!axis] = border[!axis][temp];

					count++;
				}
			}

			else if (fixedBorderHitSide[m * POINT_FIXED_TOTAL + POINT_START].x != fixedBorderHitSide[m * POINT_FIXED_TOTAL + POINT_END].x) {
				midEnabled = true;
			}
		}

		innerPoint[i].x = outerPointArr[i][AXIS_X];
		innerPoint[i].y = outerPointArr[i][AXIS_Y];
	}


	if (count == 2 && fixedBorderHitSide[m * POINT_FIXED_TOTAL + POINT_START].x != fixedBorderHitSide[m * POINT_FIXED_TOTAL + POINT_END].x && innerPoint[POINT_START] != innerPoint[POINT_END]) {
		midEnabled = true;
	}

	if (midEnabled) {

		outerPointArr[POINT_START][!axis] = border[!axis][!fixedBorderHitSide[m * POINT_FIXED_TOTAL + POINT_START].y];
		outerPointArr[POINT_START][axis] = border[axis][!fixedBorderHitSide[m * POINT_FIXED_TOTAL + POINT_END].y];

		midPoint.x = outerPointArr[POINT_START][AXIS_X];
		midPoint.y = outerPointArr[POINT_START][AXIS_Y];
	}

	else {
		midPoint = innerPoint[POINT_START];
	}
}


void shadowCast::updateShadows(Vector2f basePos) {

	//	POINT PARENTS:
	//	FIXED->OUTER->INNER | MID
	//	0->6->5 | 4 | 1->2->3

	float h, v, m, b;

	Vector2f outerPoint[POINT_FIXED_TOTAL];
	Vector2f innerPoint[POINT_FIXED_TOTAL];
	Vector2f midPoint;

	vector<Vector2i> outerBorderHitSide = { Vector2i(), Vector2i() };


	for (int i = 0; i < shadowTotal; i++) {

		//---FIND POINTS 6, 2 (OUTER)---//
		for (int n = 0; n < POINT_FIXED_TOTAL; n++) {

			findSlope(basePos, fixedPoint[i * POINT_FIXED_TOTAL + n], h, v, m, b);

			for (int j = 0; j < AXIS_TOTAL; j++) {
				if (findBorderIntersection(outerPoint[n], outerBorderHitSide[n], h, v, m, b, j)) break;
			}
		}


		//---FIND POINTS 5, 3 (INNER), 4 (MID)---//

		findCorners(outerPoint, innerPoint, midPoint, outerBorderHitSide, i);


		//SET POINTS
		for (int j = 0; j < POINT_FIXED_TOTAL; j++) {
			shadow[i].setPoint(OUTER_POINT[j], outerPoint[j]);
			shadow[i].setPoint(INNER_POINT[j], innerPoint[j]);
		}

		shadow[i].setPoint(MID_POINT, midPoint);
	}
}
