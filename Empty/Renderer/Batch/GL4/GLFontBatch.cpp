#include <PCH.h>
#include <renderer/batch/GL4/GLFontBatch.h>

#include <ft2build.h>
#include <freetype\freetype.h>

#include <cache/shadercache.h>
#include <cache/buffercache.h>
#include <graphics/context.h>

#include <Graphics/GL4/GLTexture.h>
#include <Graphics/GL4/GLConfig.h>

GLFontBatch::GLFontBatch()
	: mCurrentVtxIndexed(0), mCurrentVtxPtr(nullptr), mIsBegin(true)
{

}

GLFontBatch::~GLFontBatch()
{
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mVbo);
}

void GLFontBatch::Init(const char* filename, int size)
{
	//Font Shader
	mShader = ShaderCache::CreateFontInstanceShader();

	//Font Screen uniform Buffer
	BufferCreateInfo info{};
	info.pData = nullptr;
	info.size = sizeof(Matrix4x4);
	info.type = BufferType::BUFFER_DYNAMIC;
	info.binding = BufferBinding::BIND_UNIFORM;

	mUbo = BufferCache::CreateUniformBuffer(info);

	Viewport vp;
	G_Context->GetViewport(vp);

	Matrix4x4 screen = Matrix4x4::NDCToScreenZeroToOne(vp.w, vp.h);

	//UBO set's slot = 1 avoid main ubo
	mUbo->Update(screen.data());

	//Prepare Vertext array Pointer same as Buffer length (safe unique prt)
	mVertices = std::unique_ptr<FontInstanceVertex[]>(new FontInstanceVertex[FONT_MAX_BATCH]);

	CreateFontAndTexture(filename, size);
	CreateBuffers();
}

void GLFontBatch::BeginBatch()
{
	mIsBegin = true;
	//reset vtx indext
	mCurrentVtxIndexed = 0;
	//set ptr pos first of vertices point 
	mCurrentVtxPtr = mVertices.get();
}

void GLFontBatch::Render(int x, int y, const char* text, const vec4f& color)
{
	const uint8* c;
	//while c alive
	for (c = (const uint8*)text; *c; ++c) 
	{

		//check index out of max batch
		if (mCurrentVtxIndexed >= FONT_MAX_BATCH) break;
		//TODO : determine uv and pos

		//get charactor
		auto& font = fonts[*c];

		float posx = (x + font.bl);
		float posy = (y - (font.h + font.bt)) + mHeight;

		float w = font.w;
		float h = font.h;

		//increate ptr address (size of FontInstanceVertex)
		*mCurrentVtxPtr++ = {
			vec4f(posx, posy, w, h),
			vec4f(font.uvx1, font.uvy1, font.uvx2, font.uvy2),
			color};
		//increate index
		mCurrentVtxIndexed++;

		x += font.ax;
	}
}

void GLFontBatch::EndBatch()
{
	//do not batch without begin
	if (mIsBegin == false) return;
	//Update Buffer by sized
	//LOG_GR();
	//Bind Shader
	mShader->Bind();
	//Bind Texture
	glBindTextureUnit(0, texture);

	//Bind UBO
	mUbo->BindVS(1);		//avoid main uniform 0
	//Bind VBO
	uint instanceCounts = mCurrentVtxIndexed;
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(FontInstanceVertex) * instanceCounts,
		mVertices.get());

	G_Context->SetBlendState(true);
	//draw 6 index at once 0 1 2 2 1 3 6times
	glBindVertexArray(mVao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instanceCounts);
	glBindVertexArray(0);
	G_Context->SetBlendState(false);
	mShader->UnBind();

	//render Instance

	//check begin
}

void GLFontBatch::CreateFontAndTexture(const char * filename, int size)
{
	//TODO EACH FONT CLASS
	FT_Library lib;
	FT_Face face;

	if (FT_Init_FreeType(&lib)) {
		ASSERT_MSG(0, "failed to init ft lib");
	}
	if (FT_New_Face(lib, filename, 0, &face)) {
		ASSERT_MSG(0, "failed to load face");
	}

	FT_Set_Pixel_Sizes(face, 0, size);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
		ASSERT_MSG(0, "failed to load glyph");
	}

	FT_GlyphSlot glyph = face->glyph;

	uint rowWidth = 0;
	uint rowHeight = 0;

	//calc total font width height
	for (int c = FONT_MIN_CHAR; c < FONT_MAX_CHAR; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			continue;
		}
		rowWidth += glyph->bitmap.width;
		rowHeight = std::max(rowHeight, glyph->bitmap.rows);
	}

	mWidth = rowWidth;
	mHeight = rowHeight;

	//Create Texture

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, mWidth, mHeight, 0, GL_RED,
		GL_UNSIGNED_BYTE, NULL);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int offsetx = 0;
	int offsety = 0;

	memset(fonts, 0, sizeof(fonts));

	for (int c = FONT_MIN_CHAR; c < FONT_MAX_CHAR; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			continue;
		}
		glTexSubImage2D(GL_TEXTURE_2D, 0, offsetx, offsety, glyph->bitmap.width, glyph->bitmap.rows,
			GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

		fonts[c].ax = glyph->advance.x >> 6;
		fonts[c].ay = glyph->advance.y >> 6;

		fonts[c].w = (float)glyph->bitmap.width;
		fonts[c].h = (float)glyph->bitmap.rows;

		fonts[c].bl = (float)glyph->bitmap_left;
		fonts[c].bt = (float)((glyph->metrics.horiBearingY - glyph->metrics.height) >> 6);

		fonts[c].uvx1 = (float)offsetx / (float)mWidth;
		fonts[c].uvx2 = (float)(offsetx + glyph->bitmap.width) / (float)mWidth;

		fonts[c].uvy1 = (float)offsety / (float)mHeight;
		fonts[c].uvy2 = (float)(offsety + glyph->bitmap.rows) / (float)mHeight;

		rowHeight = std::max(rowHeight, glyph->bitmap.rows);
		offsetx += glyph->bitmap.width;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	//Assign Texture to Font texture
	
	//Release FT
	FT_Done_Face(face);
	FT_Done_FreeType(lib);
}

void GLFontBatch::CreateBuffers()
{
	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVbo);

	glBindVertexArray(mVao);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	//reserve max batch 65536
	glBufferData(GL_ARRAY_BUFFER, sizeof(FontInstanceVertex) * FONT_MAX_BATCH, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);	//pos
	glEnableVertexAttribArray(1);	//uv2
	glEnableVertexAttribArray(2);	//color

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FontInstanceVertex),
		(void*)(0));															//0
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(FontInstanceVertex),
		(void*)offsetof(FontInstanceVertex, FontInstanceVertex::uv));		//16
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(FontInstanceVertex),
		(void*)offsetof(FontInstanceVertex, FontInstanceVertex::color));	//32

	//draw at once 6time on gpu 
	glVertexAttribDivisor(0, 1);	//pass pwh at once
	glVertexAttribDivisor(1, 1);	//pass uv at once
	glVertexAttribDivisor(2, 1);	//pass color at once

	//array draw no index
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
