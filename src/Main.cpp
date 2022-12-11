#include "Platform/Platform.hpp"
#include "Razshaders.h"
#include <sstream>
#include <string>
#include "ui.h"
#include "main.h"
#include "mainopt.h"

	struct mainopt_struct options;
	int winW = options.window_width; // Window Size
	int winH = options.window_height;
	bool drawHud = true;

	static float element_radius;

	#define M_PI 3.14159265358979323846

	struct element_struct
	{
		int 	freq;			// +/- integer factor of base circle (non-0)
		float 	amplitude;		// 0-1
		int 	phase_offset;	// 0-base_resolution-1
	};

	struct envelope_struct
	{
		float offset;		// 0 - 1
		bool reset;
	};

	typedef struct
	{
		float x;
		float y;
	} point_f;

	struct colour_struct
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

//********************************************************************************************************
// Private variables
//********************************************************************************************************

	static float *sin_table;
	static struct colour_struct *colour_table;

	static struct element_struct *elements;
	static struct envelope_struct *envelopes;

	static int center_x;
	static int center_y;


//********************************************************************************************************
// Forward Declarations
//********************************************************************************************************

	void generate_sin_table(void);
	void draw_frame(sf::RenderTarget &rend);
	void DrawLineTo(sf::RenderTarget &rend, sf::Vector2f thispoint, sf::Color thiscolor);
	static point_f calc_point_of_circle(int brads, float radius);
	static point_f calc_point_of_element(struct element_struct element, int base_angle);
	static point_f sum_points_f(point_f a, point_f b);
	static void draw_point_for_base_angle(sf::RenderTarget &rend, int base_angle);
	static uint32_t get_elapsed_u32(uint32_t current);
	static float raised_cosine(float position);
	static int rand_in_range_i(int min, int max);
	static void envelope_init(void);
	static void colour_table_init(void);

