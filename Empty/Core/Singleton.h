#pragma once

#include <memory>

template<typename T>
class Singleton
{
public:
	Singleton(){}
	~Singleton(){}

	static T* GetInstance();
	static void Release();
protected:
	static std::shared_ptr<T> mInstance;
};

template<typename T>
std::shared_ptr<T> Singleton<T>::mInstance = nullptr;

template<typename T>
inline T* Singleton<T>::GetInstance()
{
	if (!mInstance) {
		mInstance = std::make_shared<T>();
	}
	return static_cast<T*>(mInstance.get());
}

template<typename T>
inline void Singleton<T>::Release()
{
	if (mInstance) {
		mInstance.reset();
	}
	mInstance = nullptr;
}
