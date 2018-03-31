#include <Core/PCH.h>
#include <Cache/TerrainCache.h>

std::unordered_map<std::string, std::shared_ptr<Terrain>> TerrainCache::mTerrains;

Terrain* TerrainCache::CreateTerrain(const std::string& name, TerrainCreateInfo* info)
{
	std::shared_ptr<Terrain> terrain = nullptr;
	auto found = mTerrains.find(name);
	if (found == mTerrains.end())
	{
		terrain = std::make_shared<Terrain>();
		terrain->Init(*info);
		mTerrains.insert(std::make_pair(name, terrain));
		return terrain.get();
	}
	return found->second.get();
}
