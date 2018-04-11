#include "Menu.hpp"
#include <iostream>
#include <string>

constexpr int startButtonWidth = 126;
constexpr int startButtonHeight = 126;

constexpr int sizeButtonWidth = 64;
constexpr int sizeButtonHeight = 29;

constexpr int menuWindowWidth = 800;
constexpr int menuWindowHeight = 600;

Menu::Menu(sf::Font &fnt)
:menuFont(fnt), settings({{10,10}, Difficulty::Easy})
{
	std::cout << "Loading Menu fonts . . .\n";

	std::string tpath(texturesPath);
	tpath+="startButton.png";
	startButtonTex.loadFromFile(tpath.c_str());
	this->startButton.setRadius(startButtonWidth/2);
	this->positionStartButton();
	this->startButton.setTexture(&startButtonTex);

	tpath=texturesPath;
	tpath+="size_10x10_unc.png";
	sizeButtonTextures[0].loadFromFile(tpath.c_str());
	// this button is checked by default so initialized later
	// when needed texture loaded

	this->settings.size.first = 10;
	this->settings.size.second = 10;
	
	tpath=texturesPath;
	tpath+="size_20x15_unc.png";
	sizeButtonTextures[1].loadFromFile(tpath.c_str());
	boardSizeButtons[1].setSize(sf::Vector2f({sizeButtonWidth,sizeButtonHeight}));
	boardSizeButtons[1].setTexture(&sizeButtonTextures[1]);

	tpath=texturesPath;
	tpath+="size_30x20_unc.png";
	sizeButtonTextures[2].loadFromFile(tpath.c_str());
	boardSizeButtons[2].setSize(sf::Vector2f({sizeButtonWidth,sizeButtonHeight}));
	boardSizeButtons[2].setTexture(&sizeButtonTextures[2]);

	tpath=texturesPath;
	tpath+="size_10x10_c.png";
	sizeButtonTextures[3].loadFromFile(tpath.c_str());
	boardSizeButtons[0].setSize(sf::Vector2f({sizeButtonWidth,sizeButtonHeight}));
	boardSizeButtons[0].setTexture(&sizeButtonTextures[3]);

	tpath=texturesPath;
	tpath+="size_20x15_c.png";
	sizeButtonTextures[4].loadFromFile(tpath.c_str());
	
	tpath=texturesPath;
	tpath+="size_30x20_c.png";
	sizeButtonTextures[5].loadFromFile(tpath.c_str());

	this->positionSizeButtons();
	
	/* DIFICULTY TEXTURES */
	tpath=texturesPath;
	tpath+="diff_easy_unc.png";
	diffButtonTextures[0].loadFromFile(tpath.c_str());
	// default diff button setted below

	tpath=texturesPath;
	tpath+="diff_normal_unc.png";
	diffButtonTextures[1].loadFromFile(tpath.c_str());
	difficulties[1].setSize(sf::Vector2f({sizeButtonWidth,sizeButtonHeight}));
	difficulties[1].setTexture(&diffButtonTextures[1]);

	tpath=texturesPath;
	tpath+="diff_hard_unc.png";
	diffButtonTextures[2].loadFromFile(tpath.c_str());
	difficulties[2].setSize(sf::Vector2f({sizeButtonWidth,sizeButtonHeight}));
	difficulties[2].setTexture(&diffButtonTextures[2]);
	

	tpath=texturesPath;
	tpath+="diff_easy_c.png";
	diffButtonTextures[3].loadFromFile(tpath.c_str());
	difficulties[0].setSize(sf::Vector2f({sizeButtonWidth,sizeButtonHeight}));
	difficulties[0].setTexture(&diffButtonTextures[3]);
	this->settings.difficulty = Menu::Difficulty::Easy;


	tpath=texturesPath;
	tpath+="diff_normal_c.png";
	diffButtonTextures[4].loadFromFile(tpath.c_str());

	tpath=texturesPath;
	tpath+="diff_hard_c.png";
	diffButtonTextures[5].loadFromFile(tpath.c_str());

	this->positionDiffButtons();
}

Menu::Settings Menu::open(sf::RenderWindow &window)
{
	resizeWindow(window, 800, 600);
	
	// Start the game loop
	while (window.isOpen())
	{
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window : exit
			if (event.type == sf::Event::Closed)
				window.close();

			if(event.type == sf::Event::MouseButtonPressed)
			{
					if(this->handleMouseEvent(event))
						return this->settings;
			}
		}
		
		window.clear({0x80,0x80,0x80});
		window.draw(this->startButton);
		
		for(auto &t:boardSizeButtons)
		{
			window.draw(t);
		}

		for(auto &t:difficulties)
		{
			window.draw(t);
		}

		window.display();
	}

	return this->settings;
}

inline bool Menu::handleMouseEvent(sf::Event &event)
{
	for(long long unsigned int i=0; i<this->boardSizeButtons.size(); i++)
	{
		if(this->boardSizeButtons[i].getGlobalBounds().contains(
					event.mouseButton.x,event.mouseButton.y))
		{
			this->handleSizeChange(i);
			return false;
		}
		if(this->difficulties[i].getGlobalBounds().contains(
					event.mouseButton.x,event.mouseButton.y))
		{
			this->handleDiffChange(i);
			return false;
		}
		if(this->startButton.getGlobalBounds().contains(
					event.mouseButton.x, event.mouseButton.y))
		{
			return true;
		}
	}
	return false;
}

inline void Menu::positionStartButton()
{
	this->startButton.setPosition(800-startButtonWidth-20,600-startButtonHeight-20);
}

inline void Menu::positionSizeButtons()
{
	int j = 70;
	for(auto &i:boardSizeButtons)
	{
		i.setPosition(j,menuWindowWidth/2);
		j+=120;
	}
}

inline void Menu::handleSizeChange(int idx)
{
	// Reset textures
	int i = 0;

	// ????
	if(idx < 0 || idx >=3)
		return;

	for(auto &t:this->boardSizeButtons)
	{
		t.setTexture(&this->sizeButtonTextures[i]);
		i++;
	}

	this->boardSizeButtons[idx].setTexture(&this->sizeButtonTextures[idx+3]);
	
	// Probably there is better way to to this but 
	// this will be called rarely and while menu opened
	// so its not that much important
	switch(idx)
	{
		case 0:
			this->settings.size.first = 10;
			this->settings.size.second = 10;
			break;
		case 1:
			this->settings.size.first = 20;
			this->settings.size.second = 15;
			break;
		case 2:
			this->settings.size.first = 30;
			this->settings.size.second = 20;
			break;
	}
}

inline void Menu::handleDiffChange(int idx)
{
	// Reset textures
	int i = 0;

	// ????
	if(idx < 0 || idx >=3)
		return;

	for(auto &t:this->difficulties)
	{
		t.setTexture(&this->diffButtonTextures[i]);
		i++;
	}

	this->difficulties[idx].setTexture(&this->diffButtonTextures[idx+3]);
	
	// Probably there is better way to to this but 
	// this will be called rarely and while menu opened
	// so its not that much important
	switch(idx)
	{
		case 0:
			this->settings.difficulty = Difficulty::Easy;
			break;
		case 1:
			this->settings.difficulty = Difficulty::Normal;
			break;
		case 2:
			this->settings.difficulty = Difficulty::Hard;
			break;
	}
}

inline void Menu::positionDiffButtons()
{
	int j = 70;
	for(auto &i:difficulties)
	{
		i.setPosition(j,menuWindowHeight/2);
		j+=120;
	}
}
