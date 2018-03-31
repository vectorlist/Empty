#pragma once

#include <Math/AABB.h>
#include <Core/Type.h>

//Render Able Tree Node
struct TreeNode
{
	uint mLevel;
	AABB mAABB;
	virtual bool IsLeaf() const = 0;
	virtual void Render() = 0;
};