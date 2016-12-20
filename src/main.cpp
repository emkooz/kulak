#include <iostream>
#include <SFML/Graphics.hpp>
#include "cvar.hpp"
#include "log.hpp"
#include "resource.hpp"


int main()
{
	sf::RenderWindow window(sf::VideoMode(800,600), "kulak");

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color (100,149,237,255));
		window.display();
	}

	return 0;
}