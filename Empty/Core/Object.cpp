#include <Core/PCH.h>

#include <core/object.h>
#include <graphics/texture.h>
#include <graphics/model.h>


uint ObjectRef::mCounter = 0;

Object::Object()
	: mTexture(nullptr), mModel(nullptr)
{
	mId = mCounter++;
}

void Object::Render()
{
	mTexture->Bind(0);
	mModel->Render();
}

uint Object::GetId() const
{
	return mId;
}

AABB Object::GetWorldAABB()
{
	AABB& aabb = mModel->mAABB;
	return aabb.GetTransformedAABB(this->GetGlobal());
}

AABB* Object::GetLocalAABB()
{
	return &mModel->mAABB;
}
