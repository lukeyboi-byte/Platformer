#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING //VS2019
#include "Editor.h"
#include "MenuHeader.h"
#include "WindowHeader.h"
#include "Game.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

std::vector<std::string> saves;
int curLevel = 0;

void GetAllSaveFiles()
{
	std::string path = "\.";
	std::string ext = ".sav";
	saves.clear();
	for (auto& p : fs::recursive_directory_iterator(path))
	{
		if (p.path().extension() == ext)
		{
			std::cout << p.path().filename().generic_string() << '\n';
			std::string file = p.path().filename().generic_string();
			saves.push_back(file);
		}
	}
}

int main()
{
	EditorClass myEditor;
	MainMenu myMainMenu;
	GameClass myGame;
	MainRenderWindow mainWindow;

	mainWindow.window.create(sf::VideoMode(mainWindow.windowWidth, mainWindow.windowHeight), "My Program", sf::Style::Titlebar | sf::Style::Close);

	while (mainWindow.window.isOpen())
	{
		sf::Event event;
		while (mainWindow.window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				mainWindow.close();
				break;
			}
		}
		switch (myMainMenu.myMode.currentMode)
		{
		case Mode::MainMenu:
			if (!myMainMenu.Start())
			{
				return EXIT_FAILURE;
			}
			myMainMenu.Update(mainWindow);
			break;
		case Mode::Editor:
			if (!myEditor.editorActive)
			{
				if (!myEditor.Start(mainWindow))
				{
					return EXIT_FAILURE;
				}
			}			
			myEditor.Update(mainWindow);
			break;
		case Mode::Game:
			if (!myGame.gameActive)
			{
				if (!myGame.Start(mainWindow))
				{
					return EXIT_FAILURE;
				}
			}
			myGame.Update(mainWindow);
			break;
		default:
			break;
		}
	}

	return 0;

}

bool MainMenu::Start()
{
	return true;
}

void MainMenu::Update(MainRenderWindow& mainWindow)
{
	while (menuActive)
	{
		mainWindow.worldPos = mainWindow.window.mapPixelToCoords(sf::Mouse::getPosition(mainWindow.window));
		levelEditorButton.checkClick(std::bind(&MainMenu::ChangeMode, this, Mode::Editor), mainWindow.worldPos);
		mainWindow.clear();
		mainWindow.window.draw(levelEditorButton);
		mainWindow.window.display();
	}
	return;
}

EditorClass::EditorClass()
{
	for (int i = 0; i < x; i++)
	{
		tile[i] = new Tile[y];
	}
}

bool EditorClass::Start(MainRenderWindow &mainWindow)
{
	//setup our views
	toolsView = sf::View(sf::FloatRect(0, 0, mainWindow.windowWidth * 0.045f, mainWindow.windowHeight));
	toolsView.setViewport(sf::FloatRect(0, 0, 0.045f, 1));
	levelEditView = sf::View(sf::FloatRect(0, 0, mainWindow.windowWidth, mainWindow.windowHeight));
	levelEditView.setViewport(sf::FloatRect(0.03f, 0, 1, 1));

	//setup variables to apoint with
	curTileType = Tile::Type::Platform;
	curActorType = Actor::Type::Coin;

	for (int i = 0; i < 9; i++)
	{
		tileButton[i].init(10, (32 + 5) * i + 150);
	}
	//setting each button to have a tile/actor type
	tileButton[0].ChangeTile(Tile::Type::Sky);
	tileButton[1].ChangeTile(Tile::Type::Platform);
	tileButton[2].ChangeTile(Tile::Type::Lava);
	tileButton[3].ChangeActor(Actor::Type::None);
	tileButton[4].ChangeActor(Actor::Type::Player);
	tileButton[5].ChangeActor(Actor::Type::Enemy);
	tileButton[6].ChangeActor(Actor::Type::Coin);
	tileButton[7].ChangeActor(Actor::Type::Spike);
	tileButton[8].ChangeActor(Actor::Type::Exit);

	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			tile[i][j].init(i * 32 + ((mainWindow.windowWidth / 2) - ((32 * x) / 2)), j * 32);
		}
	}
	editorActive = true;
	return true;
}

