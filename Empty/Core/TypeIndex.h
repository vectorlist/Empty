#pragma once

#include <core/type.h>
#include <typeindex>
#include <type_traits>

struct TypeIndex
{
	//Get Unique Id for Class
	template<typename T>
	static std::type_index GetTypeIndex();
};

//get hashcode for unordered_map
template<typename T>
inline std::type_index TypeIndex::GetTypeIndex()
{
	return std::type_index(typeid(T));
}

struct ComponentTypeIndex
{
	inline static uint GetUniqueId() noexcept
	{
		static uint lastID = 0;
		return lastID++;
	}

	template<typename T>
	inline static uint GetId() noexcept
	{
		static_assert(std::is_base_of<Component, T>::value, "invalid component type");
		static uint indexid = GetUniqueId();
		return indexid;
	}
};