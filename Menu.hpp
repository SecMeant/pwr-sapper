#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

#include "tools.hpp"

#include <stdint.h>
#include <utility>
#include <array>

constexpr const char * texturesPath 
	= "./assets/";

class Menu
{
private:
	struct Author
	{
		sf::Text name;
		sf::Text index;
		sf::Text group;
	};

public:
	enum class Difficulty
	{Easy, Normal, Hard};

	struct Settings
	{
		std::pair<int32_t, int32_t> size;
		Difficulty difficulty;
	};


private:
	const sf::Font &menuFont;
	
	sf::RectangleShape gameName;
	std::array<sf::RectangleShape,3> boardSizeButtons;
	std::array<sf::RectangleShape,3> difficulties;

	/* TEXTURES */
	sf::Texture startButtonTex;

	std::array<sf::Texture,6> sizeButtonTextures;
	std::array<sf::Texture,6> diffButtonTextures;
	
	sf::CircleShape startButton;

	Settings settings;

	Author author;

public:
	Menu(sf::Font &fnt);

	Settings open(sf::RenderWindow &wnd);

	/* Render Helpers */
	inline void positionStartButton();
	inline void positionSizeButtons();
	inline void positionDiffButtons();

	/* Handling mouse events */
	// idx are indexes of buttons in its array
	// I know it might be done a bit more explicit with enum or smth
	// but its late and i dont really want to spend on this too much time ;]
	
	// Returns true if user clicked startgame
	// false otherwise
	inline bool handleMouseEvent(sf::Event &event);

	inline void handleSizeChange(int idx);
	inline void handleDiffChange(int idx);
	inline void startButtonClicked();
	inline void sizeButtonClicked(int idx);
	inline void diffButtonClicked(int idx);
};

#endif // MENU_H
