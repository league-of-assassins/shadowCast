
//shadowCast.hpp
#ifndef SHADOWCAST_HPP
#define SHADOWCAST_HPP

#include <vector>

#include <SFML/Graphics.hpp>





class ShadowCast {
private:

	enum Axis {
		X = 0,
		Y = 1,
		AXIS_SIZE = 2
	};


	enum BorderExtremum {
		BORDER_MIN = 0,
		BORDER_MAX = 1,
		EXTREMUM_SIZE = 2
	};


	struct BorderSide {
		Axis axis;
		BorderExtremum extremum;

		bool operator==(const BorderSide& other) const {
			return (this->axis == other.axis && this->extremum == other.extremum);
		}
	};


	struct ShadowSide {

		sf::Vector2f fixedPoint;
		sf::Vector2f outerPoint;
		sf::Vector2f innerPoint;

		BorderSide fixedBorderHitSide;
		BorderSide outerBorderHitSide;

		sf::Vector2f fixedBorderHitPoint;
	};


	struct Shadow {

		ShadowSide left;
		ShadowSide right;

		sf::Vector2f midPoint;

		Shadow(const sf::Vector2f startPos, const sf::Vector2f endPos) {

			left.fixedPoint = startPos;
			right.fixedPoint = endPos;
		}
	};

	std::vector<Shadow> shadows;


	const int POINT_TOTAL = 7;



	//Optional to draw
	sf::RectangleShape borderShape;

	float border[AXIS_SIZE][EXTREMUM_SIZE];


	sf::Vector2f oldBasePos;



public:


	ShadowCast(const sf::Vector2f borderPos = sf::Vector2f(50, 50), const sf::Vector2f borderSize = sf::Vector2f(700, 700)) : oldBasePos(sf::Vector2f(99999, 99999)) {

		border[X][BORDER_MIN] = borderPos.x;
		border[X][BORDER_MAX] = borderPos.x + borderSize.x;
		border[Y][BORDER_MIN] = borderPos.y;
		border[Y][BORDER_MAX] = borderPos.y + borderSize.y;


		borderShape.setPosition(borderPos);
		borderShape.setSize(borderSize);
		borderShape.setFillColor(sf::Color::Transparent);
		borderShape.setOutlineThickness(-1);
		borderShape.setOutlineColor(sf::Color::Red);
	}



	void draw(sf::RenderWindow& window) {

		for (auto& shadow : shadows) {

			sf::ConvexShape shape(POINT_TOTAL);


			shape.setPoint(0, shadow.left.fixedPoint);
			shape.setPoint(6, shadow.left.outerPoint);
			shape.setPoint(5, shadow.left.innerPoint);

			shape.setPoint(4, shadow.midPoint);

			shape.setPoint(1, shadow.right.fixedPoint);
			shape.setPoint(2, shadow.right.outerPoint);
			shape.setPoint(3, shadow.right.innerPoint);


			window.draw(shape);
		}

		window.draw(borderShape);
	}



	void add(const sf::Vector2f startPos, const sf::Vector2f endPos) {

		shadows.push_back(Shadow(startPos, endPos));

		Shadow& shadow = shadows[shadows.size() - 1];

		findBorderIntersect(shadow.right.fixedPoint, shadow.left.fixedPoint, shadow.left.fixedBorderHitPoint, shadow.left.fixedBorderHitSide);
		findBorderIntersect(shadow.left.fixedPoint, shadow.right.fixedPoint, shadow.right.fixedBorderHitPoint, shadow.right.fixedBorderHitSide);
	}



	void remove(const int n) {

		shadows.erase(shadows.begin() + n);
	}



	void update(const sf::Vector2f basePos) {

		if (basePos != oldBasePos) {

			oldBasePos = basePos;

			//	POINT PARENT ORDER:
			//	FIXED->OUTER->INNER | MID
			//	0->6->5 | 4 | 1->2->3


			for (auto& shadow : shadows) {

				//---FIND POINTS 6, 2 (OUTER)---//

				findBorderIntersect(basePos, shadow.left.fixedPoint, shadow.left.outerPoint, shadow.left.outerBorderHitSide);
				findBorderIntersect(basePos, shadow.right.fixedPoint, shadow.right.outerPoint, shadow.right.outerBorderHitSide);



				//---FIND POINTS 5, 3 (INNER), 4 (MID)---//

				findCornerPoints(shadow);
			}
		}
	}


private:


	void findBorderIntersect(const sf::Vector2f& pointA, const sf::Vector2f& pointB, sf::Vector2f& hitPoint, BorderSide& hitSide) {

		bool undefined = false;

		Axis axis = X;

		float h, v, m, b;

		h = pointA.x - pointB.x;
		v = pointA.y - pointB.y;


		if (abs(h) < 0.0001) {

			undefined = true;
			axis = Y;

			m = 0;
			b = pointA.x;
		}

		else {

			m = v / h;
			b = pointA.y - pointA.x * m;
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


	void findInnerPoint(Shadow& shadow, ShadowSide& side, bool& midEnabled, int& innerEnabledCount) {

		sf::Vector2f tempPoint = side.outerPoint;

		if (!(shadow.left.outerBorderHitSide == shadow.right.outerBorderHitSide)) {

			if (side.outerBorderHitSide.axis == side.fixedBorderHitSide.axis) {
				if (side.outerBorderHitSide.extremum == side.fixedBorderHitSide.extremum) {

					Axis axis = side.fixedBorderHitSide.axis;

					float p1 = (axis == X) ? tempPoint.y : tempPoint.x;

					float p2 = (axis == X) ? side.fixedBorderHitPoint.y : side.fixedBorderHitPoint.x;

					BorderExtremum extremum = (p1 > p2) ? BORDER_MAX : BORDER_MIN;

					tempPoint = (axis == X) ? sf::Vector2f(tempPoint.x, border[Y][extremum]) : sf::Vector2f(border[X][extremum], tempPoint.y);

					innerEnabledCount++;
				}
			}

			else if (shadow.left.fixedBorderHitSide.axis != shadow.right.fixedBorderHitSide.axis) {
				midEnabled = true;
			}
		}

		side.innerPoint = tempPoint;
	}


	void findCornerPoints(Shadow& shadow) {

		int innerEnabledCount = 0;
		bool midEnabled = false;


		findInnerPoint(shadow, shadow.left, midEnabled, innerEnabledCount);
		findInnerPoint(shadow, shadow.right, midEnabled, innerEnabledCount);


		if (innerEnabledCount == 2 && shadow.left.fixedBorderHitSide.axis != shadow.right.fixedBorderHitSide.axis && shadow.left.innerPoint != shadow.right.innerPoint) {
			midEnabled = true;
		}

		if (midEnabled) {

			shadow.midPoint.x = border[X][!shadow.left.fixedBorderHitSide.extremum];
			shadow.midPoint.y = border[Y][!shadow.right.fixedBorderHitSide.extremum];
		}

		else {
			shadow.midPoint = shadow.left.innerPoint;
		}
	}
};


#endif // SHADOWCAST_HPP