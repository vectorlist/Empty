#pragma once

#include <core/type.h>
#include <math/transform.h>
#include <math/aabb.h>

class ObjectRef
{
	friend class Object;
	uint mId = 0;
	static uint mCounter;
};

class Object : public Transform, ObjectRef
{
public:
	Object();

	void	Render();
	uint	GetId() const;
	AABB	GetWorldAABB();
	AABB*	GetLocalAABB();

	Texture*	mTexture;
	Model*		mModel;
};



