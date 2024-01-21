
#include "shadowCast.cpp"


class App {
private:

	float width = 800;
	float height = 800;

	sf::Vector2f windowGap;

	sf::RenderWindow window;

	sf::Event event;

	sf::Vector2f mousePos;


	shadowCast shadowObj;

	bool change = true;




	void setWindow();

	void events();

	void displays();

	void getMousePos();

public:

	App();
};
