
//app.hpp
#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

#include "shadowCast.hpp"


class App {
private:

	sf::RenderWindow window;

	enum WindowSize {
		WINDOW_WIDTH = 800,
		WINDOW_HEIGHT = 800
	};

	sf::Vector2f windowPos;

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


#endif //APP_HPP