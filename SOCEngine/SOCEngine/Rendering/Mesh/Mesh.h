#pragma once

#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "MeshBasicData.h"
#include "Component.h"
#include "LPVoidType.h"

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
			MeshFilter*				_filter;
			MeshRenderer*			_renderer;
			Buffer::ConstBuffer*	_transformConstBuffer;

			MaterialUpdateType		_updateType;
			unsigned int			_selectMaterialIndex;
		public:
			Mesh();
			~Mesh();

		public:
			struct CreateFuncArguments : public MeshFilter::CreateFuncArguments
			{
				Material* material;
				CreateFuncArguments(const std::string& fileName, const std::string& key):
					MeshFilter::CreateFuncArguments(fileName, key){}
				~CreateFuncArguments() {}
			};

			bool Create(const CreateFuncArguments& args);

		public:
			virtual void Initialize();
			virtual void Update(float deltaTime);
			virtual void UpdateTransformCB(const Core::TransformPipelineShaderInput& transpose_Transform);
			virtual void Destroy();

		public:
			GET_ACCESSOR(MeshFilter, MeshFilter*, _filter);
			GET_ACCESSOR(MeshRenderer, MeshRenderer*, _renderer);

			GET_SET_ACCESSOR(MaterialUpdateType, MaterialUpdateType, _updateType);
			GET_SET_ACCESSOR(SelectMaterialIndex, unsigned int, _selectMaterialIndex);
		};
	}
}