#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
using namespace sf;
class TextTools
{
public:
	TextTools(Font font) {
		this->font = font;
	}

	Text getTextFromString(std::string txt, int c_size, Color color, Text::Style style) {
		Text text(txt, font, c_size);
		text.setFillColor(color);
		text.setStyle(style);

		return text;
	}
	
	Text getTextFromString(std::string txt, int c_size, Color color) {
		Text text(txt, font, c_size);
		text.setFillColor(color);
		return text;
	}
private:
	Font font;
};

