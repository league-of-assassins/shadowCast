
#include "App.hpp"


void App::setWindow() {
	window.create(sf::VideoMode(width, height), "App", sf::Style::None);
	window.setFramerateLimit(60);

	windowGap.x = window.getPosition().x;
	windowGap.y = window.getPosition().y;
}


void App::events() {
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) window.close();

		else if (event.type == sf::Event::KeyPressed) {

			if (event.key.code == sf::Keyboard::Escape) {
				window.close();
			}
		}
	}
}


App::App() {

	setWindow();

	

	shadowObj.initShadowBorder(90, width - 70, height - 100, 80);



	shadowObj.addShadow(sf::Vector2f(120, 130), sf::Vector2f(350, 500));

	shadowObj.addShadow(sf::Vector2f(500, 540), sf::Vector2f(590, 620));

	shadowObj.addShadow(sf::Vector2f(650, 500), sf::Vector2f(450, 700));

	shadowObj.removeShadow(0);



	while (window.isOpen())
	{
		events();

		getMousePos();



		if (change) { shadowObj.changeShadowPos(sf::Vector2f(400, 130), sf::Vector2f(350, 315), 1); change = false; }

		shadowObj.updateShadows(mousePos);



		displays();
	}
}


void App::displays() {

	window.clear(sf::Color::Black);


	shadowObj.drawShadows(window);
	

	window.display();
}


void App::getMousePos() {

	//FIND MOUSE POS
	mousePos.x = sf::Mouse::getPosition().x - windowGap.x;
	mousePos.y = sf::Mouse::getPosition().y - windowGap.y;
}

