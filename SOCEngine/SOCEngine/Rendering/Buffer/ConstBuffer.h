#pragma once

#include "BaseBuffer.h"

namespace Rendering
{
	namespace Buffer
	{
		class ConstBuffer : public BaseBuffer
		{
		public:
			ConstBuffer();
			~ConstBuffer();

		public:
			bool Create(unsigned int size);
		};

	}
}