#pragma once

#include <string>

#include "Vector2.h"
#include "Vector3.h"

#include "MaterialManager.h"
#include "TextureManager.h"

#include "tiny_obj_loader.h"
#include "Object.h"

namespace Importer
{
	namespace Obj
	{
		class ObjImporter
		{
		public:
			struct CustomSemantic
			{
				std::string		semanticName;
				const void*		data;

				CustomSemantic() : data(nullptr){}
				CustomSemantic(const std::string& _semanticName, const void* _data) : semanticName(_semanticName), data(_data) {}
			};

		public:
			ObjImporter();
			~ObjImporter();

		private:		
			Core::Object* CloneOriginObject(const std::string& fileName, const std::string& tinyShapeName);
			void CheckCorrectShape(const tinyobj::shape_t& tinyShape);

		private:
			Core::Object* LoadMesh(const tinyobj::shape_t& tinyShape, const std::vector<tinyobj::material_t>& tinyMaterials, const std::string& fileName, bool isDynamicMesh = false);

		public:
			Rendering::BasicMaterial* LoadMaterial(const tinyobj::material_t& tinyMaterial, const std::string& fileName, const std::string& materialFileFolder);
			void LoadMaterials(Structure::BaseStructure<std::string, Rendering::BasicMaterial>** outMaterials, const std::vector<tinyobj::material_t>& tinyMaterials, const std::string& fileName, const std::string& materialFileFolder);

		public:
			Core::Object* LoadMesh(const tinyobj::shape_t& tinyShape, const std::vector<tinyobj::material_t>& tinyMaterials, const std::string& fileName, const std::vector<CustomSemantic>& customSemanticData, Rendering::BasicMaterial* material, Rendering::BasicMaterial* depthWriteMaterial, Rendering::BasicMaterial* alphaTestMaterial, bool isDynamicMesh = false);
			Core::Object* Load(const std::string& fileDir, const std::string& fileName, const std::string& materialFileFolder, bool isDynamicMesh = false);
			bool Load(std::vector<tinyobj::shape_t>& outShapes, std::vector<tinyobj::material_t>& outMaterials, const std::string& fileDir, const std::string& materialFileFolder);
		};
	}
}