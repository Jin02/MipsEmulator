#pragma once

#include "DirectX.h"

namespace Rendering
{
	namespace Buffer
	{
		class BaseBuffer
		{
		protected:
			ID3D11Buffer*	_buffer;

		public:
			BaseBuffer();
			virtual ~BaseBuffer();

		public:
			void Update(ID3D11DeviceContext* context, const void* data);

		public:
			GET_ACCESSOR(Buffer, ID3D11Buffer*, _buffer);
		};
	}
}