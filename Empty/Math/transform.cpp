#include <Core/PCH.h>
#include "transform.h"

vec3f Transform::sAxis[3] = {vec3f(1,0,0), vec3f(0,1,0), vec3f(0,0,1)};

Transform::Transform()
	: mParent(nullptr), mChild(nullptr), mNext(nullptr), mAutoUpdate(true)
{
	mAxis[0] = sAxis[0];
	mAxis[1] = sAxis[1];
	mAxis[2] = sAxis[2];

	mScale = vec3f(1.0f);

	Update();
}

void Transform::SetLocalPosition(float x, float y, float z)
{
	mPosition.x = x;
	mPosition.y = y;
	mPosition.z = z;
	Update();
}

void Transform::SetLocalPosition(const vec3f &pos)
{
	mPosition.x = pos.x;
	mPosition.y = pos.y;
	mPosition.z = pos.z;
	Update();
}

void Transform::SetWorldPosition(float x, float y, float z)
{
	vec3f pos(x, y, z);
	if (mParent) {
		pos = mParent->GetGlobal().Inverted() * pos;
	}
	mPosition.x = pos.x;
	mPosition.y = pos.y;
	mPosition.z = pos.z;
	Update();
}

void Transform::SetWorldPosition(const vec3f& pos)
{
	this->SetWorldPosition(pos.x, pos.y, pos.z);
}

void Transform::MoveDirection(float x, float y, float z)
{
	//move postion with axises
	vec3f delta;
	delta += mAxis[0] * x;			//by right
	delta += mAxis[1] * y;
	delta += mAxis[2] * z;
	//current pos + delta position then internal update
	SetWorldPosition(mPosition + delta);
}

void Transform::MoveDirection(const vec3f& pos)
{
	vec3f delta;
	delta += mAxis[0] * pos.x;			
	delta += mAxis[1] * pos.y;
	delta += mAxis[2] * pos.z;
	//current pos + delta position then internal update
	SetWorldPosition(mPosition + delta);
}

void Transform::SetRotation(float x, float y, float z)
{
	Quaternion qRot = Quaternion::RotationYawPitchRoll(y, x, z);

	Matrix4x4 rot = qRot.GetMatrix();

	for (int i = 0; i < 3; ++i){
		mAxis[i] = rot.Normal(sAxis[i]);
	}
	Update();
}

void Transform::SetRotation(const Quaternion& quat)
{
	Matrix4x4 rot = quat.GetMatrix();

	for (int i = 0; i < 3; ++i) {
		mAxis[i] = rot.Normal(sAxis[i]);
	}
	Update();
}

void Transform::Rotate(float ax, float ay, float az)
{
	Matrix4x4 rotx = Matrix4x4::RotationAxis(mAxis[0], ax);
	Matrix4x4 roty = Matrix4x4::RotationAxis(mAxis[1], ay);
	Matrix4x4 rotz = Matrix4x4::RotationAxis(mAxis[2], az);

	Matrix4x4 rot = roty * rotx * rotz;

	for (int i = 0; i < 3; ++i) {
		mAxis[i] = rot.Normal(mAxis[i]);
	}
	Update();
}

vec3f Transform::GetPostion() const
{
	return mPosition;
}

Matrix4x4& Transform::GetLocal() 
{
	return mLocal;
}

Matrix4x4& Transform::GetGlobal()
{
	return mGlobal;
}

void Transform::SetParent(Transform* parent)
{
	parent->AddChild(this);
}

void Transform::AddChild(Transform* newChild)
{
	//check previous parent
	if (newChild->mParent == this) {
		//same parent?
		return;
	}
	//remove parent if we have
	RemoveParent();

	//i am camera

	//get parent(this is parent) invese matrix
	Matrix4x4 invGlobal = this->mGlobal.Inverted();

	//transform  3 axis (Normal func has normalize already)
	for (int i = 0; i < 3; ++i) {
		newChild->GetAxis()[i] = invGlobal.Normal(newChild->GetAxis()[i]);
	}

	//im ur parent
	newChild->mParent = this;

	//set child
	Transform* pChild = this->mChild;

	//add child
	if (pChild == nullptr)
	{
		this->mChild = newChild;
		newChild->mParent = this;
	}
	else 
	{
		//sorry i have child already(u will next child)
		while (pChild != nullptr)
		{
			pChild->mNext = newChild;
			newChild->mParent = this;
			break;
		}
		pChild = pChild->mNext;
	}
	Update();
}

//remove parent
void Transform::RemoveParent()
{
	if (mParent == nullptr) return;
	//yes we have parent

	//remove all child before remove parent
	Transform* pChild = mParent->mChild;

	//check child
	//am i ur first child?
	if (pChild == this) {
		//then my next child to be first child
		mParent->mChild = this->mNext;
		//next brother is null
		this->mNext = nullptr;
	}
	else {
		while (pChild != nullptr)
		{
			//am i ur next child?
			if (pChild->mNext == this) {
				pChild->mChild = this->mNext;
				this->mNext = nullptr;
				break;
			}
			pChild = pChild->mNext;
		}
	}

	//let remove parent
	this->mParent = nullptr;

	//recalc all matrices
	//x,y,z
	mPosition = { mGlobal[0][3],mGlobal[1][3] ,mGlobal[2][3] };

	//mLocal = {
	//	mRight.x, mUp.x, mForward.x, mPosition.x,
	//	mRight.y, mUp.y, mForward.y, mPosition.y,
	//	mRight.z, mUp.z, mForward.z, mPosition.z,
	//	0.f,0.f,0.f,1.f
	//};

	vec3f right =	vec3f(mGlobal[0][0], mGlobal[1][0], mGlobal[2][0]).normalized();
	vec3f up =		vec3f(mGlobal[0][1], mGlobal[1][1], mGlobal[2][1]).normalized();
	vec3f forward = vec3f(mGlobal[0][2], mGlobal[1][2], mGlobal[2][2]).normalized();

	mAxis[0] = right;
	mAxis[1] = right;
	mAxis[2] = right;

	Update();
}

void Transform::Update()
{
	if (!mAutoUpdate) return;

	mLocal = {
		mRight.x * mScale.x, mUp.x * mScale.y, mForward.x * mScale.z, mPosition.x,
		mRight.y * mScale.x, mUp.y * mScale.y, mForward.y * mScale.z, mPosition.y,
		mRight.z * mScale.x, mUp.z * mScale.y, mForward.z * mScale.z, mPosition.z,
		0.f,0.f,0.f,1.f
	};

	//Local is Wolrd if Parent is null
	if (mParent == nullptr) {
		mGlobal = mLocal;
	}

	if (mParent) {
		//not sure mult order
		this->mGlobal = this->mLocal * mParent->GetGlobal();
	}

	//CHILD UPDATE

	Transform* child = this->mChild;

	while (child != nullptr)
	{
		child->Update();
		child = this->mNext;
	}

}