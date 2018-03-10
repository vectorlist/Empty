#pragma once

#include <renderer/batch/FontBatch.h>

class DXFontBatch : public FontBatch
{
public:
	DXFontBatch();
	virtual~DXFontBatch();
	void Init(const char* filename, int size)override;
	void BeginBatch()override;
	void Render(int x, int y, const char* text, const vec4f& color)override;
	void EndBatch()override;

protected:
	void CreateFontAndTexture(const char* filename, int size);
	void CreateBuffers();

	//TODO : Font class {w,h, fonts, texture}
	FontGlyph fonts[MAX_CHARACTOR];
	uint mWidth;
	uint mHeight;
private:
	Shader* mShader;
	UniformBuffer* mUbo;
};