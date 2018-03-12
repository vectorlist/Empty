#pragma once

struct FontGlyph
{
	float ax;
	float ay;

	float w;
	float h;

	float bl;
	float bt;

	float uvx1;
	float uvy1;

	float uvx2;
	float uvy2;

	float offsety;
};

class Font final
{
public:
	Font();
	void Init(const char* filename, int size);
	uint width;
	uint height;
	FontGlyph caractor[FONT_MAX_CHAR];
	Texture* texture;
	//ref charactor indirect by index
	FontGlyph& operator[](uint index);
};

inline FontGlyph& Font::operator[](uint index) 
{
	return this->caractor[index];
}