
#include "App.hpp"


void App::setWindow() {
	window.create(VideoMode(width, height), "App", Style::None);
	window.setFramerateLimit(60);

	windowGap.x = window.getPosition().x;
	windowGap.y = window.getPosition().y;
}


App::App() {

	setWindow();

	sc.initShadowBorder(window, 50, 50, 50, 50);

	sc.addShadow(Vector2f(120, 130), Vector2f(350, 315));

	sc.addShadow(Vector2f(500, 540), Vector2f(590, 620));

	sc.addShadow(Vector2f(450, 700), Vector2f(520, 500), Color::Cyan);

	sc.removeShadow(1);

	while (window.isOpen())
	{
		events();

		general();

		displays();
	}
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

void App::displays() {

	window.clear(Color::Black);


	sc.drawShadows(window);
	

	window.display();
}

void App::getMousePos() {

	//FIND MOUSE POS
	mousePos.x = Mouse::getPosition().x - windowGap.x;
	mousePos.y = Mouse::getPosition().y - windowGap.y;

}

void App::general() {

	getMousePos();

	sc.updateShadows(mousePos);
}
