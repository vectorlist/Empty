#pragma once

#include <math/transform.h>

enum CameraType
{
	FirstPerson = 0,
	Perspective,
	GameObject
};

class Input;
class OrthoCamera;
class Camera : public Transform
{
public:
	Camera() : mFovy(0.0f), mAspect(0.0f), mZnear(0.0f), mZfar(0.0f) {}
	Camera(float fovy, float aspect, float nearplane = 0.1f, float farplane = 1000.f);
	virtual~Camera() {}

	Matrix4x4& GetView();
	Matrix4x4& GetProjection();
	CameraType GetType() const;

	float GetNear() const { return mZnear; }
	float GetFar() const { return mZfar; }
	float GetFov() const { return mFovy; }
	float GetAspect() const { return mAspect; }

	virtual void Update();

	void ProcessFirstPerson(Input* input, float delta);
	void ProcessMovement(Input* input);
protected:
	friend class OrthoCamera;
	void Init();

	Matrix4x4	mView;
	Matrix4x4	mProj;

	float		mZnear;
	float		mZfar;
	float		mFovy;
	float		mAspect;
	CameraType	mType;
};

class OrthoCamera : public Camera
{
public:
	OrthoCamera(int width, int height, float znear, float zfar);
	virtual~OrthoCamera() {}

protected:

};

