#pragma once
#include <sstream>
#include <string>

#define CONTROLS_TEXT 	"      **CONTROLS**\n"\
						"Spawn Spiro-     LMB\n"\
						"Select Spiro-     Up/Dn\n"\
						"Elements-          [ / ]\n"\
						"Speed-               PgUp/PgDn\n"\
						"InPhase-             Home/End\n"\
						"Amplitude-         Ins/Del\n"\
						"Hide Hud-           H\n"\
						"Glow Intensity-   W/S\n"\
						"Glow Sharpness- A/D\n"

sf::Font font;
sf::Text headingText, currentSpiroText, speedText, inphaseText, ampText, fpsText, controlsText, bloomText;

void InitTextObjects(sf::RenderWindow* window);
void SetSpeedText(float _speed);
void SetFpsText(float _fps);
void SetInphaseText(int _inphase, int _numElements);
void SetAmpText(float _amp);
void SetCurrentSpiroText(float _current);
void SetBloomText(float _bloomintesity, float _bloomBlurAmount);

template <typename T>
// function to convert a non-string variable to a string.
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}


void InitTextObjects(sf::RenderWindow* window)
{
	// Set Styles
	headingText.setFont(font);
	headingText.setFillColor(sf::Color::White);
	headingText.setOutlineColor(sf::Color::White);
	headingText.setString("0");
	headingText.setCharacterSize(54);
	headingText.setStyle(sf::Text::Bold);
	speedText 	= headingText;
	speedText.setCharacterSize(20);
	fpsText 		= speedText;
	inphaseText 	= speedText;
	ampText 		= speedText;
	currentSpiroText= speedText;
	controlsText	= speedText;
	controlsText.setCharacterSize(15);
	bloomText = controlsText;

	// Set Strings
	headingText.setString("SPIROMORPH");
	speedText.setString("SPEED: ");
	inphaseText.setString("IN PHASE: ");
	ampText.setString("AMPLITUDE: ");
	fpsText.setString("FPS: ");
	currentSpiroText.setString("SPIRO: ");
	controlsText.setString(CONTROLS_TEXT);
	bloomText.setString("Intensity: ");

	// Set positions
	headingText.setPosition((window->getSize().x / 2) - 170, 30);
	currentSpiroText.setPosition(10, 30);
	speedText.setPosition(10, 50);
	inphaseText.setPosition(10, 70);
	ampText.setPosition(10, 90);
	bloomText.setPosition(window->getSize().x -170, window->getSize().y-90);

	fpsText.setPosition(window->getSize().x -100, 30);
	controlsText.setPosition(10, window->getSize().y-200);

	// load the font
	// note that this path is relative to the workspace
	if (!font.loadFromFile("content/HoboStd.otf"))
	{
		std::cout << "Failed to load font" << std::endl;
	}
}

void SetCurrentSpiroText(float _current){
	currentSpiroText.setString("SPIRO:         " + to_string(_current));
}
void SetSpeedText(float _speed){
	speedText.setString("SPEED:         " + to_string(_speed));
}
void SetInphaseText(int _inphase, int _numElements){
	inphaseText.setString("IN PHASE:    " + to_string(_inphase) + " of " + to_string(_numElements));
}
void SetAmpText(float _amp){
	ampText.setString("AMPLITUDE: " + to_string(_amp));
}
void SetFpsText(float _fps){
	fpsText.setString("FPS: " + to_string(_fps));
}
void SetBloomText(float _bloomintesity, float _bloomBlurAmount){
	bloomText.setString("*BLOOM SHADER*\n  Intensity:   " + to_string(_bloomintesity) + "\n  Sharpness: " + to_string(_bloomBlurAmount));
}