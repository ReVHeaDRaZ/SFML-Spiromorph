#include "Platform/Platform.hpp"
#include "Razshaders.h"
#include <sstream>
#include <string>
#include <vector>
#include "ui.h"
#include "main.h"
#include "mainopt.h"
#include "spiromorph.h"

struct mainopt_struct options;
int winW = options.window_width; // Window Size
int winH = options.window_height;
bool drawHud = true;

float 	frame_time;
float 	second_time = 0;
int 	frame_cnt = 0;

std::vector<Spiromorph> spiromorphs;
int numberOfSpiros 	= 1;
int selectedSpiro	= 1;

float bloomIntensity = 3.0f;
float bloomBlurAmount = 640.0f;

uint32_t get_elapsed_u32(uint32_t current);
void CalculateFrameTime(sf::Clock Frameclk);

void ResetAllSpiromorphsOptions(std::vector<Spiromorph> &spiros, int _numberOfSpiros, struct mainopt_struct mainopts);
void ReinitAllSpiromorphs(std::vector<Spiromorph> &spiros, int _numberOfSpiros, struct mainopt_struct mainopts);
void ReinitASpiromorph(std::vector<Spiromorph> &spiros, int _selectSpiro);
void InitANewSpiromorph(std::vector<Spiromorph> &spiros, int _selectSpiro, struct mainopt_struct mainopts);
void UpdateSpiromorphs(std::vector<Spiromorph> &spiros, int _numberOfSpiros, float Frametime);
void SetSpiromorphSpeed(std::vector<Spiromorph> &spiros, int _selectSpiro, float _speed);
void SetTextForCurrentSpiro();
void DrawHud(sf::RenderTarget &window);

