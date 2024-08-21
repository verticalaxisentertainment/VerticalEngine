#pragma once

#include "Layer/Layer.h"
#include "DebugLayer.h"

class ActiveSceneLayer :public Layer
{
public:
	ActiveSceneLayer()
		:Layer("ActiveSceneLayer"){}

	void OnAttach()
	{

	}

	void OnUpdate()
	{
		DebugLayer::GetActiveScene()->OnUpdate();
	}
};
