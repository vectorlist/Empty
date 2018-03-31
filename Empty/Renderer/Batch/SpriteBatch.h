#pragma once

#define SPRITE_MAX			8096

struct Sprite
{
	VertexPS LT;
	VertexPS RT;
	VertexPS RB;
	VertexPS LB;
	Texture* texture;
};

struct BatchItem
{
	BatchItem(uint offset, uint verticesCount, Texture* texture)
		: offset(offset), verticesCount(verticesCount), texture(texture)
	{}
	uint offset;
	uint verticesCount;
	Texture* texture;
};

class SpriteBatch
{
public:
	enum SortType { TEXTURE = 0U };
	SpriteBatch();
	~SpriteBatch();

	//Render (gather batches)
	void Render(float x, float y, float w, float h, Texture* texture = nullptr);
	void BeginBatch(SortType type = TEXTURE);
	void EndBatch();

	static SpriteBatch* globalSpriteBatch;
private:
	void CreateBuffers();
	void SortSprites();
	void CreateBatches();
	void RenderBatch();

	VertexBuffer*	mVertexBuffer;
	IndexBuffer*	mIndexBuffer;
	Shader*			mSpriteShader;
	UniformBuffer*	mUBO;
	Matrix4x4		mScreenSpace;

	bool			mIsBegin;
	SortType		mSortType;

	std::unique_ptr<Sprite[]>	mQueuedSprites;
	uint						mQueuedIndexed;

	std::vector<BatchItem>		mBatches;
	std::vector<Sprite*>		mSortedSprites;
};

#define GSpriteBatch	SpriteBatch::globalSpriteBatch