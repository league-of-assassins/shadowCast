
#include <SFML/Graphics.hpp>
#include <iostream>




class shadowCast {

public:

	//7 Points
	std::vector<sf::ConvexShape> shadow;

	//Optional to draw
	sf::RectangleShape borderCube;



private:

	enum Parents {
		PARENT_START = 0,
		PARENT_END = 1,
		PARENT_TOTAL = 2
	};

	const int POINT_TOTAL = 7;

	const int FIXED_POINT[PARENT_TOTAL] = { 0, 1 };
	const int OUTER_POINT[PARENT_TOTAL] = { 6, 2 };
	const int INNER_POINT[PARENT_TOTAL] = { 5, 3 };
	const int MID_POINT = 4;

	struct ParentPoint {
		sf::Vector2f fixedPoint;
		sf::Vector2f fixedBorderHitPos;
		sf::Vector2i fixedBorderHitSide;
	};

	//2 parents per shadow: start, end
	std::vector<ParentPoint> parent;



	enum AxisSide {
		AXIS_X = 0,
		AXIS_Y = 1,
		AXIS_TOTAL = 2
	};

	enum BorderExtremum {
		BORDER_MIN = 0,
		BORDER_MAX = 1,
		BORDER_EXTREMUM_TOTAL = 2
	};

	float border[AXIS_TOTAL][BORDER_EXTREMUM_TOTAL];



	bool undefined = false;

	int shadowTotal = 0;



public:

	shadowCast();

	~shadowCast() {};

	void initShadowBorder(float topBorder, float rightBorder, float bottomBorder, float leftBorder);

	void drawShadows(sf::RenderWindow& window);

	void addShadow(sf::Vector2f startPoint, sf::Vector2f endPoint, sf::Color shadowColor);

	void removeShadow(int removeNo);

	void changeShadowPos(sf::Vector2f startPoint, sf::Vector2f endPoint, int changeNo);

	void updateShadows(sf::Vector2f basePos);


private:

	void setFixedPoint(sf::Vector2f& startPoint, sf::Vector2f& endPoint, int No);

	void fixedPointErrorCatch(sf::Vector2f& startPoint, sf::Vector2f& endPoint, int No);

	int findSide(float distance);

	void findSlope(sf::Vector2f posA, sf::Vector2f posB, float& h, float& v, float& m, float& b);

	bool findBorderIntersection(sf::Vector2f& posV, sf::Vector2i& borderIntersectSide, float h, float v, float m, float b, int side);

	void findCorners(sf::Vector2f outerPoint[2], sf::Vector2f innerPoint[2], sf::Vector2f& midPoint, sf::Vector2i outerBorderHitSide[2], int m);
};
