#pragma once
#include <sstream>
#include <string>

sf::Font font;
sf::Text headingText, speedText, inphaseText, ampText, fpsText, controlsText;

void InitTextObjects(sf::RenderWindow* window);
void SetSpeedText(float _speed);
void SetFpsText(float _fps);
void SetInphaseText(int _inphase, int _numElements);
void SetAmpText(float _amp);

template <typename T>
//function to convert a non-string variable to a string.
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}


void InitTextObjects(sf::RenderWindow* window)
{
	//Set Styles
	headingText.setFont(font);
	headingText.setFillColor(sf::Color::White);
	headingText.setOutlineColor(sf::Color::White);
	headingText.setString("0");
	headingText.setCharacterSize(54);
	headingText.setStyle(sf::Text::Bold);
	speedText 	= headingText;
	speedText.setCharacterSize(20);
	fpsText 	= speedText;
	inphaseText = speedText;
	ampText 	= speedText;
	controlsText= speedText;
	controlsText.setCharacterSize(15);

	//Set Strings
	headingText.setString("SPIROMORPH");
	speedText.setString("SPEED: ");
	inphaseText.setString("IN PHASE: ");
	ampText.setString("AMPLITUDE: ");
	fpsText.setString("FPS: ");
	controlsText.setString("      **CONTROLS**\nSpeed-       PgUp/PgDn\nInPhase-     Home/End\nAmplitude- Ins/Del\nHide Hud-   H");

	//Set positions
	headingText.setPosition((window->getSize().x / 2) - 170, 30);
	speedText.setPosition(10, 30);
	inphaseText.setPosition(10, 50);
	ampText.setPosition(10, 70);
	fpsText.setPosition(window->getSize().x -100, 30);
	controlsText.setPosition(10, window->getSize().y-120);

	//load the font
	//note that this path is relative to the workspace
	if (!font.loadFromFile("content/HoboStd.otf"))
	{
		std::cout << "Failed to load font" << std::endl;
	}
}

void SetSpeedText(float _speed){
	speedText.setString("SPEED:        " + to_string(_speed));
}
void SetInphaseText(int _inphase, int _numElements){
	inphaseText.setString("IN PHASE:   " + to_string(_inphase) + " of " + to_string(_numElements));
}
void SetAmpText(float _amp){
	ampText.setString("AMPLITUDE: " + to_string(_amp));
}
void SetFpsText(float _fps){
	fpsText.setString("FPS: " + to_string(_fps));
}