// Main------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int retval = 0;

	//	Parse options,
	options = mainopt_parse(argc, argv);
	if(options.error)
		retval = -1;
	if(options.finished)
		return retval;

	//Create a window
	winW = options.window_width; // Window Size
	winH = options.window_height;
	sf::Uint32 windowStyle;

	if(options.full_screen)
		windowStyle = sf::Style::Fullscreen;
	else
		windowStyle = sf::Style::Titlebar | sf::Style::Close;

	sf::RenderWindow window(sf::VideoMode(winW, winH), "Spiromorph", windowStyle);

	//Initialize Shaders
	if(!InitShaders(winW,winH)) return -1;

	//Creating text and setting it's font and parameters
	InitTextObjects(&window);

	// Start a Clock for shaders and frames
	sf::Clock clk;

	// Spiromorph initialisations
	float 	frame_time;
	float 	base_envelope = 0;
	float 	envelope;
	float 	second_time = 0;
	int i;
	int frame_cnt = 0;

	elements = static_cast<element_struct*>(malloc(sizeof(*elements)*options.number_of_elements));
	envelopes = static_cast<envelope_struct*>(calloc(sizeof(*envelopes), options.number_of_elements));
	sin_table = static_cast<float*>(malloc(sizeof(*sin_table)*options.base_resolution));
	colour_table = static_cast<colour_struct*>(malloc(sizeof(*colour_table)*options.base_resolution));

	center_x	= options.window_width / 2;
	center_y	= options.window_height / 2;
  	element_radius = (options.amplitude * 0.5 * options.window_height / ((options.number_of_elements - options.envelopes_in_phase + 1)*0.5 + (options.envelopes_in_phase - 1)*1.0));

	i = 0;
	while(i != options.number_of_elements){
		elements[i].amplitude = 1;
		elements[i].freq = 1;
		elements[i].phase_offset = 0;
		i++;
	}

	generate_sin_table();
	envelope_init();
	colour_table_init();
	SetSpeedText(options.envelope_speed);
	SetInphaseText(options.envelopes_in_phase, options.number_of_elements);
	SetAmpText(options.amplitude);

	// Window Loop-------------------------------------------------
	while (window.isOpen())
	{
		sf::Event event;

		//handle event inputs
		while (window.pollEvent(event))
		{
			// Close window if x button is pressed
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
			// Speed
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::PageUp){
				options.envelope_speed += 0.01f;
				if (options.envelope_speed < 0.001f && options.envelope_speed > -0.001f) options.envelope_speed = 0.0f;
				if (options.envelope_speed > 1.0f) options.envelope_speed = 1.0f;
				SetSpeedText(options.envelope_speed);
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::PageDown){
				options.envelope_speed -= 0.01f;
				if (options.envelope_speed < 0.001f && options.envelope_speed > -0.001f) options.envelope_speed = 0.0f;
				if (options.envelope_speed < -1.0f) options.envelope_speed = -1.0f;
				SetSpeedText(options.envelope_speed);
			}
			// Elements In Phase
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Home){
				options.envelopes_in_phase++;
				if (options.envelopes_in_phase > options.number_of_elements) options.envelopes_in_phase = options.number_of_elements;
			  	element_radius = (options.amplitude * 0.5 * options.window_height / ((options.number_of_elements - options.envelopes_in_phase + 1)*0.5 + (options.envelopes_in_phase - 1)*1.0));
				envelope_init();
				SetInphaseText(options.envelopes_in_phase, options.number_of_elements);
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::End){
				options.envelopes_in_phase--;
				if (options.envelopes_in_phase < 1) options.envelopes_in_phase = 1;
			  	element_radius = (options.amplitude * 0.5 * options.window_height / ((options.number_of_elements - options.envelopes_in_phase + 1)*0.5 + (options.envelopes_in_phase - 1)*1.0));
				envelope_init();
				SetInphaseText(options.envelopes_in_phase, options.number_of_elements);
			}
			// Amplitude
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Insert){
				options.amplitude += 0.1f;
				if (options.amplitude > 2.0f) options.amplitude = 2.0f;
			  	element_radius = (options.amplitude * 0.5 * options.window_height / ((options.number_of_elements - options.envelopes_in_phase + 1)*0.5 + (options.envelopes_in_phase - 1)*1.0));
				envelope_init();
				SetAmpText(options.amplitude);
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Delete){
				options.amplitude -= 0.1f;
				if (options.amplitude < 0.1f) options.amplitude = 0.1f;
			  	element_radius = (options.amplitude * 0.5 * options.window_height / ((options.number_of_elements - options.envelopes_in_phase + 1)*0.5 + (options.envelopes_in_phase - 1)*1.0));
				envelope_init();
				SetAmpText(options.amplitude);
			}
			// Draw HUD ON/OFF
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H)
				drawHud = !drawHud;
		}

		// Frametime
		frame_time = (float)get_elapsed_u32(clk.getElapsedTime().asMilliseconds()) / 1000.0;
		second_time += frame_time;
		if(second_time > 1.0)
		{
			second_time = 0;
			SetFpsText(frame_cnt);
			frame_cnt = 0;
		};

		i = 0;

		//rotate base envelope
		base_envelope += options.envelope_speed * frame_time;
		if(base_envelope >= 1.0)
			base_envelope -= 1.0;

		while(i != options.number_of_elements)
		{
			envelope = envelopes[i].offset + base_envelope;
			if(envelope >= 1.0)
				envelope -= 1.0;

			if(envelope < 0.5 && !envelopes[i].reset)
			{
				elements[i].freq = rand_in_range_i(1, options.element_freq_max);
				if(rand()&1)
					elements[i].freq *=-1;
				elements[i].phase_offset = rand_in_range_i(0, options.base_resolution);
			};
			envelopes[i].reset = envelope < 0.5;
			elements[i].amplitude = raised_cosine(envelope);
			i++;
		};

		// Set shader parameters
		UpdateShaderParameters(&clk, window.getSize().x/2, (window.getSize().y/2)+350);

		window.clear();

		// Draw Rendertexture for post effects (WarpShader)
		if(drawHud){
			rendertexture.clear(sf::Color::Transparent);
			rendertexture.draw(headingText);
			rendertexture.display();
			render.setTexture(rendertexture.getTexture()); 	//Set RenderSprite from RenderTexture
			window.draw(render, &warpshader);				//Draw RenderSprite with shader
		}

		// Draw Rendertexture for spiromorph with (BloomShader)
		rendertexture.clear(sf::Color::Transparent);
		draw_frame(rendertexture);
		rendertexture.display();
		render.setTexture(rendertexture.getTexture());
		window.draw(render, &bloomshader);

		// Draw Hud
		if(drawHud){
			window.draw(speedText);
			window.draw(fpsText);
			window.draw(inphaseText);
			window.draw(ampText);
			window.draw(controlsText);
		}

		window.display();

		frame_cnt++;

	} // End of Window Loop
	return 0;
}	// End Of Main---------------------------------------------------------------------


