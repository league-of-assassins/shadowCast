
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;





class shadowCast {
private:

	enum Points {
		POINT_START = 0,
		POINT_END = 1,
		POINT_TOTAL = 7,
		POINT_FIXED_TOTAL = 2
	};

	enum BorderSide {
		BORDER_MIN = 0,
		BORDER_MAX = 1,
	};

	enum AxisSide {
		AXIS_X = 0,
		AXIS_Y = 1,
		AXIS_TOTAL
	};

public:

	//7 Points
	vector<ConvexShape> shadow;

	//Optional to draw
	RectangleShape borderCube;

private:

	//2 start, end
	vector<Vector2f> fixedPoint;

	//2 start, end
	vector<Vector2f> fixedBorderHitPos;

	//2 start, end
	vector<Vector2i> fixedBorderHitSide;

	//x, y, min, max
	float border[2][2];

	const int FIXED_POINT[POINT_FIXED_TOTAL] = { 0, 1 };
	const int OUTER_POINT[POINT_FIXED_TOTAL] = { 6, 2 };
	const int INNER_POINT[POINT_FIXED_TOTAL] = { 5, 3 };
	const int MID_POINT = 4;


	bool undefined = false;

	int shadowTotal = 0;


public:

	shadowCast();

	~shadowCast() {};

	void initShadowBorder(float topBorder, float rightBorder, float bottomBorder, float leftBorder);

	void drawShadows(RenderWindow& window);

	void addShadow(Vector2f startPoint, Vector2f endPoint, Color shadowColor);

	void removeShadow(int removeNo);

	void changeShadowPos(Vector2f startPoint, Vector2f endPoint, int changeNo);

	void updateShadows(Vector2f basePos);


private:

	void setFixedPoint(Vector2f& startPoint, Vector2f& endPoint, int No);

	void fixedPointErrorCatch(Vector2f& startPoint, Vector2f& endPoint, int No);

	int findSide(float distance);

	void findSlope(Vector2f posA, Vector2f posB, float& h, float& v, float& m, float& b);

	bool findBorderIntersection(Vector2f& posV, Vector2i& borderIntersectSide, float h, float v, float m, float b, int side);

	void findCorners(Vector2f outerPoint[2], Vector2f innerPoint[2], Vector2f& midPoint, vector<Vector2i>& outerBorderHitSide, int m);
};
