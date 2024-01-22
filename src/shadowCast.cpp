
#include "shadowCast.hpp"


shadowCast::shadowCast() {

	initShadowBorder(0, 800, 800, 0);
}


void shadowCast::initShadowBorder(float topBorder, float rightBorder, float bottomBorder, float leftBorder) {

	//ERROR CATCHING
	if (leftBorder >= rightBorder) { 
		leftBorder = 0; rightBorder = 800;
		std::cerr << "\n shadowCast.cpp: Left border position cant be greater than right. Reverted to default";
	}

	if (topBorder >= bottomBorder) {
		topBorder = 0; bottomBorder = 800;
		std::cerr << "\n shadowCast.cpp: top border position cant be greater than bottom. Reverted to default";
	}

	//SET
	border[AXIS_X][BORDER_MIN] = leftBorder;
	border[AXIS_X][BORDER_MAX] = rightBorder;
	border[AXIS_Y][BORDER_MIN] = topBorder;
	border[AXIS_Y][BORDER_MAX] = bottomBorder;

	borderCube.setFillColor(sf::Color::Black);
	borderCube.setPosition(sf::Vector2f(border[AXIS_X][BORDER_MIN], border[AXIS_Y][BORDER_MIN]));
	borderCube.setSize(sf::Vector2f(border[AXIS_X][BORDER_MAX] - border[AXIS_X][BORDER_MIN], border[AXIS_Y][BORDER_MAX] - border[AXIS_Y][BORDER_MIN]));
	borderCube.setOutlineColor(sf::Color::Red);
	borderCube.setOutlineThickness(-1);
}


void shadowCast::drawShadows(sf::RenderWindow& window) {
	window.draw(borderCube);

	for (int i = 0; i < shadowTotal; i++) {
		window.draw(shadow[i]);
	}
}


void shadowCast::addShadow(sf::Vector2f startPoint, sf::Vector2f endPoint, sf::Color shadowColor) {

	for (int i = 0; i < PARENT_TOTAL; i++) {
		parent.push_back({ sf::Vector2f(), sf::Vector2f(), sf::Vector2i() });
	}

	shadow.push_back(sf::ConvexShape(POINT_TOTAL));
	shadow[shadowTotal].setFillColor(shadowColor);

	setFixedPoint(startPoint, endPoint, shadowTotal);

	shadowTotal++;
}


void shadowCast::changeShadowPos(sf::Vector2f startPoint, sf::Vector2f endPoint, int changeNo) {
	
	if (shadowTotal > 0 && changeNo < shadowTotal && changeNo >= 0) {

		setFixedPoint(startPoint, endPoint, changeNo);
	}

	else {
		std::cerr << "\n\n shadowCast.cpp: changeNo: " << changeNo << " is out of scope. Range is 0 to " << shadowTotal - 1;
	}
}


void shadowCast::setFixedPoint(sf::Vector2f& startPoint, sf::Vector2f& endPoint, int No) {

	//SET FIXED POINTS
	fixedPointErrorCatch(startPoint, endPoint, No);

	parent[No * PARENT_TOTAL + PARENT_START].fixedPoint = startPoint;
	parent[No * PARENT_TOTAL + PARENT_END].fixedPoint = endPoint;

	for (int i = 0; i < PARENT_TOTAL; i++) {
		shadow[No].setPoint(FIXED_POINT[i], parent[No * PARENT_TOTAL + i].fixedPoint);
	}


	//FIND FIXED BORDER HIT POS AND SIDE
	float h, v, m, b;

	findSlope(parent[No * PARENT_TOTAL + PARENT_END].fixedPoint, parent[No * PARENT_TOTAL + PARENT_START].fixedPoint, h, v, m, b);

	for (int i = 0; i < PARENT_TOTAL; i++) {

		for (int j = 0; j < AXIS_TOTAL; j++) {

			if (findBorderIntersection(parent[No * PARENT_TOTAL + i].fixedBorderHitPos, parent[No * PARENT_TOTAL + i].fixedBorderHitSide, h, v, m, b, j)) break;
		}

		h *= -1;
		v *= -1;
	}
};


