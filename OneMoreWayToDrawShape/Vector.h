#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include "TextTools.h"

using namespace sf;

class Vector
{
public:
	Vector(){}

	Vector(Font *font, float x0 = 0, float y0 = 0, float x1 = 0, float y1 = 1): x0(x0), y0(y0), x1(x1), y1(y1){ 
		v_order++;
		id = v_order;
		angle = 0;
		show_next_line = false;

		sf_line = VertexArray(Lines, 2);
		sf_line.append(Vertex(Vector2f(x0, y0)));
		sf_line.append(Vertex(Vector2f(x1, y1)));
		sf_line[2].color = Color::Green;
		sf_line[3].color = Color::Green;
		
		nx0 = -1000, ny0 = y1, nx1 = 1000, ny1 = y1;
		next_length = sqrtf(powf((nx1 - x0), 2) + powf((ny1 - y0), 2));
		sf_next_line = VertexArray(Lines, 2);
		sf_next_line.append(Vertex(Vector2f(nx0, ny0)));
		sf_next_line.append(Vertex(Vector2f(nx1, ny1)));
		sf_next_line[2].color = Color::Blue;
		sf_next_line[3].color = Color::Blue;

		text = Text(std::to_string(id), *font, 24);
		text.setFillColor(Color::Red);
		text.setPosition(Vector2f(((x1 - x0) / 2 + x0), ((y1 - y0) / 2) + y0));

		setAngle(0);
	}

	~Vector() {

	}

	void rotate(float angle) {
		float length = getLength();		
		this->angle += angle;
		this->angle = fmod(this->angle, 360);
		float tx = x1 - x0;
		float ty = y1 - y0;
		float cx = tx;
		float cy = ty;

		float teta = angle * M_PI / 180.f;

		if (teta != 0) {
			tx = cx * cos(teta) - cy * sin(teta);
			ty = cx * sin(teta) + cy * cos(teta);

			x1 = tx + x0;
			y1 = ty + y0;
		}else {
			teta = this->angle * M_PI / 180.f;
			tx = cx = 0;
			ty = cy = length;

			tx = cx * cos(teta) - cy * sin(teta);
			ty = cx * sin(teta) + cy * cos(teta);

			x1 = tx + x0;
			y1 = ty + y0;
		}
		resize(length);
	}

	void resize(float value) {
		float length = getLength();
		float tx = x1 - x0;
		float ty = y1 - y0;

		float cx = tx / length, cy = ty / length;

		tx = cx * value;
		ty = cy * value;

		x1 = tx + x0;
		y1 = ty + y0;

		nx0 = -cy * next_length + x1;
		ny0 = cx * next_length + y1;
		nx1 = cy * next_length + x1;
		ny1 = -cx * next_length + y1;

		sf_line[2] = this->getStart();
		sf_line[3] = this->getEnd();		
		sf_line[2].color = Color::Green;
		sf_line[3].color = Color::Green;

		sf_next_line[2] = Vector2f(nx0, ny0);
		sf_next_line[3] = Vector2f(nx1, ny1);
		sf_next_line[2].color = Color::Blue;
		sf_next_line[3].color = Color::Blue;

		text.setPosition(this->getCenter());
	}

	Vector2f intersection(Vector another) {
		float length = getLength();
		float tx = x1 - x0;
		float ty = y1 - y0;

		float cx = tx / length, cy = ty / length;
		float ncx0 = -cy, ncy0 = cx;
		float ncx1 = cy, ncy1 = -cx;

		return Vector2f();
	}

	void setAngle(float angel) {
		this->angle = angel;
		rotate(0);
	}

	void setShowNextLine(bool v) {
		show_next_line = v;
	}

	int getID() {
		return id;
	}

	float getAngle() {
		return angle;
	}

	float getLength() {
		return sqrtf(powf((x1 - x0), 2) + powf((y1 - y0), 2));
	}

	bool getShowNextLine() {
		return show_next_line;
	}

	void draw(RenderWindow& window) {
		window.draw(sf_line);
		window.draw(text);

		if (show_next_line)
			window.draw(sf_next_line);
	}

	Vector2f getCenter() {
		return Vector2f(((x1 - x0) / 2 + x0), ((y1 - y0) / 2) + y0);
	}

	Vector2f getStart() {
		return Vector2f(x0, y0);
	}

	Vector2f getEnd() {
		return Vector2f(x1, y1);
	}

private:
	float x0, y0, x1, y1;
	float nx0, ny0, nx1, ny1;
	static int v_order;
	static float next_length;
	int id;
	float angle;

	VertexArray sf_line;
	VertexArray sf_next_line;
	Text text;
	bool show_next_line;
};

int Vector::v_order = 0;
float Vector::next_length = 1000;
