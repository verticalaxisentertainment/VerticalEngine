#pragma once

#include "Layer.h"
#include "Core.h"

class _API ImGUILayer :public Layer
{
public:
	ImGUILayer();
	~ImGUILayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Event& e) override;

	void Begin();
	void End();
	inline static bool show = false;
private:
	float m_Time;
};
