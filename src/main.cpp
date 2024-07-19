
#include <SFML/Graphics.hpp>

#include "shadowCast.hpp"



int main() { 


	//--INITS--//

	sf::Vector2u windowSize(800, 800);

	sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Shadow Cast", sf::Style::None);
	window.setFramerateLimit(60);



	const float borderGap = 50;

	ShadowCast shadowCast(sf::Vector2f(borderGap, borderGap), sf::Vector2f(windowSize.x - borderGap * 2, windowSize.y - borderGap * 2));


	shadowCast.add(sf::Vector2f(500, 540), sf::Vector2f(590, 620));
	shadowCast.add(sf::Vector2f(200, 200), sf::Vector2f(250, 300));
	shadowCast.add(sf::Vector2f(1, 1), sf::Vector2f(5, 5)); //Next Function removes this shadow

	shadowCast.remove(2);




	//--WINDOW LOOP--//

	while (window.isOpen())
	{
		//Events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();


			else if (event.type == sf::Event::KeyPressed) {

				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}
		}





		shadowCast.update(sf::Vector2f(sf::Mouse::getPosition(window)));






		window.clear();


		shadowCast.draw(window);


		window.display();
	}
}