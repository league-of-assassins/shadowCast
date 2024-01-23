
//shadowCast.hpp
#ifndef SHADOWCAST_HPP
#define SHADOWCAST_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <array>




class shadowCast {

public:

	//7 Points
	std::vector<sf::ConvexShape> shadow;

	//Optional to draw
	sf::RectangleShape borderShape;



private:

	enum AxisDefine {
		X = 0,
		Y = 1,
		AXIS_SIZE = 2
	};

	enum BorderExtremumDefine {
		BORDER_MIN = 0,
		BORDER_MAX = 1,
		EXTREMUM_SIZE = 2
	};

	enum SplitDefine {
		START = 0,
		END = 1,
		SPLIT_SIZE = 2
	};


	const int POINT_TOTAL = 7;

	const int FIXED_POINT[SPLIT_SIZE] = { 0, 1 };
	const int OUTER_POINT[SPLIT_SIZE] = { 6, 2 };
	const int INNER_POINT[SPLIT_SIZE] = { 5, 3 };
	const int MID_POINT = 4;


	struct BorderSide {
		int axis;
		int extremum;
	};

	struct FixedPointTraits {
		sf::Vector2f fixedPoint;
		sf::Vector2f fixedBorderHitPoint;
		BorderSide fixedBorderHitSide;
	};


	std::vector<std::array<FixedPointTraits, SPLIT_SIZE>> split;


	float border[AXIS_SIZE][EXTREMUM_SIZE];


	int shadowTotal = 0;



public:

	shadowCast();

	~shadowCast() {};

	void initShadowBorder(float leftBorder, float rightBorder, float topBorder, float bottomBorder);

	void addShadow(sf::Vector2f startPoint, sf::Vector2f endPoint);

	void removeShadow(const int removeNo);

	void changeShadowPos(sf::Vector2f startPoint, sf::Vector2f endPoint, const int changeNo);

	void updateShadows(const sf::Vector2f& basePos);


private:

	void borderInputErrorCatch();

	void pointInputErrorCatch(sf::Vector2f& startPoint, sf::Vector2f& endPoint, int No);

	const bool NumberInputErrorCatch(const int No, const std::string NoName);



	void setFixedPoint(sf::Vector2f& startPoint, sf::Vector2f& endPoint, const int No);

	void findBorderIntersection(const sf::Vector2f& pointA, const sf::Vector2f& pointB, sf::Vector2f& hitPoint, BorderSide& hitSide);

	void findCorners(const sf::Vector2f outerPoint[SPLIT_SIZE], sf::Vector2f innerPoint[SPLIT_SIZE], sf::Vector2f& midPoint, const BorderSide outerBorderSide[SPLIT_SIZE], const int No);
};


#endif // SHADOWCAST_HPP
