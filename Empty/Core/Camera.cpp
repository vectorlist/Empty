#include <PCH.h>

#include <core/camera.h>
#include <graphics/context.h>
#include <core/input.h>
#include <Windows.h>

Camera::Camera(float fovy, float aspect, float nearplane, float farplane)
	: mFovy(fovy), mAspect(aspect), mZnear(nearplane), mZfar(farplane)
{
	Init();
}

Matrix4x4& Camera::GetView()
{
	return mView;
}

Matrix4x4& Camera::GetProjection()
{
	return mProj;
}

CameraType Camera::GetType() const
{
	return mType;
}

void Camera::Init()
{
	switch (G_Context->GetApiType())
	{
	case GraphicAPI::OPENGL45:
		mProj = Matrix4x4::PerspectiveDXLH(mFovy, mAspect, mZnear, mZfar);
		break;
	case GraphicAPI::DIRECTX11:
		mProj = Matrix4x4::PerspectiveDXLH(mFovy, mAspect, mZnear, mZfar);
		break;
	case GraphicAPI::VULKAN:
		mProj = Matrix4x4::VulkanClip() * Matrix4x4::PerspectiveLH(mFovy, mAspect, mZnear, mZfar);
		break;
	}
	
}

void Camera::Update()
{
	//update transform and invert matrix for view
	Transform::Update();
	mView = mGlobal.Inverted();
}

void Camera::ProcessFirstPerson(Input* input, float delta)
{
	static float yaw = 0.0f;
	static float pitch = 0.0f;
	static float mouseSpeed = 0.1999f;
	static float velocity = 0.1333;

	float mouseSensor = delta * 399.99;
	float moveSensor = delta * 9.33;

	if (input->IsKeyPressed(VK_LBUTTON))
	{
		vec2i mouseDelta = input->GetMouseDelta();

		yaw += mouseDelta.y * mouseSpeed;
		pitch += mouseDelta.x * mouseSpeed;

		yaw = math::clamp(yaw, -90.f, 90.f);
		this->SetRotation(yaw, pitch, 0.0f);
	}

	vec3f pos;

	if (input->IsKeyPressed('W')) {
		pos.z += moveSensor;
	}
	if (input->IsKeyPressed('S')) {
		pos.z -= moveSensor;
	}
	if (input->IsKeyPressed('A')) {
		pos.x -= moveSensor;
	}
	if (input->IsKeyPressed('D')) {
		pos.x += moveSensor;
	}
	if (abs(pos.x) > FLOAT_EPSILON || abs(pos.y) > FLOAT_EPSILON || abs(pos.z) > FLOAT_EPSILON) {
		this->MoveDirection(pos);			//Update -> to child
	}
}

void Camera::ProcessMovement(Input* input)
{
	switch (mType)
	{
	case FirstPerson:
		//firstPersonCameraProcess(input);
		break;
	case Perspective:
		break;
	}
}

OrthoCamera::OrthoCamera(int width, int height, float znear, float zfar)
{
	mZnear = znear;
	mZfar = zfar;
	mAspect = (float)width / (float)height;
	mProj = Matrix4x4::OrthoLH(width, height, znear, zfar);
}