//********************************************************************************************************
// Spiromorph functions
//********************************************************************************************************

static void envelope_init(void)
{
	int i = 0;
	while(i != options.number_of_elements)
		envelopes[i++].offset = 0;

	i = 0;
	while(i != options.number_of_elements - options.envelopes_in_phase + 1)
	{
		envelopes[i].offset = (float)i / (float)(options.number_of_elements - options.envelopes_in_phase + 1);
		i++;
	};
}

static float raised_cosine(float position)
{
	int brads;

	brads = (position * options.base_resolution);
	brads += options.base_resolution/4;
	brads &= options.base_resolution-1;

	return (sin_table[brads]/-2.0)+0.5;
}

static int rand_in_range_i(int min, int max)
{
	int value;

	value = rand() % abs(max-min);

	if(min < max)
		value += min;
	else
		value += max;

	return value;
}

void generate_sin_table(void)
{
	int brads = 0;
	float q;

	while(brads != options.base_resolution)
	{
		q = (float)brads / options.base_resolution;
		q *= 2 * M_PI;
		sin_table[brads] = sin(q);
		brads++;
	};
}

static void colour_table_init(void)
{
	int i = 0;
	float pos;

	while(i != options.base_resolution)
	{
		pos = i;
		pos /= (float)options.base_resolution;

		colour_table[i].r = 255.0*raised_cosine(pos + 0.0000);
		colour_table[i].g = 255.0*raised_cosine(pos + 0.3333);
		colour_table[i].b = 255.0*raised_cosine(pos + 0.6666);
		i++;
	};
}

void draw_frame(sf::RenderTarget &rend)
{
	int base_angle = 0;

	draw_point_for_base_angle(rend, 0);
	rend.clear(sf::Color::Transparent);

	base_angle = 1;
	while(base_angle != options.base_resolution)
	{
		draw_point_for_base_angle(rend, base_angle);
		base_angle++;
	};
	draw_point_for_base_angle(rend, 0);
}

void DrawLineTo(sf::RenderTarget &rend, sf::Vector2f thispoint, sf::Color thiscolor){
	static sf::Vector2f lastpoint = thispoint;
	sf::VertexArray thisline(sf::Lines,2);
	thisline[0] = lastpoint;
	thisline[0].color = thiscolor;
	thisline[1] = thispoint;
	thisline[1].color = thiscolor;
	rend.draw(thisline);
	lastpoint = thispoint;
}

static void draw_point_for_base_angle(sf::RenderTarget &rend, int base_angle)
{

	sf::Vector2f point_final;
	point_f point;
	point.x = 0.0;
	point.y = 0.0;
	int i;

	i = 0;
	while(i != options.number_of_elements)
	{
		point = sum_points_f(point, calc_point_of_element(elements[i], base_angle));
		i++;
	};

	sf::Color currentColor = sf::Color(colour_table[base_angle].r, colour_table[base_angle].g, colour_table[base_angle].b, 255);

	point_final.x = center_x + point.x;
	point_final.y = center_y + point.y;
	DrawLineTo(rend, point_final, currentColor);
}


static point_f calc_point_of_element(struct element_struct element, int base_angle)
{
	point_f point;
	int freq;
	float amplitude;

	freq = element.freq * base_angle;
	freq += element.phase_offset;

	amplitude = element.amplitude * element_radius;
	point = calc_point_of_circle(freq, amplitude);

	return point;
}

static point_f calc_point_of_circle(int brads, float radius)
{
	point_f point;

	brads &= options.base_resolution-1;
	point.x = sin_table[brads] * radius;

	brads += options.base_resolution/4;
	brads &= options.base_resolution-1;
	point.y = sin_table[brads] * radius;

	return point;
}

static point_f sum_points_f(point_f a, point_f b)
{
	point_f sum;

	sum.x = a.x + b.x;
	sum.y = a.y + b.y;

	return sum;
}

static uint32_t get_elapsed_u32(uint32_t current)
{
	static uint32_t last = 0;
	uint32_t elapsed;

	elapsed = current - last;
	last = current;

	return elapsed;
}