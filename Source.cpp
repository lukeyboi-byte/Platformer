#include "Editor.h"

int main()
{
	EditorClass myEditor;
	if (!myEditor.Start())
	{
		return EXIT_FAILURE;
	}
	return myEditor.Update();
	return 0;

}

bool EditorClass::Start()
{
	//setup our views
	toolsView = sf::View(sf::FloatRect(0, 0, windowWidth * 0.045f, windowHeight));
	toolsView.setViewport(sf::FloatRect(0, 0, 0.045f, 1));
	levelEditView = sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight));
	levelEditView.setViewport(sf::FloatRect(0, 0, 1, 1));
	//setup the window!
	window.create(sf::VideoMode(windowWidth, windowHeight), "Level Editor", sf::Style::Titlebar | sf::Style::Close);
	return true;
}

int EditorClass::Update()
{
	//Editorloop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		//prep window for displaying stuff
		window.clear(sf::Color::White);
		window.setView(toolsView);
		//track mouse pos
		worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		window.draw(tools);
		window.setView(levelEditView);
		window.display();
	}
	
	return 0;
}