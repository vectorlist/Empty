#pragma once

#include <math/matrix4x4.h>
#include <math/quaternion.h>

class Camera;
class Transform
{
public:
	Transform();

	void SetLocalPosition(float x, float y, float z);		
	void SetLocalPosition(const vec3f& pos);

	void SetWorldPosition(float x, float y, float z);
	void SetWorldPosition(const vec3f& pos);

	void MoveDirection(float x, float y, float z);
	void MoveDirection(const vec3f& pos);

	void SetRotation(float x, float y, float z);
	void SetRotation(const Quaternion& quat);

	void Rotate(float ax, float ay, float az);

	vec3f	GetPostion() const;
	vec3f*	GetAxis() { return mAxis; }

	Matrix4x4& GetLocal();
	Matrix4x4& GetGlobal();
	//virtual enable to change by derived
	virtual void Update();
	friend class Camera;	//allow to access private for cmera
	//TRANSFORM NODE
	void		SetParent(Transform* parent);
	void		AddChild(Transform* child);
	void		RemoveParent();

	Transform*		mParent;
	Transform*		mChild;
	Transform*		mNext;
private:
	union{
	struct{ vec3f mAxis[3];};
	struct{ vec3f mRight; vec3f mUp; vec3f mForward;};
	};
	static vec3f	sAxis[3];

	vec3f			mPosition;
	vec3f			mScale;
	Matrix4x4		mLocal;
	Matrix4x4		mGlobal;		

	bool			mAutoUpdate;
};