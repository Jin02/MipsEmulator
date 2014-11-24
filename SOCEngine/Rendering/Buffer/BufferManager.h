#pragma once

#include "ConstBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Structure.h"
#include "LPVoidType.h"

namespace Rendering
{
	namespace Buffer
	{
		class Manager
		{
		private:
			Structure::MapInMap<Buffer::VertexBuffer>	_vertexBuffers;
			Structure::MapInMap<Buffer::IndexBuffer>	_indexBuffers;
			Structure::MapInMap<LPVoidType>				_originVertexBufferDatas;

		public:
			Manager();
			~Manager();

		public:
			void Add(const std::string file, const std::string key, Buffer::VertexBuffer* bufferData, bool copy = false);
			void Add(const std::string file, const std::string key, Buffer::IndexBuffer* bufferData, bool copy = false);
			void Add(const std::string file, const std::string key, LPVoidType* bufferData, bool copy = false);

			bool Find(Buffer::VertexBuffer*& outBuffer, const std::string file, const std::string key);
			bool Find(Buffer::IndexBuffer*& outBuffer, const std::string file, const std::string key);
			bool Find(LPVoidType*& outBuffer, const std::string file, const std::string key);

			void DeleteVertexBuffer(const std::string& file, const std::string& key, bool remove);
			void DeleteIndexBuffer(const std::string& file, const std::string& key, bool remove);
			void DeleteOriginVBData(const std::string& file, const std::string& key, bool remove);

			void DeleteVertexBuffer(const std::string& file, bool remove);
			void DeleteIndexBuffer(const std::string& file, bool remove);
			void DeleteOriginVBData(const std::string& file, bool remove);

			void DeleteAllVertexBuffer(bool remove);
			void DeleteAllIndexBuffer(bool remove);
			void DeleteAllOriginVBData(bool remove);
		};
	}
}