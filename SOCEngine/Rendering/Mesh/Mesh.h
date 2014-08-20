#pragma once

#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "MeshBasicData.h"
#include "Component.h"

namespace Rendering
{
	namespace Mesh
	{
		class Mesh : public Core::Component
		{
		public:
			static const Core::Component::Type GetComponentType() {	return Core::Component::Type::Mesh;	}
			enum MaterialUpdateType
			{
				One, All
			};

		private:
			MeshFilter*			_filter;
			MeshRenderer*		_renderer;

			MaterialUpdateType	_updateType;
			unsigned int		_selectMaterialIndex;

			unsigned int		_indexCount;

		public:
			Mesh();
			~Mesh();

		public:
			bool Create(const void* vertexBufferDatas, unsigned int vertexBufferDataCount, unsigned int vertexBufferSize,
				const ENGINE_INDEX_TYPE* indicesData, unsigned int indicesCount, Material::Material* material, bool isDynamic);

		public:
			virtual void Initialize();
			virtual void Update(float deltaTime);
			virtual void Render(const Core::TransformPipelineParam& transform, const std::vector<Rendering::Light::LightForm*> *lights, const Math::Vector4& viewPos);
			virtual void Destroy();

		public:
			GET_ACCESSOR(MeshFilter, MeshFilter*, _filter);
			GET_ACCESSOR(MeshRenderer, MeshRenderer*, _renderer);

			GET_SET_ACCESSOR(MaterialUpdateType, MaterialUpdateType, _updateType);
			GET_SET_ACCESSOR(SelectMaterialIndex, unsigned int, _selectMaterialIndex);
		};
	}
}