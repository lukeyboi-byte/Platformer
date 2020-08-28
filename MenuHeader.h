#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include "WindowHeader.h"

class Mode
{
public:
	enum MenuMode
	{
		MainMenu,
		Editor,
		EditorLevelSelect,
		Game,
		GameLevelSelect,
		GameWin,
		GameOver
	};
	MenuMode currentMode = MainMenu;
};

class EditorLevelSelect{};

class GameLevelSelect{};

class MenuButton : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(rect, states);
		target.draw(text, states);
	}
public:
	int textSize = 30;
	float rectOutline = 1;
	sf::RectangleShape rect;
	sf::Font font;
	sf::Text text;
	sf::FloatRect rBounds;

	//Init
	MenuButton()
	{
		font.loadFromFile("arial.ttf");
		text.setFont(font);
		text.setCharacterSize(textSize);
		text.setFillColor(sf::Color::Black);
		text.setStyle(sf::Text::Regular);
		//setup visuals
		rect.setFillColor(sf::Color::White);
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(rectOutline);
	}

	bool mouseOver(sf::Vector2f windPos)
	{
		rBounds = rect.getGlobalBounds();
		if (rBounds.contains(windPos.x, windPos.y))
		{
			rect.setOutlineThickness(rectOutline * 2);
			return true;
		}
		else
		{
			rect.setOutlineThickness(rectOutline);
			return false;
		}
	}

	void checkClick(std::function<void()> f, sf::Vector2f windPos)
	{
		if(mouseOver(windPos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			f();
		}
	}
};

//added win and lose modes
class GameWin
{
	sf::Vector2f worldPos;

	bool menuActive = true;

};

class GameOver
{
	sf::Vector2f worldPos;

	bool menuActive = true;

};

//tried to implement the win and lose in MainMenu function but doesn't work
class MainMenu
{
public:
	MenuButton levelEditorButton;
	MenuButton gameButton;
	MenuButton returnToMenuButton;
	MenuButton quitButton;
	GameWin gameWin;
	GameOver gameOver;

	Mode myMode;
	MainMenu()
	{
		UpdateButtons();		
	}

	void UpdateButtons()
	{
		switch (MainMenu::myMode.currentMode)
		{
		case Mode::MainMenu:
			levelEditorButton.text.setString("Level Editor");
			levelEditorButton.rect.setSize(sf::Vector2f(levelEditorButton.text.getGlobalBounds().width + 8,
				levelEditorButton.text.getGlobalBounds().height + 8));
			levelEditorButton.rect.setPosition(sf::Vector2f(700 - (700 / 2 - (levelEditorButton.rect.getSize().x / 2)),
				700 - (700 / 2 - (levelEditorButton.rect.getSize().y / 2))));
			levelEditorButton.text.setPosition(levelEditorButton.rect.getPosition().x + (levelEditorButton.rect.getSize().x / 400000),
				levelEditorButton.rect.getPosition().y);

			gameButton.text.setString("Game");
			gameButton.rect.setSize(sf::Vector2f(gameButton.text.getGlobalBounds().width + 8,
				gameButton.text.getGlobalBounds().height + 8));
			gameButton.rect.setPosition(sf::Vector2f(850 - (850 / 2 - (gameButton.rect.getSize().x / 2)),
				725 - (725 / 2 + 50 - (gameButton.rect.getSize().y / 2))));
			gameButton.text.setPosition(gameButton.rect.getPosition().x + (gameButton.rect.getSize().x / 400000),
				gameButton.rect.getPosition().y);
			break;
			//putting the game win and lose states in here to see if it works...
		case Mode::GameOver:
			quitButton.text.setString("Quit");
			quitButton.rect.setSize(sf::Vector2f(quitButton.text.getGlobalBounds().width + 8,
				quitButton.text.getGlobalBounds().height + 8));
			quitButton.rect.setPosition(sf::Vector2f(700 - (700 / 2 - (quitButton.rect.getSize().x / 2)),
				700 - (700 / 2 - (quitButton.rect.getSize().y / 2))));
			quitButton.text.setPosition(quitButton.rect.getPosition().x + (quitButton.rect.getSize().x / 400000),
				quitButton.rect.getPosition().y);

			returnToMenuButton.text.setString("Main Menu");
			returnToMenuButton.rect.setSize(sf::Vector2f(returnToMenuButton.text.getGlobalBounds().width + 8,
				returnToMenuButton.text.getGlobalBounds().height + 8));
			returnToMenuButton.rect.setPosition(sf::Vector2f(850 - (850 / 2 - (returnToMenuButton.rect.getSize().x / 2)),
				725 - (725 / 2 + 50 - (returnToMenuButton.rect.getSize().y / 2))));
			returnToMenuButton.text.setPosition(returnToMenuButton.rect.getPosition().x + (returnToMenuButton.rect.getSize().x / 400000),
				returnToMenuButton.rect.getPosition().y);
			break;
		case Mode::GameWin:
			quitButton.text.setString("Quit");
			quitButton.rect.setSize(sf::Vector2f(quitButton.text.getGlobalBounds().width + 8,
				quitButton.text.getGlobalBounds().height + 8));
			quitButton.rect.setPosition(sf::Vector2f(700 - (700 / 2 - (quitButton.rect.getSize().x / 2)),
				700 - (700 / 2 - (quitButton.rect.getSize().y / 2))));
			quitButton.text.setPosition(quitButton.rect.getPosition().x + (quitButton.rect.getSize().x / 400000),
				quitButton.rect.getPosition().y);

			returnToMenuButton.text.setString("Main Menu");
			returnToMenuButton.rect.setSize(sf::Vector2f(returnToMenuButton.text.getGlobalBounds().width + 8,
				returnToMenuButton.text.getGlobalBounds().height + 8));
			returnToMenuButton.rect.setPosition(sf::Vector2f(850 - (850 / 2 - (returnToMenuButton.rect.getSize().x / 2)),
				725 - (725 / 2 + 50 - (returnToMenuButton.rect.getSize().y / 2))));
			returnToMenuButton.text.setPosition(returnToMenuButton.rect.getPosition().x + (returnToMenuButton.rect.getSize().x / 400000),
				returnToMenuButton.rect.getPosition().y);
			break;
		default:
			break;
		}
	}


	sf::Vector2f worldPos;

	bool Start();
	void Update(MainRenderWindow& mainWindow);

	bool menuActive = true;

	void ChangeMode(Mode::MenuMode m)
	{
		myMode.currentMode = m;
		menuActive = false;
	}

};