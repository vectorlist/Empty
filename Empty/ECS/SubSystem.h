#pragma once

#include <core/typeindex.h>

class SubSystem
{
public:
	SubSystem() = default;
	SubSystem(const SubSystem&) = delete;		//delete copy op
	SubSystem& operator=(const SubSystem&) = delete;
	virtual ~SubSystem() {}

	virtual bool Initialize() { return true; };
	virtual void Release() { /*do nothing here*/ };
};

struct SubSystemContext
{
	template<typename T, typename ...Args>
	T* AddSubSystem(Args... args);

	template<typename T>
	T* GetSubSystem();

	template<typename T>
	bool HasSubSystem() const;

	std::unordered_map<uint, std::shared_ptr<SubSystem>> mSubSystem;
};

template<typename T, typename ...Args>
inline T* SubSystemContext::AddSubSystem(Args... args)
{
	uint hash = (uint)TypeIndex::GetTypeIndex<T>().hash_code();

	static_assert(std::is_base_of<SubSystem, T>::value, "invalid system type");

	assert(!HasSubSystem<T>());

	auto system = std::make_shared<T>(std::forward<Args>(args)...);
	assert(system->Initialize() && "failed to init subsystem");

	mSubSystem.insert(std::make_pair(hash, system));

	return static_cast<T*>(system.get());
}

template<typename T>
inline T* SubSystemContext::GetSubSystem()
{
	uint hash = TypeIndex::GetTypeIndex<T>().hash_code();

	if(!HasSubSystem<T>()) {
		assert(0 && "failed to find subsystem");
	}
	return static_cast<T*>(mSubSystem[hash].get());
}

template<typename T>
inline bool SubSystemContext::HasSubSystem() const
{
	uint hash = TypeIndex::GetTypeIndex<T>().hash_code();
	return (mSubSystem.find(hash) != mSubSystem.end());
}

namespace ECS
{
SubSystemContext&		Context();
} 

template<typename T, typename ...Args>
inline T* AddSubSystem(Args&&... args)
{
	return ECS::Context().AddSubSystem<T>(std::forward<Args>(args)...);
}

template<typename T>
inline T* GetSubSystem()
{
	return ECS::Context().GetSubSystem<T>();
}


