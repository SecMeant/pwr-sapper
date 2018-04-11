#include "tools.hpp"

void resizeWindow(sf::RenderWindow &wnd, int w, int h)
{
	sf::Vector2u newSize({static_cast<unsigned int>(w), static_cast<unsigned int>(h)});
	sf::FloatRect viewParams(0.f, 0.f, w, h);
	sf::View newView(viewParams);

	wnd.setSize(newSize);
	wnd.setView(newView);
}
