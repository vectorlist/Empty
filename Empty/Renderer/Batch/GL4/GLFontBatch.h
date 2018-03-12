#pragma once

#include <renderer/batch/FontBatch.h>

class GLFontBatch : public FontBatch
{
public:
	GLFontBatch();
	virtual~GLFontBatch();

	void Init(const char* filename, int size)override;
	void BeginBatch() override;
	void Render(int x, int y, const char* text, const vec4f& color = vec4f(1)) override;
	void EndBatch()override;

	//Create Font
	void CreateFontAndTexture(const char* filename, int size);
	//Create instancing Buffer
	void CreateBuffers();
	//============ Font Texture ==================
	uint texture;
	uint mWidth;
	uint mHeight;

	FontGlyph fonts[FONT_MAX_CHAR];
private:
	Shader* mShader;
	UniformBuffer* mUbo;

	uint mVao;
	uint mVbo;

	std::unique_ptr<FontInstanceVertex[]> mVertices;
	uint mCurrentVtxIndexed;
	FontInstanceVertex* mCurrentVtxPtr;
	bool mIsBegin;
};