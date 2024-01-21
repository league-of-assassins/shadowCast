
#include "App.hpp"


void App::setWindow() {
	window.create(VideoMode(width, height), "App", Style::None);
	window.setFramerateLimit(60);

	windowGap.x = window.getPosition().x;
	windowGap.y = window.getPosition().y;
}


void App::events() {
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed) window.close();

		else if (event.type == Event::KeyPressed) {

			if (event.key.code == Keyboard::Escape) {
				window.close();
			}
		}
	}
}


App::App() {

	setWindow();

	

	shadowObj.initShadowBorder(50, width - 50, height - 50, 50);



	shadowObj.addShadow(Vector2f(120, 130), Vector2f(350, 500));

	shadowObj.addShadow(Vector2f(500, 540), Vector2f(590, 620));

	shadowObj.addShadow(Vector2f(450, 700), Vector2f(650, 500));

	shadowObj.removeShadow(1);



	while (window.isOpen())
	{
		events();

		getMousePos();



		if (change) { shadowObj.changeShadowPos(Vector2f(400, 130), Vector2f(350, 315), 0); change = false; }

		shadowObj.updateShadows(mousePos);



		displays();
	}
}


void App::displays() {

	window.clear(Color::Black);


	shadowObj.drawShadows(window);
	

	window.display();
}


void App::getMousePos() {

	//FIND MOUSE POS
	mousePos.x = Mouse::getPosition().x - windowGap.x;
	mousePos.y = Mouse::getPosition().y - windowGap.y;
}

