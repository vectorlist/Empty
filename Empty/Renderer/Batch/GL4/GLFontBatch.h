#pragma once

#include <renderer/batch/FontBatch.h>

class GLFontBatch : public FontBatch
{
public:
	GLFontBatch();
	virtual~GLFontBatch();

	//init
	void Init(const char* filename, int size)override;

	//Start Batch
	void BeginBatch() override;

	//Gather Item
	void Render(int x, int y, const char* text, const vec4f& color = vec4f(1)) override;

	//Render Batch
	void EndBatch()override;

	//Create Font
	void CreateFontAndTexture(const char* filename, int size);
	//Create instancing Buffer
	void CreateBuffers();
	//============ Font Texture ==================
	uint texture;
	uint mWidth;
	uint mHeight;

	FontGlyph fonts[MAX_CHARACTOR];
private:
	Shader* mShader;
	
	UniformBuffer* mUbo;
	//Font mFont;

	uint mVao;
	uint mVbo;

	//pointer arrays
	std::unique_ptr<FontInstanceVertex[]> mVertices;
	//current arrays indexed
	uint mCurrentVtxIndexed;
	//current pointer of vertices position;
	FontInstanceVertex* mCurrentVtxPtr;

	bool mIsBegin;
};