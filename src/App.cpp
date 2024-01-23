
#include "App.hpp"


void App::setWindow() {
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "App", sf::Style::None);
	window.setFramerateLimit(60);

	windowPos.x = window.getPosition().x;
	windowPos.y = window.getPosition().y;
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

	//--INITS--//

	setWindow();

	

	shadowObj.initShadowBorder(90, WINDOW_WIDTH - 70, 80, WINDOW_HEIGHT - 100);



	shadowObj.addShadow(sf::Vector2f(120, 130), sf::Vector2f(350, 500));

	shadowObj.addShadow(sf::Vector2f(500, 540), sf::Vector2f(590, 620));

	shadowObj.addShadow(sf::Vector2f(200, 300), sf::Vector2f(450, 560));

	shadowObj.removeShadow(0);


	//--WINDOW LOOP--//

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

	
	window.draw(shadowObj.borderShape);

	for (int i = 0; i < shadowObj.shadow.size(); i++) {
		window.draw(shadowObj.shadow[i]);
	}
	

	window.display();
}


void App::getMousePos() {

	//FIND MOUSE POS
	mousePos.x = sf::Mouse::getPosition().x - windowPos.x;
	mousePos.y = sf::Mouse::getPosition().y - windowPos.y;
}

