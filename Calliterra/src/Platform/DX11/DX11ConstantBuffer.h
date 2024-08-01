#pragma once
#include "Platform/DX11/DX11Context.h"
#include "Renderer/ConstantBuffer.h"

template<typename Type>
class DX11ConstantBuffer : public ConstantBuffer
{
public:
	DX11ConstantBuffer(DX11Context& context, const Type& constants, uint32_t slot = 0)
		: m_Context(context), m_Slot(slot)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = sizeof(constants);
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = &constants;
		
		ASSERT_HR(
			m_Context.GetDevice().CreateBuffer(
				&bufferDesc, &subresourceData, &m_ConstantBuffer
			)
		);
		
	}

	DX11ConstantBuffer(DX11Context& context, uint32_t slot = 0)
		: m_Context(context), m_Slot(slot)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = sizeof(Type);
		bufferDesc.StructureByteStride = 0;

		ASSERT_HR(
			m_Context.GetDevice().CreateBuffer(
				&bufferDesc, nullptr, &m_ConstantBuffer
			)
		);
		
	}

	void Update(const Type& constants)
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		ASSERT_HR(
			m_Context.GetDeviceContext().Map(
				m_ConstantBuffer.Get(),
				0,
				D3D11_MAP_WRITE_DISCARD,
				0,
				&mappedSubresource
			)
		);

		memcpy(mappedSubresource.pData, &constants, sizeof(constants));
		m_Context.GetDeviceContext().Unmap(m_ConstantBuffer.Get(), 0);
	}

protected:
	DX11Context& m_Context;
	ComPtr<ID3D11Buffer> m_ConstantBuffer;
	uint32_t m_Slot;
};

template<typename Type>
class DX11VertexConstantBuffer : public DX11ConstantBuffer<Type>
{
	using DX11ConstantBuffer<Type>::m_Context;
	using DX11ConstantBuffer<Type>::m_ConstantBuffer;
	using DX11ConstantBuffer<Type>::m_Slot;
public:
	using DX11ConstantBuffer<Type>::DX11ConstantBuffer;
	void Bind(std::optional<uint32_t> slot) override
	{
		
		if (slot.has_value())
		{
			m_Context.GetDeviceContext().VSSetConstantBuffers(slot.value(), 1, m_ConstantBuffer.GetAddressOf());
		}
		else
		{
			m_Context.GetDeviceContext().VSSetConstantBuffers(m_Slot, 1, m_ConstantBuffer.GetAddressOf());
		}
	}
	
};

template<typename Type>
class DX11PixelConstantBuffer : public DX11ConstantBuffer<Type>
{
	using DX11ConstantBuffer<Type>::m_Context;
	using DX11ConstantBuffer<Type>::m_ConstantBuffer;
	using DX11ConstantBuffer<Type>::m_Slot;
public:
	using DX11ConstantBuffer<Type>::DX11ConstantBuffer;
	void Bind(std::optional<uint32_t> slot) override
	{
		if (slot.has_value())
		{
			m_Context.GetDeviceContext().PSSetConstantBuffers(slot.value(), 1, m_ConstantBuffer.GetAddressOf());
		}
		else
		{
			m_Context.GetDeviceContext().PSSetConstantBuffers(m_Slot, 1, m_ConstantBuffer.GetAddressOf());
		}
	}

};

template<typename Type>
class DX11ComputeConstantBuffer : public DX11ConstantBuffer<Type>
{
	using DX11ConstantBuffer<Type>::m_Context;
	using DX11ConstantBuffer<Type>::m_ConstantBuffer;
	using DX11ConstantBuffer<Type>::m_Slot;
public:
	using DX11ConstantBuffer<Type>::DX11ConstantBuffer;
	void Bind(std::optional<uint32_t> slot) override
	{
		if (slot.has_value())
		{
			m_Context.GetDeviceContext().CSSetConstantBuffers(slot.value(), 1, m_ConstantBuffer.GetAddressOf());
		}
		else
		{
			m_Context.GetDeviceContext().CSSetConstantBuffers(m_Slot, 1, m_ConstantBuffer.GetAddressOf());
		}
	}

};

template<typename Type>
class DX11GeometryConstantBuffer : public DX11ConstantBuffer<Type>
{
	using DX11ConstantBuffer<Type>::m_Context;
	using DX11ConstantBuffer<Type>::m_ConstantBuffer;
	using DX11ConstantBuffer<Type>::m_Slot;
public:
	using DX11ConstantBuffer<Type>::DX11ConstantBuffer;
	void Bind(std::optional<uint32_t> slot) override
	{
		if (slot.has_value())
		{
			m_Context.GetDeviceContext().CSSetConstantBuffers(slot.value(), 1, m_ConstantBuffer.GetAddressOf());
		}
		else
		{
			m_Context.GetDeviceContext().CSSetConstantBuffers(m_Slot, 1, m_ConstantBuffer.GetAddressOf());
		}
	}

};

template<typename Type>
class DX11HullConstantBuffer : public DX11ConstantBuffer<Type>
{
	using DX11ConstantBuffer<Type>::m_Context;
	using DX11ConstantBuffer<Type>::m_ConstantBuffer;
	using DX11ConstantBuffer<Type>::m_Slot;
public:
	using DX11ConstantBuffer<Type>::DX11ConstantBuffer;
	void Bind(std::optional<uint32_t> slot) override
	{
		if (slot.has_value())
		{
			m_Context.GetDeviceContext().CSSetConstantBuffers(slot.value(), 1, m_ConstantBuffer.GetAddressOf());
		}
		else
		{
			m_Context.GetDeviceContext().CSSetConstantBuffers(m_Slot, 1, m_ConstantBuffer.GetAddressOf());
		}
	}

};

template<typename Type>
class DX11DomainConstantBuffer : public DX11ConstantBuffer<Type>
{
	using DX11ConstantBuffer<Type>::m_Context;
	using DX11ConstantBuffer<Type>::m_ConstantBuffer;
	using DX11ConstantBuffer<Type>::m_Slot;
public:
	using DX11ConstantBuffer<Type>::DX11ConstantBuffer;
	void Bind(std::optional<uint32_t> slot) override
	{
		if (slot.has_value())
		{
			m_Context.GetDeviceContext().CSSetConstantBuffers(slot.value(), 1, m_ConstantBuffer.GetAddressOf());
		}
		else
		{
			m_Context.GetDeviceContext().CSSetConstantBuffers(m_Slot, 1, m_ConstantBuffer.GetAddressOf());
		}
	}

};

