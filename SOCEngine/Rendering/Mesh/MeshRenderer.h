#pragma once

#include "Material.h"
#include "Structure.h"

namespace Rendering
{
	namespace Mesh
	{
		class MeshRenderer
		{
		private:
			Structure::Vector<Material::Material> _materials;

		public:
			MeshRenderer();
			~MeshRenderer();

		public:
			bool AddMaterial(Material::Material* material, bool copy = false);			void UpdateMaterial();
		};
	}
}