#pragma once

class Game
{
private:
	int winW = 1024; // Window Size
	int winH = 768;
	sf::RenderWindow* window;
	bool endgame;
	sf::Event event;

	void InitVariables();
	void InitWindow();

public:
	//Construct Destruct
	Game();
	~Game();

	//Functions
	const bool isRunning() const;
	void PollEvents();

	void Update();
	void Render();
};