#pragma once
#include "mainopt.h"

extern struct mainopt_struct options;


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


class Spiromorph{

private:
	float *sin_table;
	struct colour_struct *colour_table;

	struct element_struct *elements;
	struct envelope_struct *envelopes;


	void generate_sin_table(void);
	void DrawLineTo(sf::RenderTarget &rend, sf::Vector2f thispoint, sf::Color thiscolor);
	point_f calc_point_of_circle(int brads, float radius);
	point_f calc_point_of_element(struct element_struct element, int base_angle);
	point_f sum_points_f(point_f a, point_f b);
	void draw_point_for_base_angle(sf::RenderTarget &rend, int base_angle);
	float raised_cosine(float position);
	int rand_in_range_i(int min, int max);
	void envelope_init(void);
	void colour_table_init(void);

public:
	int center_x;
	int center_y;
	float element_radius;

	void draw_frame(sf::RenderTarget &rend);
	void reinit();
	void Update(float FrameTime);

	struct mainopt_struct spiroOptions;
	float 	spiroFrameTime;
	float 	base_envelope = 0;
	float 	envelope;


Spiromorph(struct mainopt_struct options){
	spiroOptions = options;
	elements = static_cast<element_struct*>(malloc(sizeof(*elements)*options.number_of_elements));
	envelopes = static_cast<envelope_struct*>(calloc(sizeof(*envelopes), options.number_of_elements));
	sin_table = static_cast<float*>(malloc(sizeof(*sin_table)*options.base_resolution));
	colour_table = static_cast<colour_struct*>(malloc(sizeof(*colour_table)*options.base_resolution));

	center_x	= options.window_width / 2;
	center_y	= options.window_height / 2;
  	element_radius = (options.amplitude * 0.5 * options.window_height / ((options.number_of_elements - options.envelopes_in_phase + 1)*0.5 + (options.envelopes_in_phase - 1)*1.0));

	for(int i=0; i<options.number_of_elements;i++){
		elements[i].amplitude = 1;
		elements[i].freq = 1;
		elements[i].phase_offset = 0;
	}
	generate_sin_table();
	envelope_init();
	colour_table_init();
}
};

void Spiromorph::envelope_init(void)
{
	int i = 0;
	while(i != spiroOptions.number_of_elements)
		envelopes[i++].offset = 0;

	i = 0;
	while(i != spiroOptions.number_of_elements - spiroOptions.envelopes_in_phase + 1)
	{
		envelopes[i].offset = (float)i / (float)(spiroOptions.number_of_elements - spiroOptions.envelopes_in_phase + 1);
		i++;
	};
}

float Spiromorph::raised_cosine(float position)
{
	int brads;

	brads = (position * spiroOptions.base_resolution);
	brads += spiroOptions.base_resolution/4;
	brads &= spiroOptions.base_resolution-1;

	return (sin_table[brads]/-2.0)+0.5;
}

int Spiromorph::rand_in_range_i(int min, int max)
{
	int value;

	value = rand() % abs(max-min);

	if(min < max)
		value += min;
	else
		value += max;

	return value;
}

void Spiromorph::generate_sin_table(void)
{
	int brads = 0;
	float q;

	while(brads != spiroOptions.base_resolution)
	{
		q = (float)brads / spiroOptions.base_resolution;
		q *= 2 * M_PI;
		sin_table[brads] = sin(q);
		brads++;
	};
}

void Spiromorph::colour_table_init(void)
{
	int i = 0;
	float pos;

	while(i != spiroOptions.base_resolution)
	{
		pos = i;
		pos /= (float)spiroOptions.base_resolution;

		colour_table[i].r = 255.0*raised_cosine(pos + 0.0000);
		colour_table[i].g = 255.0*raised_cosine(pos + 0.3333);
		colour_table[i].b = 255.0*raised_cosine(pos + 0.6666);
		i++;
	};
}

void Spiromorph::draw_frame(sf::RenderTarget &rend)
{
	int base_angle = 0;

	draw_point_for_base_angle(rend, 0);
	rend.clear(sf::Color::Transparent);

	base_angle = 1;
	while(base_angle != spiroOptions.base_resolution)
	{
		draw_point_for_base_angle(rend, base_angle);
		base_angle++;
	};
	draw_point_for_base_angle(rend, 0);
}

void Spiromorph::DrawLineTo(sf::RenderTarget &rend, sf::Vector2f thispoint, sf::Color thiscolor){
	static sf::Vector2f lastpoint = thispoint;
	sf::VertexArray thisline(sf::Lines,2);
	thisline[0] = lastpoint;
	thisline[0].color = thiscolor;
	thisline[1] = thispoint;
	thisline[1].color = thiscolor;
	rend.draw(thisline);
	lastpoint = thispoint;
}

void Spiromorph::draw_point_for_base_angle(sf::RenderTarget &rend, int base_angle)
{

	sf::Vector2f point_final;
	point_f point;
	point.x = 0.0;
	point.y = 0.0;
	int i;

	i = 0;
	while(i != spiroOptions.number_of_elements)
	{
		point = sum_points_f(point, calc_point_of_element(elements[i], base_angle));
		i++;
	};

	sf::Color currentColor = sf::Color(colour_table[base_angle].r, colour_table[base_angle].g, colour_table[base_angle].b, 255);

	point_final.x = center_x + point.x;
	point_final.y = center_y + point.y;
	DrawLineTo(rend, point_final, currentColor);
}


point_f Spiromorph::calc_point_of_element(struct element_struct element, int base_angle)
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

point_f Spiromorph::calc_point_of_circle(int brads, float radius)
{
	point_f point;

	brads &= spiroOptions.base_resolution-1;
	point.x = sin_table[brads] * radius;

	brads += spiroOptions.base_resolution/4;
	brads &= spiroOptions.base_resolution-1;
	point.y = sin_table[brads] * radius;

	return point;
}

point_f Spiromorph::sum_points_f(point_f a, point_f b)
{
	point_f sum;

	sum.x = a.x + b.x;
	sum.y = a.y + b.y;

	return sum;
}


void Spiromorph::reinit(){
	element_radius = (spiroOptions.amplitude * 0.5 * spiroOptions.window_height / ((spiroOptions.number_of_elements - spiroOptions.envelopes_in_phase + 1)*0.5 + (spiroOptions.envelopes_in_phase - 1)*1.0));
	envelope_init();
}

void Spiromorph::Update(float FrameTime){
		spiroFrameTime = FrameTime;
		int i = 0;

		//rotate base envelope
		base_envelope += spiroOptions.envelope_speed * spiroFrameTime;
		if(base_envelope >= 1.0)
			base_envelope -= 1.0;

		while(i != spiroOptions.number_of_elements)
		{
			envelope = envelopes[i].offset + base_envelope;
			if(envelope >= 1.0)
				envelope -= 1.0;

			if(envelope < 0.5 && !envelopes[i].reset)
			{
				elements[i].freq = rand_in_range_i(1, spiroOptions.element_freq_max);
				if(rand()&1)
					elements[i].freq *=-1;
				elements[i].phase_offset = rand_in_range_i(0, spiroOptions.base_resolution);
			};
			envelopes[i].reset = envelope < 0.5;
			elements[i].amplitude = raised_cosine(envelope);
			i++;
		};

}
