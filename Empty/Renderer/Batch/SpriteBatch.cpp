#include <Core/PCH.h>
#include <Renderer/Batch/SpriteBatch.h>
#include <Cache/BufferCache.h>
#include <Cache/TextureCache.h>
#include <Cache/ShaderCache.h>

#include <Graphics/Context.h>

SpriteBatch* SpriteBatch::globalSpriteBatch = nullptr;

SpriteBatch::SpriteBatch()
	: mIsBegin(false), mSortType(TEXTURE), mQueuedIndexed(0)
{
	if (globalSpriteBatch != nullptr) {
		ASSERT_MSG(0, "Invalid Multiple Sprite Batches");
	}
	globalSpriteBatch = this;
	this->CreateBuffers();
}

SpriteBatch::~SpriteBatch()
{
}

//Add Sprite to Queue Sprite Container
void SpriteBatch::Render(float x, float y, float w, float h, Texture* texture)
{
	if (!mIsBegin || mQueuedIndexed >= SPRITE_MAX) return;

	mQueuedSprites[mQueuedIndexed].texture = texture;
	mQueuedSprites[mQueuedIndexed].LT = { vec3f(    x,     y, 0.0f), vec2f(0.0f, 0.0f) };
	mQueuedSprites[mQueuedIndexed].RT = { vec3f(x + w,     y, 0.0f), vec2f(1.0f, 0.0f) };
	mQueuedSprites[mQueuedIndexed].RB = { vec3f(    x, y + h, 0.0f), vec2f(0.0f, 1.0f) };
	mQueuedSprites[mQueuedIndexed].LB = { vec3f(x + w, y + h, 0.0f), vec2f(1.0f, 1.0f) };
	mQueuedIndexed++;
}

void SpriteBatch::BeginBatch(SortType type)
{
	//Check Begin
	mIsBegin = true;
	mQueuedIndexed = 0;
	//Clear Sorted Sprites
	mSortedSprites.clear();
	//Clear Batches
	mBatches.clear();
}

void SpriteBatch::EndBatch()
{
	if (mIsBegin == false || mQueuedIndexed == 0) return;
	//we dont need update UBO

	//Copy Queued sprites To Sort Sprites
	mSortedSprites.resize(mQueuedIndexed);
	for (uint i = 0; i < mQueuedIndexed; ++i) {
		mSortedSprites[i] = &mQueuedSprites[i];
	}

	//SORT SPRITES
	this->SortSprites();

	//CREATE BATCH INSTANCE AND UPLOAD TO GPU
	this->CreateBatches();

	//FINALY RENDER BATCHES
	this->RenderBatch();
	mIsBegin = false;
}

void SpriteBatch::CreateBuffers()
{
	//------------- Create Vertex Buffer ----------------
	std::vector<VertexAttrib> attribs = {
		{ 0, VertexFormat::FLOAT, 3, sizeof(VertexPS), offsetof(VertexPS, VertexPS::pos)},
		{ 0, VertexFormat::FLOAT, 2, sizeof(VertexPS), offsetof(VertexPS, VertexPS::st) }
	};

	VertexBufferCreateInfo vertexInfo{};
	vertexInfo.Type = BufferType::BUFFER_DYNAMIC;
	vertexInfo.pVertices = nullptr;
	vertexInfo.VerticesSize = sizeof(VertexPS) * 4 * SPRITE_MAX;
	vertexInfo.pAttrib = attribs.data();
	vertexInfo.AttribSize = attribs.size();

	mVertexBuffer = BufferCache::CreateVertexBuffer(&vertexInfo);
	
	//------------- Create Index Buffer -----------------
	uint indexPerSprite[6] = { 0, 1, 2, 2, 1, 3 };
	std::vector<uint[6]> indices(SPRITE_MAX);

	for (uint i = 0; i < (uint)indices.size(); ++i) 
	{
		for (uint j = 0; j < 6; ++j)
		{
			indices[i][j] = indexPerSprite[j] + i * 4;
		}
	}
	IndexBufferCreateInfo indexInfo{};
	indexInfo.IndicesSize = sizeof(uint) * 6 * indices.size();
	indexInfo.pIndices = indices.data();

	mIndexBuffer = BufferCache::CreateIndexBuffer(&indexInfo);

	//-------------- Create Uniform Buffer ------------------

	Viewport vp{};
	GContext->GetViewport(vp);
	Matrix4x4 screen = Matrix4x4::NDCToScreenZeroToOne(vp.w, vp.h);

	BufferCreateInfo bufferInfo{};
	bufferInfo.type = BufferType::BUFFER_DYNAMIC;
	bufferInfo.binding = BufferBinding::BIND_UNIFORM;
	bufferInfo.pData = screen.data();
	bufferInfo.size = sizeof(Matrix4x4);
	
	mUBO = BufferCache::CreateUniformBuffer(&bufferInfo);
	mUBO->Update(screen.data());
	//---------------- Create Shader ------------------------

	mSpriteShader = ShaderCache::CreateShader("SpriteVS", "SpritePS");

	//--------------- Allocate Max Sprites ------------------

	mQueuedSprites = std::unique_ptr<Sprite[]>(new Sprite[SPRITE_MAX]);
}

