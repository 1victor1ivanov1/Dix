#pragma once

#include "Dix/Core/Base.h"

namespace Dix
{
	struct Buffer
	{
		u64 Size = 0;
		u8* Data = nullptr;

		Buffer() = default;
		Buffer(u64 size)
		{
			Allocate(size);
		}

		Buffer(const void* data, u64 size)
			: Data((u8*)data), Size(size)
		{}

		Buffer(const Buffer&) = default;

		~Buffer()
		{
			Release();
		}

		void Allocate(u64 size)
		{
			Data = new u8[size];
			Size = size;
		}

		void Release()
		{
			delete[] Data;
			Data = nullptr;
			Size = 0;
		}

		template <typename T>
		T* As()
		{
			return (T*)Data;
		}
	};
}