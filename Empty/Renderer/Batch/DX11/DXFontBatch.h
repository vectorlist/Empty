#pragma once

#include <renderer/batch/FontBatch.h>

class DXFontBatch : public FontBatch
{
public:
	DXFontBatch();
	virtual~DXFontBatch();
	void Init(const char* filename, int size)override;
	void BeginBatch()override;
	void Render(int x, int y, const char* text, const vec4f& color = vec4f(1)) override;
	void EndBatch()override;

protected:
	void CreateFontAndTexture(const char* filename, int size);
	void CreateBuffers();

	//TODO : Font class {w,h, fonts, texture}
	FontGlyph fonts[FONT_MAX_CHAR];
	uint mWidth;
	uint mHeight;
	ID3D11ShaderResourceView*	mTexture;
	ID3D11Buffer*				mVbo;
private:
	Shader* mShader;
	UniformBuffer* mUbo;
	std::unique_ptr<FontInstanceVertex[]> mVertices;
	FontInstanceVertex* mCurrentVtxPtr;
	uint mCurrentVtxIndexed;
	bool mIsBegin;
};