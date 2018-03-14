#pragma once

#include <World\Terrain.h>

class TerrainCache
{
public:
	static Terrain* CreateTerrain(const std::string& name, TerrainCreateInfo* info = nullptr);

private:
	static std::unordered_map<std::string, std::shared_ptr<Terrain>> mTerrains;
};