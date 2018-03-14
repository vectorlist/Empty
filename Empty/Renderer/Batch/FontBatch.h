#pragma once

#include <renderer/batch/Font.h>

struct FontInstanceVertex
{
	vec4f pwh;			//pos width height
	vec4f uv;				//uv1   uv2
	vec4f color;			//rgba
};

class FontBatch
{
public:
	FontBatch();
	virtual~FontBatch();

	//init
	virtual void Init(const char* filename, int size) = 0;

	//Start Batch
	virtual void BeginBatch() = 0;

	//Gather Item
	virtual void Render(int x, int y, const char* text, const vec4f& color) = 0;

	//Render Batch
	virtual void EndBatch() = 0;
	
	static FontBatch* globalFontBatch;
private:
	
};

#define G_FontBatch FontBatch::globalFontBatch