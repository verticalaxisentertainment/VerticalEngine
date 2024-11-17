#pragma once
#include "Core.h"

class Entity;

template <class T>
class _API ComponentEvent
{
public:
	static void OnComponentChanged(Entity entity, T &component);
	static void OnComponentAdded(Entity entity, T &component);
};
