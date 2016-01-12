#include "Component.h"
#include "Object.h"
#include "LightForm.h"

namespace Core
{

	Component::Component(void)
		:_owner(nullptr)
	{
	}

	Component::~Component(void)
	{
	}

	void Component::Update(float deltaTime)
	{
		//null
	}

	void Component::UpdateTransformCB(const TransformPipelineShaderInput& transpose_Transform)
	{
		//null
	}

	void Component::Render()
	{
		//null
	}

}