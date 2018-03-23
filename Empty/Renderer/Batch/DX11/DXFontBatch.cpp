#include <PCH.h>
#include <renderer/batch/DX11/DXFontBatch.h>

#include <cache/shadercache.h>
#include <Cache/buffercache.h>

#include <ft2build.h>
#include <freetype/freetype.h>

#include <Graphics/Context.h>
#include <Graphics/DX11/DXContext.h>

DXFontBatch::DXFontBatch()
	: mIsBegin(false), mCurrentVtxIndexed(0), mCurrentVtxPtr(nullptr)
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
	GContext->GetViewport(v);

	Matrix4x4 screen = Matrix4x4::NDCToScreenZeroToOne(v.w, v.h);

	mUbo->Update(&screen);

	CreateFontAndTexture(filename, size);
	CreateBuffers();

	mVertices = std::unique_ptr<FontInstanceVertex[]>(new FontInstanceVertex[FONT_MAX_BATCH]);
}

void DXFontBatch::BeginBatch()
{
	mIsBegin = true;
	mCurrentVtxIndexed = 0;
	//reset current pointer
	mCurrentVtxPtr = mVertices.get();
}

void DXFontBatch::Render(int x, int y, const char * text, const vec4f& color)
{
	if (mIsBegin == false) return;
	//Increate Current Vertice Index
	const uint8* c;

	for (c = (const uint8*)text; *c; ++c) 
	{
		//check out of bound
		if (mCurrentVtxIndexed >= FONT_MAX_BATCH) break;
		auto& font = fonts[*c];

		float posx = x + font.bl;
		float posy = (y - (font.h - font.bt)) + mHeight;

		float w = (float)font.w;
		float h = (float)font.h;

		//Increase offset of sizeof(FontInstanceVertex)
		*mCurrentVtxPtr++ = {
			vec4f(posx, posy, w, h),
			vec4f(font.uvx1, font.uvy1, font.uvx2, font.uvy2),
			color};

		mCurrentVtxIndexed++;
		x += font.ax;
	}
}

void DXFontBatch::EndBatch()
{

	if (mIsBegin == false || mCurrentVtxIndexed == 0) {
		return;
	}

	//TODO : Update Buffer
	D3D11_MAPPED_SUBRESOURCE mapped{};
	G_DXContext->Map(mVbo, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, mVertices.get(), sizeof(FontInstanceVertex) * mCurrentVtxIndexed);
	G_DXContext->Unmap(mVbo, 0);

	mShader->Bind();
	mUbo->BindVS(1);


	uint stride = sizeof(FontInstanceVertex);
	uint offset = 0;	
	G_DXContext->IASetVertexBuffers(0, 1, &mVbo, &stride, &offset);
	
	//Bind Texture and Sampler
	G_DXContext->PSSetShaderResources(0, 1, &mTexture);
	auto sampler = static_cast<DXContext*>(GContext)->state.samplerWarpState;
	G_DXContext->PSSetSamplers(0, 1, &sampler);
	
	G_DXContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GContext->SetBlendState(true);
	G_DXContext->DrawInstanced(6, mCurrentVtxIndexed, 0, 0);
	GContext->SetBlendState(false);
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

	uint width = 0;
	uint height = 0;

	//calc total font width height
	for (int c = FONT_MIN_CHAR; c < FONT_MAX_CHAR; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			continue;
		}
		width += glyph->bitmap.width;
		height = std::max(height, glyph->bitmap.rows);
	}

	mWidth = width;
	mHeight = height;

	//allocate buffer
	uint8* buffer = new uint8[width * height];
	
	memset(buffer, (uint8)0, sizeof(uint8) * width * height);
	memset(fonts, 0, sizeof(fonts));

	int offsetx = 0;
	int offsety = 0;
	for (int c = FONT_MIN_CHAR; c < FONT_MAX_CHAR; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			continue;
		}

		auto& bitmap = glyph->bitmap;
		//Set Buffer
		for (int y = 0; y < bitmap.rows; ++y) {
			for (int x = 0; x < bitmap.width; ++x) {
				buffer[(y * width) + (offsetx + x)] =
					bitmap.buffer[y * bitmap.width + x];
			}
		}

		fonts[c].ax = glyph->advance.x >> 6;
		fonts[c].ay = glyph->advance.y >> 6;

		fonts[c].w = (float)glyph->bitmap.width;
		fonts[c].h = (float)glyph->bitmap.rows;

		fonts[c].bl = (float)glyph->bitmap_left;
		fonts[c].bt = (float)((glyph->metrics.height - glyph->metrics.horiBearingY) >> 6);

		fonts[c].uvx1 = (float)offsetx / (float)mWidth;
		fonts[c].uvy1 = (float)offsety / (float)mHeight;

		fonts[c].uvx2 = (float)(offsetx + glyph->bitmap.width) / (float)mWidth;
		fonts[c].uvy2 = (float)(offsety + glyph->bitmap.rows) / (float)mHeight;

		offsetx += glyph->bitmap.width;
	}

	D3D11_TEXTURE2D_DESC textureInfo{};
	textureInfo.Width = width;
	textureInfo.Height = height;
	textureInfo.MipLevels = 1;
	textureInfo.ArraySize = 1;
	textureInfo.Format = DXGI_FORMAT_R8_UNORM;
	textureInfo.SampleDesc.Count = 1;
	textureInfo.SampleDesc.Quality = 0;
	textureInfo.Usage = D3D11_USAGE_DEFAULT;
	textureInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureInfo.CPUAccessFlags = 0;
	textureInfo.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = buffer;
	data.SysMemPitch = width;
	data.SysMemSlicePitch = width * height;

	ID3D11Texture2D* texture2d;

	LOG_HR << G_DXDevice->CreateTexture2D(&textureInfo, &data, &texture2d);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvInfo{};

	srvInfo.Format = textureInfo.Format;
	srvInfo.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvInfo.Texture2D.MipLevels = 1;
	srvInfo.Texture2D.MostDetailedMip = 0;

	LOG_HR << G_DXDevice->CreateShaderResourceView(texture2d, &srvInfo, &mTexture);

	SAFE_RELEASE(texture2d);
	FT_Done_Face(face);
	FT_Done_FreeType(lib);
	delete[] buffer;
}

void DXFontBatch::CreateBuffers()
{
	D3D11_BUFFER_DESC vtxInfo{};
	vtxInfo.Usage = D3D11_USAGE_DYNAMIC;
	vtxInfo.ByteWidth = sizeof(FontInstanceVertex) * FONT_MAX_BATCH;
	vtxInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vtxInfo.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vtxInfo.MiscFlags = 0;
	vtxInfo.StructureByteStride = 0;

	LOG_HR << G_DXDevice->CreateBuffer(&vtxInfo, nullptr, &mVbo);
}
