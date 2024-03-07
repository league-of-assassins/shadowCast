
#include "shadowCast.hpp"


shadowCast::shadowCast() {

	initShadowBorder(0, 800, 0, 800);
	borderShape.setOutlineColor(sf::Color::Red);
	borderShape.setOutlineThickness(-1);
	borderShape.setFillColor(sf::Color::Black);

	oldBasePos.x = 99999;
	oldBasePos.y = 99999;
}


void shadowCast::initShadowBorder(float leftBorder, float rightBorder, float topBorder, float bottomBorder) {


	//SET
	border[X][BORDER_MIN] = leftBorder;
	border[X][BORDER_MAX] = rightBorder;
	border[Y][BORDER_MIN] = topBorder;
	border[Y][BORDER_MAX] = bottomBorder;

	borderInputErrorCatch();

	borderShape.setPosition(sf::Vector2f(border[X][BORDER_MIN], border[Y][BORDER_MIN]));
	borderShape.setSize(sf::Vector2f(border[X][BORDER_MAX] - border[X][BORDER_MIN], border[Y][BORDER_MAX] - border[Y][BORDER_MIN]));
}


void shadowCast::borderInputErrorCatch() {

	std::string axisNames[AXIS_SIZE] = { "X", "Y" };

	for (int axis = 0; axis < AXIS_SIZE; axis++) {
		if (border[axis][BORDER_MIN] >= border[axis][BORDER_MAX]) {
			border[axis][BORDER_MIN] = 0; border[axis][BORDER_MAX] = 800;
			std::cerr << "\n\n shadowCast.cpp: " << axisNames[axis] << " axis: MIN cant be greater than MAX.Reverted to default";
		}
	}
}


void shadowCast::addShadow(sf::Vector2f startPoint, sf::Vector2f endPoint) {

	split.push_back({});

	shadow.push_back(sf::ConvexShape(POINT_TOTAL));

	setFixedPoint(startPoint, endPoint, shadowTotal);

	shadowTotal++;
}


void shadowCast::removeShadow(const int removeNo) {

	if (!NumberInputErrorCatch(removeNo, "remove")) {

		split.erase(split.begin() + removeNo);

		shadow.erase(shadow.begin() + removeNo);

		shadowTotal--;
	}
}


void shadowCast::changeShadowPos(sf::Vector2f startPoint, sf::Vector2f endPoint, const int changeNo) {

	if (!NumberInputErrorCatch(changeNo, "change")) {
		setFixedPoint(startPoint, endPoint, changeNo);
	}
}


const bool shadowCast::NumberInputErrorCatch(const int No, const std::string NoName) {

	if (shadowTotal == 0) {
		std::cerr << "\n\n shadowCast.cpp: " << NoName << "No: " << No << " is out of scope. shadow total is 0.\n";
		return true;
	}

	if (No >= shadowTotal || No < 0) {

		std::cerr << "\n\n shadowCast.cpp: " << NoName << "No: " << No << " is out of scope. Range is 0 to " << shadowTotal - 1 << ".\n";
		return true;
	}

	return false;
}


void shadowCast::setFixedPoint(sf::Vector2f& startPoint, sf::Vector2f& endPoint, const int No) {

	pointInputErrorCatch(startPoint, endPoint, No);

	shadow[No].setPoint(FIXED_POINT[START], startPoint);
	shadow[No].setPoint(FIXED_POINT[END], endPoint);

	split[No][START].fixedPoint = startPoint;
	split[No][END].fixedPoint = endPoint;


	//FIND FIXED BORDER HIT POS AND SIDE
	for (int i = 0; i < SPLIT_SIZE; i++) {

		findBorderIntersection(split[No][!i].fixedPoint, split[No][i].fixedPoint, split[No][i].fixedBorderHitPoint, split[No][i].fixedBorderHitSide);
	}
}


void shadowCast::pointInputErrorCatch(sf::Vector2f& startPoint, sf::Vector2f& endPoint, int No) {

	float pos[SPLIT_SIZE][AXIS_SIZE] = {
		{ startPoint.x, startPoint.y },
		{ endPoint.x, endPoint.y }
	};


	std::string whichPos[SPLIT_SIZE] = { "start", "end" };
	std::string axis[AXIS_SIZE][EXTREMUM_SIZE] = { {"x", "y"}, { "lower", "greater" } };

	for (int n = 0; n < SPLIT_SIZE; n++) {
		for (int i = 0; i < AXIS_SIZE; i++) {
			if (pos[n][i] <= border[i][BORDER_MIN]) {
				pos[n][i] = border[i][BORDER_MIN] + 1;
				std::cerr << "\n\n Warning: shadow[" << No << "] " << whichPos[n] << "Pos." << axis[0][i] << " is " << axis[1][BORDER_MIN] << " than border. It was fixed to border.\n";
			}

			if (pos[n][i] >= border[i][BORDER_MAX]) {
				pos[n][i] = border[i][BORDER_MAX] - 1;
				std::cerr << "\n\n Warning: shadow[" << No << "] " << whichPos[n] << "Pos." << axis[0][i] << " is " << axis[1][BORDER_MAX] << " than border. It was fixed to border.\n";
			}
		}
	}

	startPoint.x = pos[START][X];
	startPoint.y = pos[START][Y];

	endPoint.x = pos[END][X];
	endPoint.y = pos[END][Y];

	if (startPoint == endPoint) {
		std::cerr << "\n\n Warning: shadow[" << No << "] startPoint and endPoint are equal.\n";
	}
}