// Main------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int retval = 0;

	// Parse options,
	options = mainopt_parse(argc, argv);
	if(options.error)
		retval = -1;
	if(options.finished)
		return retval;

	// Create a window
	winW = options.window_width; // Window Size
	winH = options.window_height;
	sf::Uint32 windowStyle;

	if(options.full_screen)
		windowStyle = sf::Style::Fullscreen;
	else
		windowStyle = sf::Style::Titlebar | sf::Style::Close;

	sf::RenderWindow window(sf::VideoMode(winW, winH), "Spiromorph", windowStyle);
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);

	// Initialize Shaders
	if(!InitShaders(winW,winH)) return -1;

	// Creating text and setting it's font and parameters
	InitTextObjects(&window);

	// Start a Clock for shaders and frames
	sf::Clock clk;

	// Initialize Spiromorphs
	for(int i=0; i<numberOfSpiros; i++)
		spiromorphs.push_back(Spiromorph(options));

	ReinitAllSpiromorphs(spiromorphs, numberOfSpiros, options);

	// Initialize Hud
	SetCurrentSpiroText(selectedSpiro);
	SetSpeedText(options.envelope_speed);
	SetInphaseText(options.envelopes_in_phase, options.number_of_elements);
	SetAmpText(options.amplitude);
	SetBloomText(bloomIntensity, bloomBlurAmount);

	// Window Loop-------------------------------------------------
	while (window.isOpen())
	{
		sf::Event event;

		// handle event inputs
		while (window.pollEvent(event))
		{
			// Close window if x button is pressed
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();

			// Spiro Selection
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up){
				selectedSpiro++;
				if (selectedSpiro > numberOfSpiros) selectedSpiro = numberOfSpiros;
				SetTextForCurrentSpiro();
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down){
				selectedSpiro--;
				if (selectedSpiro < 1) selectedSpiro = 1;
				SetTextForCurrentSpiro();
			}
			// Speed
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::PageUp){
				spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed += 0.01f;
				if (spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed < 0.001f && spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed > -0.001f)
					spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed = 0.0f;
				if (spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed > 1.0f)
					spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed = 1.0f;
				SetSpeedText(spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed);
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::PageDown){
				spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed -= 0.01f;
				if (spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed < 0.001f && spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed > -0.001f)
					spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed = 0.0f;
				if (spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed < -1.0f)
					spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed = -1.0f;
				SetSpeedText(spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed);
			}
			// Elements In Phase
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Home){
				spiromorphs[selectedSpiro-1].spiroOptions.envelopes_in_phase++;
				if (spiromorphs[selectedSpiro-1].spiroOptions.envelopes_in_phase > spiromorphs[selectedSpiro-1].spiroOptions.number_of_elements)
					spiromorphs[selectedSpiro-1].spiroOptions.envelopes_in_phase = spiromorphs[selectedSpiro-1].spiroOptions.number_of_elements;
				SetTextForCurrentSpiro();
				ReinitASpiromorph(spiromorphs, selectedSpiro);
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::End){
				spiromorphs[selectedSpiro-1].spiroOptions.envelopes_in_phase--;
				if (spiromorphs[selectedSpiro-1].spiroOptions.envelopes_in_phase < 1)
					spiromorphs[selectedSpiro-1].spiroOptions.envelopes_in_phase = 1;
				SetTextForCurrentSpiro();
				ReinitASpiromorph(spiromorphs, selectedSpiro);
			}
			// Amplitude
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Insert){
				spiromorphs[selectedSpiro-1].spiroOptions.amplitude += 0.1f;
				if (spiromorphs[selectedSpiro-1].spiroOptions.amplitude > 2.0f)
					spiromorphs[selectedSpiro-1].spiroOptions.amplitude = 2.0f;
				SetTextForCurrentSpiro();
				ReinitASpiromorph(spiromorphs, selectedSpiro);
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Delete){
				spiromorphs[selectedSpiro-1].spiroOptions.amplitude -= 0.1f;
				if (spiromorphs[selectedSpiro-1].spiroOptions.amplitude < 0.1f) spiromorphs[selectedSpiro-1].spiroOptions.amplitude = 0.1f;
				SetTextForCurrentSpiro();
				ReinitASpiromorph(spiromorphs, selectedSpiro);
			}
			// Draw HUD ON/OFF
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H)
				drawHud = !drawHud;
			// Number of Elements
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::RBracket){
				spiromorphs[selectedSpiro-1].spiroOptions.number_of_elements++;
				if(spiromorphs[selectedSpiro-1].spiroOptions.number_of_elements > 30)
					spiromorphs[selectedSpiro-1].spiroOptions.number_of_elements = 30;
				spiromorphs[selectedSpiro-1].ReinitNumberOfElements();
				SetTextForCurrentSpiro();
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LBracket){
				spiromorphs[selectedSpiro-1].spiroOptions.number_of_elements--;
				if(spiromorphs[selectedSpiro-1].spiroOptions.number_of_elements < 2)
					spiromorphs[selectedSpiro-1].spiroOptions.number_of_elements = 2;
				if(spiromorphs[selectedSpiro-1].spiroOptions.envelopes_in_phase > spiromorphs[selectedSpiro-1].spiroOptions.number_of_elements)
					spiromorphs[selectedSpiro-1].spiroOptions.envelopes_in_phase = spiromorphs[selectedSpiro-1].spiroOptions.number_of_elements;
				spiromorphs[selectedSpiro-1].ReinitNumberOfElements();
				SetTextForCurrentSpiro();
			}
			// Add a Spiromorph
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
				spiromorphs.push_back(Spiromorph(options));
				numberOfSpiros++;
				spiromorphs.back().center_x = sf::Mouse::getPosition(window).x;
				spiromorphs.back().center_y = sf::Mouse::getPosition(window).y;
				InitANewSpiromorph(spiromorphs, numberOfSpiros-1, options);
				selectedSpiro = numberOfSpiros;
				SetTextForCurrentSpiro();
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W){
				bloomIntensity += 0.5f;
				if(bloomIntensity > 10.0f) bloomIntensity = 10.0f;
				SetBloomShader(bloomIntensity, bloomBlurAmount);
				SetBloomText(bloomIntensity, bloomBlurAmount);
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S){
				bloomIntensity -= 0.5f;
				if(bloomIntensity < 0.0f) bloomIntensity = 0.0f;
				SetBloomShader(bloomIntensity, bloomBlurAmount);
				SetBloomText(bloomIntensity, bloomBlurAmount);
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D){
				bloomBlurAmount += 8.0f;
				if(bloomBlurAmount > 1000.0f) bloomBlurAmount = 1000.0f;
				SetBloomShader(bloomIntensity, bloomBlurAmount);
				SetBloomText(bloomIntensity, bloomBlurAmount);
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A){
				bloomBlurAmount -= 8.0f;
				if(bloomBlurAmount < 0.0f) bloomBlurAmount = 0.0f;
				SetBloomShader(bloomIntensity, bloomBlurAmount);
				SetBloomText(bloomIntensity, bloomBlurAmount);
			}
		}

		CalculateFrameTime(clk);
		UpdateSpiromorphs(spiromorphs, numberOfSpiros, frame_time);

		// Set shader parameters
		UpdateShaderParameters(&clk, sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

		window.clear();

		// Draw Rendertexture for post effects (WarpShader)
		if(drawHud){
			rendertexture.clear(sf::Color::Transparent);
			rendertexture.draw(headingText);
			rendertexture.display();
			render.setTexture(rendertexture.getTexture()); 	//Set RenderSprite from RenderTexture
			window.draw(render, &warpshader);				//Draw RenderSprite with shader
		}

		// Draw spiromorphs with (BloomShader)
		for(int i=0; i<numberOfSpiros; i++){
			rendertexture.clear(sf::Color::Transparent);
			spiromorphs[i].draw_frame(rendertexture);
			rendertexture.display();
			render.setTexture(rendertexture.getTexture());
			window.draw(render, &bloomshader);
		}

		// Draw Glowing Mouse pointer
		rendertexture.clear(sf::Color::Transparent);
		rendertexture.display();
		render.setTexture(rendertexture.getTexture());
		window.draw(render, renderstate);

		// Draw Hud
		if(drawHud){
			DrawHud(window);
		}

		window.display();

		frame_cnt++;

	} // End of Window Loop
	return 0;
}	// End Of Main---------------------------------------------------------------------

