#include <PCH.h>
#include <renderer/batch/DX11/DXFontBatch.h>

#include <cache/shadercache.h>
#include <Cache/buffercache.h>

#include <ft2build.h>
#include <freetype/freetype.h>

DXFontBatch::DXFontBatch()
{
}

DXFontBatch::~DXFontBatch()
{
}

void DXFontBatch::Init(const char* filename, int size)
{
	mShader = ShaderCache::CreateFontInstanceShader();

	BufferCreateInfo info{};
	info.type = BufferType::BUFFER_DYNAMIC;
	info.binding = BufferBinding::BIND_UNIFORM;
	info.pData = nullptr;
	info.size = sizeof(Matrix4x4);

	mUbo = BufferCache::CreateUniformBuffer(info);

	Viewport v;
	G_Context->GetViewport(v);

	Matrix4x4 screen = Matrix4x4::NDCToScreenZeroToOne(v.w, v.h);

	mUbo->Update(&screen);

	CreateFontAndTexture(filename, size);
	CreateBuffers();
}

void DXFontBatch::BeginBatch()
{
}

void DXFontBatch::Render(int x, int y, const char * text, const vec4f & color)
{
}

void DXFontBatch::EndBatch()
{
}

void DXFontBatch::CreateFontAndTexture(const char * filename, int size)
{
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
	for (int c = 32; c < MAX_CHARACTOR; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			continue;
		}
		rowWidth += glyph->bitmap.width;
		rowHeight = std::max(rowHeight, glyph->bitmap.rows);
	}

	mWidth = rowWidth;
	mHeight = rowHeight;

	//TODO : DirectX Texture


	int offsetx = 0;
	int offsety = 0;

	memset(fonts, 0, sizeof(fonts));

	for (int c = 32; c < MAX_CHARACTOR; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			continue;
		}

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

	//Release FT
	FT_Done_Face(face);
	FT_Done_FreeType(lib);
}

void DXFontBatch::CreateBuffers()
{
}
