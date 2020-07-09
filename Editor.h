#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <list>
#include <functional>

class Actor : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
	}
public:
	sf::Sprite sprite;
	enum Type 
	{
		None,
		Player,
		Enemy,
		Coin,
		Spike,
		Exit
	};
	Type type = None;
	sf::Texture blankTexture;
	sf::Texture playerTexture;
	sf::Texture enemyTexture;
	sf::Texture coinTexture;
	sf::Texture spikeTexture;
	sf::Texture doorTexture;
	Actor() {}

	void loadTextures()
	{
		if (!blankTexture.loadFromFile("SpritesPlatformer/None.png"))
		{
			std::cout << "Failed to load None.png";
		}
		if (!playerTexture.loadFromFile("SpritesPlatformer/Player.png"))
		{
			std::cout << "Failed to load Player.png";
		}
		if (!enemyTexture.loadFromFile("SpritesPlatformer/EnemyAlive.png"))
		{
			std::cout << "Failed to load EnemyAlive.png";
		}
		if (!coinTexture.loadFromFile("SpritesPlatformer/Coin.png"))
		{
			std::cout << "Failed to load Coin.png";
		}
		if (!spikeTexture.loadFromFile("SpritesPlatformer/Spike.png"))
		{
			std::cout << "Failed to load Spike.png";
		}
		if (!doorTexture.loadFromFile("SpritesPlatformer/Door.png"))
		{
			std::cout << "Failed to load Door.png";
		}
	}

	void Init(int x, int y)
	{
		loadTextures();
		sprite.setPosition(x, y);
		RefreshActor();
	}

	void ChangeActor(Type t)
	{
		type = t;
		RefreshActor();
	}

	void RefreshActor()
	{
		switch (type)
		{
		case None:
			sprite.setTexture(blankTexture);
			break;
		case Player:
			sprite.setTexture(playerTexture);
			break;
		case Enemy:
			sprite.setTexture(enemyTexture);
			break;
		case Coin:
			sprite.setTexture(coinTexture);
			break;
		case Spike:
			sprite.setTexture(spikeTexture);
			break;
		case Exit:
			sprite.setTexture(doorTexture);
			break;
		}
	}
};

class Tile : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
		target.draw(actor, states);
	}
public:
	sf::Sprite sprite;
	Actor actor;
	enum Type
	{
		Sky,
		Platform,
		Lava

	};
	Type type = Sky;
	sf::Texture skyTexture;
	sf::Texture platformTexture;
	sf::Texture lavaTexture;

	void loadTextures()
	{
		if (!skyTexture.loadFromFile("SpritesPlatformer/BlockSky.png"))
		{
			std::cout << "Failed to load BlockSky.png";
		}
		if (!platformTexture.loadFromFile("SpritesPlatformer/BlockPlatform.png"))
		{
			std::cout << "Failed to load BlockPlatform.png";
		}
		if (!lavaTexture.loadFromFile("SpritesPlatformer/BlockLava.png"))
		{
			std::cout << "Failed to load BlockLava.png";
		}
	}

	void init(int x, int y)
	{
		loadTextures();
		sprite.setPosition(x, y);
		RefreshTile();
		actor.init(x, y);
	}

	void ChangeTile(Type t)
	{
		type = t;
		RefreshTile();
	}

	//instead of typing tile.actor.ChangeActor(t) every time
	//use tile.ChangeActor(t) instead
	void ChangeActor(Actor::Type a)
	{
		actor.ChangeActor(a);
	}

	void RefreshTile()
	{
		switch (type)
		{
		case Sky:
			sprite.setTexture(skyTexture);
			break;
		case Platform:
			sprite.setTexture(platformTexture);
			break;
		case Lava:
			sprite.setTexture(lavaTexture);
			break;
		}
	}
};

class Button : public sf::Drawable, public sf::Transformable 
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(rect, states);
		target.draw(text, states);
	}
public:
	int textSize = 16;
	float rectOutline = 1;
	sf::RectangleShape rect;
	sf::Font font;
	sf::Text text;
	sf::FloatRect rBounds;

	Button()
	{
		font.loadFromFile("arial.ttf");
		text.setFont(font);
		text.setCharacterSize(textSize);
		text.setFillColor(sf::Color::Black);
		text.setStyle(sf::Text::Regular);
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
	//check for click, if so, pass a function as an argument to store
	void checkClick(std::function<void()> f, sf::Vector2f windpos)
	{
		if (mouseOver(windpos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			f();
		}
	}
};

class ToolPanel : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(loadButton, states);
		target.draw(saveButton, states);
	}
public:
	Button loadButton;
	Button saveButton;

	ToolPanel()
	{
		loadButton.text.setString("load");
		loadButton.rect.setSize(sf::Vector2f(loadButton.text.getGlobalBounds().width + 6, 
			loadButton.text.getGlobalBounds().height + 6));
		loadButton.rect.setPosition(sf::Vector2f(2, 2));
		loadButton.text.setPosition(loadButton.rect.getPosition().x + (loadButton.rect.getSize().x / 400000), 
			loadButton.rect.getPosition().y);

		saveButton.text.setString("save");
		saveButton.rect.setSize(sf::Vector2f(saveButton.text.getGlobalBounds().width + 6, 
			saveButton.text.getGlobalBounds().height + 6));
		saveButton.rect.setPosition(sf::Vector2f(2, 27));
		saveButton.text.setPosition(saveButton.rect.getPosition().x + (saveButton.rect.getSize().x / 400000),
			saveButton.rect.getPosition().y);
	}
};

class EditorClass 
{
public:
	// grid amounts
	static const int x = 30;
	static const int y = 20;
	//keep track of if we are painting with an actor or a tile
	bool actorNotTile = false;
	//setup window size
	const int windowWidth = 1025;
	const int windowHeight = 650;

	//mouse pos
	sf::Vector2f worldPos;
	sf::View toolsView;
	sf::View levelEditView;
	sf::RenderWindow window;
	//setup variable to track selected brush
	Tile::Type curTileType;
	Actor::Type curActorType;
	//setup space for our tool bar
	ToolPanel tools;
	//setup tiles to select and paint with
	Tile tileButton[9];
	//setup tiles array
	Tile tile[x][y];

	bool Start();
	int Update();

	//saving

	//loading

	//printing out tiles (debug)
	
};