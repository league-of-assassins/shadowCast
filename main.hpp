
#include <SFML/Graphics.hpp>
#include <iostream>


using namespace std;
using namespace sf;


const int wallTotal = 4;
const int pointsTotal = 7;


class shadowCast {
private:

	float width = 800;
	float height = 800;

	Vector2f windowGap;

	RenderWindow window;

	Event event;


	ConvexShape shadow[wallTotal];
	Vector2f shadowPos[wallTotal][pointsTotal];


	RectangleShape wall[wallTotal];
	Vector2f wallPos[wallTotal][2];
	Vector2f wallSize[wallTotal];
	float wallAngle[wallTotal];

	Vector2f wallBorderPos[wallTotal][2];
	int wallBorderSide[wallTotal][2][2] = {};

	//SIZE 2, POS 2, ANGLE 1
	int wallValues[4][5] = {
		{200, 1, 500, 300, 35},
		{200, 1, 200, 600, 45},
		{150, 1, 150, 200, 130},
		{250, 1, 700, 550, 145}
	};


	RectangleShape borderCube;

	float border[2][2] = { {50, width - 50}, {50, height - 50} };


	Vector2f mousePos;

	float radian = 3.14 / 180;

	bool undefined = false;


public:

	void setWindow();

	void objects();

	void events();

	void mouseLogic();

	int findSide(float s);

	void findSlope(Vector2f posA, Vector2f posB, float& h, float& v, float& m, float& b);

	bool findBorderIntersection(Vector2f& posV, float h, float v, float m, float b, int side, int sideArray[2]);

	void findCorner(Vector2f pointPos[pointsTotal], Vector2f wallBorderPos[2], int pointSide[2][2], int wallSide[2][2]);

	void shadowMain();

	void setPositions();

	void general();

	void displays();

	shadowCast();
};
