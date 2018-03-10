#pragma once

#include <core/type.h>
#include <math/transform.h>
#include <math/aabb.h>

class ObjectRef
{
	//access to id
	friend class Object;
	uint mId = 0;
	static uint mCounter;
};

//TODO : Parent/ child system
class Object : public Transform, ObjectRef
{
public:
	Object();
	Texture* mTexture;
	Model* mModel;
	//test
	void Render();
	uint GetId() const;
	AABB GetWorldAABB();
	AABB* GetLocalAABB();
};