void shadowCast::fixedPointErrorCatch(sf::Vector2f& startPoint, sf::Vector2f& endPoint, int No) {

	float pos[2][2] = {
		{startPoint.x, startPoint.y},
		{endPoint.x, endPoint.y}
	};

	std::string whichPos[2] = { "start", "end" };
	std::string axis[2][2] = { {"x", "y"}, { "lower", "greater" } };

	for (int n = 0; n < PARENT_TOTAL; n++) {
		for (int i = 0; i < AXIS_TOTAL; i++) {
			if (pos[n][i] <= border[i][BORDER_MIN]) {
				pos[n][i] = border[i][BORDER_MIN] + 1;
				std::cerr << "\n\n Warning: shadow[" << No << "] " << whichPos[n] << "Pos." << axis[0][i] << " is " << axis[1][BORDER_MIN] << " than border. It was fixed to border";
			}

			if (pos[n][i] >= border[i][BORDER_MAX]) {
				pos[n][i] = border[i][BORDER_MAX] - 1;
				std::cerr << "\n\n Warning: shadow[" << No << "] " << whichPos[n] << "Pos." << axis[0][i] << " is " << axis[1][BORDER_MAX] << " than border. It was fixed to border";
			}
		}
	}

	startPoint.x = pos[PARENT_START][AXIS_X];
	startPoint.y = pos[PARENT_START][AXIS_Y];

	endPoint.x = pos[PARENT_END][AXIS_X];
	endPoint.y = pos[PARENT_END][AXIS_Y];
}


void shadowCast::removeShadow(int removeNo) {

	if (shadowTotal > 0 && removeNo < shadowTotal && removeNo >= 0) {

		for (int i = 0; i < PARENT_TOTAL; i++) {
			parent.erase(parent.begin() + removeNo * PARENT_TOTAL);
		}
		
		shadow.erase(shadow.begin() + removeNo);

		shadowTotal--;
	}

	else {
		std::cerr << "\n\n shadowCast.cpp: removeNo: " << removeNo << " is out of scope. Range is 0 to " << shadowTotal - 1;
	}
}




int shadowCast::findSide(float distance) {

	if (distance < 0) return BORDER_MAX;
	return BORDER_MIN;
}


void shadowCast::findSlope(sf::Vector2f posA, sf::Vector2f posB, float& h, float& v, float& m, float& b) {
	h = posA.x - posB.x;
	v = posA.y - posB.y;

	m = v / h;
	b = posA.y - posA.x * m;


	if (abs(h) < 0.0001) {
		undefined = true;

		m = 0;
		b = posA.x;
	}
}


bool shadowCast::findBorderIntersection(sf::Vector2f& hitPos, sf::Vector2i& borderSide, float h, float v, float m, float b, int axis) {

	float pos[AXIS_TOTAL] = { hitPos.x, hitPos.y };

	float distances[AXIS_TOTAL] = { h, v };

	if (undefined) axis = AXIS_Y;


	borderSide.x = axis;
	borderSide.y = findSide(distances[axis]);


	pos[axis] = border[axis][borderSide.y];

	if (axis == AXIS_X || undefined) {
		undefined = false;
		pos[!axis] = pos[axis] * m + b;
	}

	else if (axis == AXIS_Y) {
		pos[!axis] = (pos[axis] - b) / m;
	}


	if (pos[!axis] >= border[!axis][BORDER_MIN] && pos[!axis] <= border[!axis][BORDER_MAX]) {
		hitPos.x = pos[AXIS_X];
		hitPos.y = pos[AXIS_Y];

		return true;
	}

	return false;
}


