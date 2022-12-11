#include "game.h"

Game::Game()
{
	InitVariables();
	InitWindow();
}

Game::~Game()
{
	delete this->window;
}

void Game::InitVariables()
{
	endgame = false;
}

void Game::InitWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(1024, 768), "RaZPoNG", sf::Style::Close | sf::Style::Titlebar);
}

const bool Game::isRunning() const
{
	return this->window->isOpen();
}

void Game::PollEvents()
{
	while (this->window->pollEvent(this->event))
	{
		switch (this->event.type)
		{
			case sf::Event::Closed:
				this->window->close();
				break;
			case sf::Event::KeyPressed:
				if (this->event.key.code == sf::Keyboard::Escape)
					this->window->close();
				break;
			default:
				break;
		}
	}
}

void Game::Update()
{
	PollEvents();
}

void Game::Render()
{
	this->window->clear();
	// DRAW

	this->window->display();
}