void CalculateFrameTime(sf::Clock Frameclk){
	frame_time = (float)get_elapsed_u32(Frameclk.getElapsedTime().asMilliseconds()) / 1000.0;
	second_time += frame_time;
	if(second_time > 1.0)
	{
		second_time = 0;
		SetFpsText(frame_cnt);
		frame_cnt = 0;
	};
}

uint32_t get_elapsed_u32(uint32_t current)
{
	static uint32_t last = 0;
	uint32_t elapsed;

	elapsed = current - last;
	last = current;

	return elapsed;
}

void ResetAllSpiromorphsOptions(std::vector<Spiromorph> &spiros, int _numberOfSpiros, struct mainopt_struct mainopts){
	for(int i=0; i < _numberOfSpiros; i++){
		spiros[i].spiroOptions = mainopts;
		spiros[i].spiroOptions.amplitude = spiros[i].spiroOptions.amplitude / (i+1);
	}
}

void ReinitAllSpiromorphs(std::vector<Spiromorph> &spiros, int _numberOfSpiros, struct mainopt_struct mainopts){
	for(int i=0; i < _numberOfSpiros; i++){
		spiros[i].spiroOptions = mainopts;
		spiros[i].spiroOptions.amplitude = spiros[i].spiroOptions.amplitude / (i+1);
		spiros[i].reinit();
	}
}

void ReinitASpiromorph(std::vector<Spiromorph> &spiros, int _selectSpiro){
		spiros[_selectSpiro-1].reinit();
}

void InitANewSpiromorph(std::vector<Spiromorph> &spiros, int _selectSpiro, struct mainopt_struct mainopts){
		spiros[_selectSpiro].spiroOptions = mainopts;
		spiros[_selectSpiro].reinit();
}

void UpdateSpiromorphs(std::vector<Spiromorph> &spiros, int _numberOfSpiros, float Frametime){
	for(int i=0; i < _numberOfSpiros; i++){
		spiros[i].Update(Frametime);
	}
}

void SetSpiromorphSpeed(std::vector<Spiromorph> &spiros, int _selectSpiro, float _speed){
	spiros[_selectSpiro-1].spiroOptions.envelope_speed = _speed;
}

void SetTextForCurrentSpiro(){
	SetCurrentSpiroText(selectedSpiro);
	SetSpeedText(spiromorphs[selectedSpiro-1].spiroOptions.envelope_speed);
	SetInphaseText(spiromorphs[selectedSpiro-1].spiroOptions.envelopes_in_phase, spiromorphs[selectedSpiro-1].spiroOptions.number_of_elements);
	SetAmpText(spiromorphs[selectedSpiro-1].spiroOptions.amplitude);
}

void DrawHud(sf::RenderTarget &window){
	window.draw(currentSpiroText);
	window.draw(speedText);
	window.draw(fpsText);
	window.draw(inphaseText);
	window.draw(ampText);
	window.draw(controlsText);
	window.draw(bloomText);
}