void shadowCast::findBorderIntersection(const sf::Vector2f& pointA, const sf::Vector2f& pointB, sf::Vector2f& hitPoint, BorderSide& hitSide) {

	float h, v, m, b;

	bool undefined = false;

	int axis = X;

	h = pointA.x - pointB.x;
	v = pointA.y - pointB.y;

	m = v / h;
	b = pointA.y - pointA.x * m;

	if (abs(h) < 0.0001) {
		undefined = true;

		m = 0;
		b = pointA.x;
		axis = Y;
	}

	//CHECK BOTH AXIS

	if (axis == X) {

		hitSide.axis = X;
		hitSide.extremum = (h < 0) ? BORDER_MAX : BORDER_MIN;

		hitPoint.x = border[hitSide.axis][hitSide.extremum];
		hitPoint.y = hitPoint.x * m + b;

		//IF NOT WITHIN LIMITS CHECK OTHER AXIS
		if (hitPoint.y < border[Y][BORDER_MIN] || hitPoint.y > border[Y][BORDER_MAX]) {
			axis = Y;
		}
	}

	if (axis == Y) {

		hitSide.axis = Y;
		hitSide.extremum = (v < 0) ? BORDER_MAX : BORDER_MIN;

		hitPoint.y = border[hitSide.axis][hitSide.extremum];
		hitPoint.x = (undefined) ? b : (hitPoint.y - b) / m;
	}
}


void shadowCast::findCorners(const sf::Vector2f outerPoint[SPLIT_SIZE], sf::Vector2f innerPoint[SPLIT_SIZE], sf::Vector2f& midPoint, const BorderSide outerBorderSide[SPLIT_SIZE], int No) {

	float outerPointArr[SPLIT_SIZE][AXIS_SIZE] = {
		{outerPoint[START].x, outerPoint[START].y},
		{outerPoint[END].x, outerPoint[END].y}
	};

	float fixedBorderHitPoint[SPLIT_SIZE][AXIS_SIZE] = {
		{split[No][START].fixedBorderHitPoint.x, split[No][START].fixedBorderHitPoint.y},
		{split[No][END].fixedBorderHitPoint.x, split[No][END].fixedBorderHitPoint.y},
	};

	int axis, extremum, innerEnabledCount = 0;

	bool midEnabled = false;


	for (int i = 0; i < SPLIT_SIZE; i++) {

		if (!(outerBorderSide[START].axis == outerBorderSide[END].axis &&
			outerBorderSide[START].extremum == outerBorderSide[END].extremum)) {

			axis = split[No][i].fixedBorderHitSide.axis;

			if (outerBorderSide[i].axis == split[No][i].fixedBorderHitSide.axis) {
				if (outerBorderSide[i].extremum == split[No][i].fixedBorderHitSide.extremum) {

					extremum = (outerPointArr[i][!axis] > fixedBorderHitPoint[i][!axis]) ? BORDER_MAX : BORDER_MIN;

					outerPointArr[i][!axis] = border[!axis][extremum];

					innerEnabledCount++;
				}
			}

			else if (split[No][START].fixedBorderHitSide.axis != split[No][END].fixedBorderHitSide.axis) {
				midEnabled = true;
			}
		}

		innerPoint[i].x = outerPointArr[i][X];
		innerPoint[i].y = outerPointArr[i][Y];
	}


	if (innerEnabledCount == 2 && split[No][START].fixedBorderHitSide.axis != split[No][END].fixedBorderHitSide.axis && innerPoint[START] != innerPoint[END]) {
		midEnabled = true;
	}

	if (midEnabled) {
		midPoint.x = border[X][!split[No][START].fixedBorderHitSide.extremum];
		midPoint.y = border[Y][!split[No][END].fixedBorderHitSide.extremum];
	}

	else {
		midPoint = innerPoint[START];
	}
}


void shadowCast::updateShadows(const sf::Vector2f& basePos) {

	if (basePos != oldBasePos) {

		oldBasePos = basePos;

		//	POINT PARENT ORDER:
		//	FIXED->OUTER->INNER | MID
		//	0->6->5 | 4 | 1->2->3

		sf::Vector2f outerPoint[SPLIT_SIZE];
		sf::Vector2f innerPoint[SPLIT_SIZE];
		sf::Vector2f midPoint;

		BorderSide outerPointSide[SPLIT_SIZE];


		for (int No = 0; No < shadowTotal; No++) {

			//---FIND POINTS 6, 2 (OUTER)---//

			for (int i = 0; i < SPLIT_SIZE; i++) {

				findBorderIntersection(basePos, split[No][i].fixedPoint, outerPoint[i], outerPointSide[i]);
			}


			//---FIND POINTS 5, 3 (INNER), 4 (MID)---//

			findCorners(outerPoint, innerPoint, midPoint, outerPointSide, No);



			//SET POINTS
			for (int i = 0; i < SPLIT_SIZE; i++) {
				shadow[No].setPoint(OUTER_POINT[i], outerPoint[i]);
				shadow[No].setPoint(INNER_POINT[i], innerPoint[i]);
			}
			shadow[No].setPoint(MID_POINT, midPoint);
		}
	}
}