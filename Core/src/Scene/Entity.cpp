#include "pch.h"
#include "Entity.h"

#include "Renderer/UIRenderer.h"
#include "Input.h"

Entity::Entity(entt::entity handle, Scene* scene)
	:m_EntityHandle(handle), m_Scene(scene)
{
}

bool Entity::IsHovered()
{
	if (UIRenderer::GetPickedID() == this->GetUUID().id())
	{
		UIRenderer::m_onUI = true;
		return true;
	}
	else
		UIRenderer::m_onUI = false;


	return false;
}

bool Entity::IsClicked()
{
	if (IsHovered() && Input::IsMouseButtonPressed(Mouse::Button0))
		return true;
	return false;
}
