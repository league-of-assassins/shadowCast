
#include "shadowCast.cpp"


class App {
private:

	float width = 800;
	float height = 800;

	Vector2f windowGap;

	RenderWindow window;

	Event event;

	Vector2f mousePos;


	shadowCast sc;




	void setWindow();

	void events();

	void displays();

	void getMousePos();

	void general();

public:

	App();
};