void EditorClass::Update(MainRenderWindow &mainWindow)
{	
	//prep window for displaying stuff
	mainWindow.window.clear(sf::Color::White);
	mainWindow.window.setView(toolsView);
	//track mouse pos
	worldPos = mainWindow.window.mapPixelToCoords(sf::Mouse::getPosition(mainWindow.window));

	//bind our save/load button to save/load functions
	tools.saveButton.CheckClick(std::bind(&EditorClass::save, this, playerInput, tile),worldPos);
	tools.loadButton.CheckClick(std::bind(&EditorClass::load, this, playerInput, tile), worldPos);

	//loop through our toolbar to check for clicks
	for (int i = 0; i < 9; i++)
	{
		if (tileButton[i].mouseOver(worldPos))
		{
			tileButton[i].sprite.setScale(sf::Vector2f(1.1f, 1.1f));
			tileButton[i].actor.sprite.setScale(sf::Vector2f(1.1f, 1.1f));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (i < 3)
				{
					actorNotTile = false;
					curTileType = tileButton[i].type;
				}
				else //3 and above are actors
				{
					actorNotTile = true;
					curActorType = tileButton[i].actor.type;
				}
			}
		}
		else
		{
			tileButton[i].sprite.setScale(sf::Vector2f(1.f, 1.f));
			tileButton[i].actor.sprite.setScale(sf::Vector2f(1.f, 1.f));
		}
		mainWindow.window.draw(tileButton[i]);
	}

	mainWindow.window.draw(tools);
	mainWindow.window.setView(levelEditView);
	worldPos = mainWindow.window.mapPixelToCoords(sf::Mouse::getPosition(mainWindow.window), mainWindow.window.getView());

	//draw our tiles
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			//check for click
			if (tile[i][j].mouseOver(worldPos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (!actorNotTile)
				{
					tile[i][j].ChangeTile(curTileType);
				}
				else
				{
					tile[i][j].ChangeActor(curActorType);
				}
			}
			tile[i][j].RefreshTile();
			mainWindow.window.draw(tile[i][j]);
		}
	}

	sf::Event event;
	while (mainWindow.window.pollEvent(event))
	{
		if (event.type == sf::Event::TextEntered)
		{
			if (event.text.unicode == 8 && playerInput.size() != 0)
			{
				playerInput.pop_back();
				std::cout << playerInput << std::endl;
			}
			else if (event.text.unicode < 128)
			{
				playerInput += static_cast<char>(event.text.unicode);
				std::cout << playerInput << std::endl;
			}
			inputField.text.setString(playerInput);
		}
	}
	mainWindow.window.draw(inputField);
	mainWindow.window.display();
}

bool GameClass::Start(MainRenderWindow& mainWindow)
{
	GetAllSaveFiles();
	//setup of game, init tiles
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			tile[i][j].init(i * 32 + ((mainWindow.windowWidth / 2) - ((32 * x) / 2)), j * 32);
		}
	}

	//load save files
	if (saves.size() != 0)
	{
		LoadLevel(saves[curLevel], tile);
	}
	else
	{
		std::cout << "Error, no saved levels!" << std::endl;
	}

	player.nextPos = player.getPosition();
	gameActive = true;
	return true;
}

void GameClass::Update(MainRenderWindow& mainWindow)
{
	mainWindow.window.clear(sf::Color::White);
	deltaTime = clock.restart().asSeconds();
	//Controls!
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		//Move Right
		if (player.isGrounded)
		{
			player.velocity.x += player.speed * deltaTime;
			player.velocity.y += deltaTime;
		}
		else
		{
			player.velocity.x += player.speed / 3 * deltaTime;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		//Move left
		if (player.isGrounded)
		{
			player.velocity.x -= player.speed * deltaTime;
			player.velocity.y += deltaTime;
		}
		else
		{
			player.velocity.x -= player.speed / 3 * deltaTime;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (player.isGrounded)
		{
			player.isGrounded = false;
			player.velocity.y += -player.jumpSpeed + deltaTime;
		}
	}

	//friction
	if (player.isGrounded)
	{
		if (!player.isAnyKeyPressed())
		{
			player.velocity.x -= friction * deltaTime * (sign(player.velocity.x));
		}
	}

	//maximum abs velocity
	if (abs(player.velocity.x) > 0.06f)
	{
		player.velocity.x = 0.06f * sign(player.velocity.x);
	}

	//max vertical velocity
	if (player.velocity.y < 1.0f)
	{
		player.velocity.y += gravity * deltaTime;
	}
	else if (player.velocity.y < -1.0f)
	{
		player.velocity.y = -1.0f;
	}

	//this is the players next potential position, if they aren't obstructed
	player.nextPos = player.getPosition() + player.velocity;
	//project a hit box
	player.nextRect = sf::FloatRect(player.nextPos, sf::Vector2f(32.f, 32.f));
	player.isGrounded = false;
	//loop for collision with tiles
	//for(int....

	//set player position
	player.setPosition(player.nextPos);
	//draw
	mainWindow.window.draw(player);
	mainWindow.window.display();
}