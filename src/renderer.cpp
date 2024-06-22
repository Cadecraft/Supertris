#include "renderer.h"

Renderer::Renderer(sf::RenderWindow& window) : window(window) {
	// TODO: update defaults if needed
}

void Renderer::renderGame() {
	// TODO: impl
	window.clear(sf::Color(22, 25, 40));

	sf::RectangleShape toRender;
	toRender.setSize(sf::Vector2f(5, 10));
	toRender.setPosition(sf::Vector2f(0, 0));
	toRender.setFillColor(sf::Color(255, 255, 0));
	window.draw(toRender);

	window.display();
}
