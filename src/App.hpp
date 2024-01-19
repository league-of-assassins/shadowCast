
#include "shadowCast.cpp"


class App {
private:

	float width = 800;
	float height = 800;

	Vector2f windowGap;

	RenderWindow window;

	Event event;

	Vector2f mousePos;


	shadowCast shadowObj;

	bool change = true;




	void setWindow();

	void events();

	void displays();

	void getMousePos();

public:

	App();
};
