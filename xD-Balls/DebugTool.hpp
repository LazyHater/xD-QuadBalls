#pragma once
#include "Tool.hpp"
#include "World.hpp"

class DebugTool :
	public Tool
{
public:
	DebugTool(World* w);
	virtual ~DebugTool();
};

