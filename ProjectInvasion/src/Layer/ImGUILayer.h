#pragma once

#include "Layer.h"

class ImGUILayer:public Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;
private:

};
