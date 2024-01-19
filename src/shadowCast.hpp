#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;


class shadowCast {
public:

	vector<ConvexShape> shadow;

	RectangleShape borderCube;

private:

	//7 points
	vector<Vector2f> shadowPos;

	//2 start, end
	vector<Vector2f> fixedBorderPos;

	//2 start, end
	vector<Vector2i> fixedBorderSide;

	//x, y, min, max
	float border[2][2];


	const float radian = 3.14 / 180;

	bool undefined = false;

	int shadowTotal = 0;


public:

	void initShadowBorder(float topBorder, float rightBorder, float bottomBorder, float leftBorder);

	void drawShadows(RenderWindow& window);

	void addShadow(Vector2f startPos, Vector2f endPos, Color shadowColor);

	void removeShadow(int removeNo);

	void changeShadowPos(Vector2f startPos, Vector2f endPos, int changeNo);

	void updateShadows(Vector2f basePos);

	shadowCast();

private:

	void setShadowFixedPos(Vector2f& startPos, Vector2f& endPos, int No);

	void fixedShadowPosErrorCatch(Vector2f& StartPos, Vector2f& EndPos, int No);

	int findSide(float s);

	void findSlope(Vector2f posA, Vector2f posB, float& h, float& v, float& m, float& b);

	bool findBorderIntersection(Vector2f& posV, Vector2i& borderSide, float h, float v, float m, float b, int side);

	void findCorner(vector<Vector2f>& ShadowPos, vector<Vector2f>& FixedBorderPos, vector<Vector2i>& FixedBorderSide, vector<Vector2i>& WallBorderSide, int m);

	void setPositions();
};