void SpriteBatch::CreateBatches()
{
	if (mSortedSprites.empty()) return;

	std::vector<VertexPS> vertices;

	//Allocate upload vertex with sorted sprites
	vertices.resize(mSortedSprites.size() * 4);

	uint currentVtx = 0;
	uint offset = 0;

	vertices[currentVtx++] = mSortedSprites[0]->LT;
	vertices[currentVtx++] = mSortedSprites[0]->RT;
	vertices[currentVtx++] = mSortedSprites[0]->RB;
	vertices[currentVtx++] = mSortedSprites[0]->LB;

	//push back first of batch
	mBatches.emplace_back(offset, 6, mSortedSprites[0]->texture);

	//increase offset(vertex) for first
	offset += 4;

	//previous texture
	Texture* preTexture = nullptr;

	for (uint index = 1; index < mSortedSprites.size(); ++index)
	{
		//conditional texure
		preTexture = mSortedSprites[index - 1]->texture;
		if (mSortedSprites[index]->texture != preTexture) {
			// if not match pre texture
			mBatches.emplace_back(offset, 6, mSortedSprites[index]->texture);
		}
		else {
			mBatches.back().verticesCount += 6;
		}

		vertices[currentVtx++] = mSortedSprites[index]->LT;
		vertices[currentVtx++] = mSortedSprites[index]->RT;
		vertices[currentVtx++] = mSortedSprites[index]->RB;
		vertices[currentVtx++] = mSortedSprites[index]->LB;
		offset += 4;
	}

	//Upload to GPU
	mVertexBuffer->SubData(0, sizeof(VertexPS) * vertices.size(), vertices.data());
}

void SpriteBatch::RenderBatch()
{
	GContext->SetDepthStencil(false);
	mSpriteShader->Bind();
	mUBO->BindVS(1);				//screen coords uniform buffer gl(binding = 1) dx(b1)
	mVertexBuffer->Bind();
	mIndexBuffer->Bind();
	//TODO : Bind Texture

	for (uint i = 0; i < mBatches.size(); ++i)
	{
		uint offset = mBatches[i].offset;
		uint nVertices = mBatches[i].verticesCount;
		if (mBatches[i].texture) {
			mBatches[i].texture->Bind(0);
		}

		mIndexBuffer->DrawIndexedBaseVertex(Topology::TRIANGLES, nVertices, 0, offset);
	}

	mVertexBuffer->UnBind();
	mIndexBuffer->UnBind();
	GContext->SetDepthStencil(true);
}



bool ConditionSprite(Sprite* a, Sprite* b);

void SpriteBatch::SortSprites()
{
	std::stable_sort(mSortedSprites.begin(), mSortedSprites.end(), ConditionSprite);
}

bool ConditionSprite(Sprite* a, Sprite* b)
{

	return (a->texture > b->texture);
}
