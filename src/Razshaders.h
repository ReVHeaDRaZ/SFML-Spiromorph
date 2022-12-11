#pragma once

sf::Shader warpshader;
sf::Shader fireshader;
sf::Shader glowshader;
sf::Shader bloomshader;

sf::RenderTexture rendertexture;
sf::Sprite render;

sf::Texture firetexture;
sf::Sprite fireball;

sf::RenderStates renderstate;
bool InitShaders(float WIN_WIDTH, float WIN_HEIGHT);
void UpdateShaderParameters(sf::Clock* clk, float posX, float posY);
void SetFireShaderResolution(float x, float y);

bool InitShaders(float WIN_WIDTH, float WIN_HEIGHT){
	// Shaders

	// Check if shaders are available
	if (!sf::Shader::isAvailable())
	{
		std::cerr << "Shader are not available" << std::endl;
		return false;
	}

	// Load shaders
	if (!warpshader.loadFromFile("content/example_001.frag", sf::Shader::Fragment))
	{
		std::cerr << "Error while shaders" << std::endl;
		return false;
	}
	if (!fireshader.loadFromFile("content/fire.glsl", sf::Shader::Fragment))
	{
		std::cerr << "Error while shaders" << std::endl;
		return false;
	}
	if (!glowshader.loadFromFile("content/razdot.frag", sf::Shader::Fragment))
	{
		std::cerr << "Error while shaders" << std::endl;
		return false;
	}
	if (!bloomshader.loadFromFile("content/razbloom.frag", sf::Shader::Fragment))
	{
		std::cerr << "Error while shaders" << std::endl;
		return false;
	}

	// Set Shader resolutions
	fireshader.setUniform("resolution", sf::Vector2f(45, 45));
	glowshader.setUniform("resolution", sf::Vector2f(45, 45));
	bloomshader.setUniform("resolution", sf::Vector2f(WIN_WIDTH, WIN_HEIGHT));

	//Set Blend renderstate for glow
	renderstate.blendMode = sf::BlendAdd;
	renderstate.shader = &glowshader;

	//Create Texture and Sprite for fireball
	firetexture.create(WIN_WIDTH, WIN_HEIGHT);
	fireball.setTexture(firetexture);

	// Create Rendertexture for shader sprite
	rendertexture.create(WIN_WIDTH, WIN_HEIGHT);

	return true;
}

void UpdateShaderParameters(sf::Clock* clk, float posX, float posY){
	// Set shader parameters
		warpshader.setUniform("time", clk->getElapsedTime().asSeconds());
		fireshader.setUniform("time", clk->getElapsedTime().asSeconds() * 2);
		fireshader.setUniform("mouse", sf::Vector2f(posX + 20, posY - 705));
		glowshader.setUniform("mouse", sf::Vector2f(posX + 20, posY));

}

void SetFireShaderResolution(float x, float y){
	fireshader.setUniform("resolution", sf::Vector2f(x, y));
}