void shadowCast::findCorners(sf::Vector2f outerPoint[PARENT_TOTAL], sf::Vector2f innerPoint[PARENT_TOTAL], sf::Vector2f& midPoint, sf::Vector2i outerBorderHitSide[PARENT_TOTAL], int m) {

	float outerPointArr[PARENT_TOTAL][AXIS_TOTAL] = {
		{outerPoint[PARENT_START].x, outerPoint[PARENT_START].y},
		{outerPoint[PARENT_END].x, outerPoint[PARENT_END].y}
	};

	float fixedBorderIntersectPosArr[PARENT_TOTAL][AXIS_TOTAL] = {
		{parent[m * PARENT_TOTAL + PARENT_START].fixedBorderHitPos.x, parent[m * PARENT_TOTAL + PARENT_START].fixedBorderHitPos.y},
		{parent[m * PARENT_TOTAL + PARENT_END].fixedBorderHitPos.x, parent[m * PARENT_TOTAL + PARENT_END].fixedBorderHitPos.y},
	};

	int axis, temp, count = 0;

	bool midEnabled = false;


	for (int i = 0; i < PARENT_TOTAL; i++) {

		if (!(outerBorderHitSide[PARENT_START].x == outerBorderHitSide[PARENT_END].x &&
			outerBorderHitSide[PARENT_START].y == outerBorderHitSide[PARENT_END].y)) {

			axis = parent[m * PARENT_TOTAL + i].fixedBorderHitSide.x;

			if (outerBorderHitSide[i].x == parent[m * PARENT_TOTAL + i].fixedBorderHitSide.x) {
				if (outerBorderHitSide[i].y == parent[m * PARENT_TOTAL + i].fixedBorderHitSide.y) {

					if (outerPointArr[i][!axis] > fixedBorderIntersectPosArr[i][!axis]) temp = AXIS_Y;
					else temp = AXIS_X;

					outerPointArr[i][!axis] = border[!axis][temp];

					count++;
				}
			}

			else if (parent[m * PARENT_TOTAL + PARENT_START].fixedBorderHitSide.x != parent[m * PARENT_TOTAL + PARENT_END].fixedBorderHitSide.x) {
				midEnabled = true;
			}
		}

		innerPoint[i].x = outerPointArr[i][AXIS_X];
		innerPoint[i].y = outerPointArr[i][AXIS_Y];
	}


	if (count == 2 && parent[m * PARENT_TOTAL + PARENT_START].fixedBorderHitSide.x != parent[m * PARENT_TOTAL + PARENT_END].fixedBorderHitSide.x && innerPoint[PARENT_START] != innerPoint[PARENT_END]) {
		midEnabled = true;
	}

	if (midEnabled) {

		outerPointArr[PARENT_START][!axis] = border[!axis][!parent[m * PARENT_TOTAL + PARENT_START].fixedBorderHitSide.y];
		outerPointArr[PARENT_START][axis] = border[axis][!parent[m * PARENT_TOTAL + PARENT_END].fixedBorderHitSide.y];

		midPoint.x = outerPointArr[PARENT_START][AXIS_X];
		midPoint.y = outerPointArr[PARENT_START][AXIS_Y];

		system("cls");
		std::cout << "\n ";
	}

	else {
		midPoint = innerPoint[PARENT_START];
	}
}


void shadowCast::updateShadows(sf::Vector2f basePos) {

	//	POINT PARENTS:
	//	FIXED->OUTER->INNER | MID
	//	0->6->5 | 4 | 1->2->3

	float h, v, m, b;

	sf::Vector2f outerPoint[PARENT_TOTAL];
	sf::Vector2f innerPoint[PARENT_TOTAL];
	sf::Vector2f midPoint;

	sf::Vector2i outerBorderHitSide[PARENT_TOTAL] = { sf::Vector2i(), sf::Vector2i()};


	for (int i = 0; i < shadowTotal; i++) {

		//---FIND POINTS 6, 2 (OUTER)---//


		for (int n = 0; n < PARENT_TOTAL; n++) {

			findSlope(basePos, parent[i * PARENT_TOTAL + n].fixedPoint, h, v, m, b);

			for (int j = 0; j < AXIS_TOTAL; j++) {
				if (findBorderIntersection(outerPoint[n], outerBorderHitSide[n], h, v, m, b, j)) break;
			}

			/*
			if (n == 0) {

				system("cls");
				std::cout << "\n\n h: " << h << " v: " << v << "\n m: " << m << "\n b: " << b;
				std::cout << "\n\n posA pos: " << basePos.x << " | " << basePos.y;
				std::cout << "\n\n posB pos: " << parent[0].fixedPoint.x << " | " << parent[0].fixedPoint.y;
				std::cout << "\n\n fixed pos: " << outerPoint[0].x << " | " << outerPoint[0].y;
			}
			*/
		}


		//---FIND POINTS 5, 3 (INNER), 4 (MID)---//

		findCorners(outerPoint, innerPoint, midPoint, outerBorderHitSide, i);


		//SET POINTS
		for (int j = 0; j < PARENT_TOTAL; j++) {
			shadow[i].setPoint(OUTER_POINT[j], outerPoint[j]);
			shadow[i].setPoint(INNER_POINT[j], innerPoint[j]);
		}
		shadow[i].setPoint(MID_POINT, midPoint);
	}
}
