#pragma once

namespace apothec::lithium
{

	class ArrayBuffer
	{
	public:
		virtual ~ArrayBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static ArrayBuffer* CreateArrayBuffer(float* data, size_t size);
	};

}