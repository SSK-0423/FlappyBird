#include <d3dx12.h>
#include "DescriptorHeapCBV_SRV_UAV.h"

#include "Texture.h"
#include "ConstantBuffer.h"

#include "ShaderResourceViewDesc.h"

using namespace Utility;

namespace DX12Wrapper
{
	HRESULT DescriptorHeapCBV_SRV_UAV::CreateDescriptorHeap(ID3D12Device& device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NodeMask = 0;
		heapDesc.NumDescriptors = m_MAX_CBV_DESCRIPTOR_NUM + m_MAX_SRV_DESCRIPTOR_NUM + m_MAX_UAV_DESCRIPTOR_NUM;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		HRESULT result = device.CreateDescriptorHeap(
			&heapDesc, IID_PPV_ARGS(m_descriptorHeap.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { return result; }

		return result;
	}

	RESULT DescriptorHeapCBV_SRV_UAV::Create(ID3D12Device& device)
	{
		// �n���h���̃C���N�������g�T�C�Y�擾
		m_handleIncrimentSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		// �f�B�X�N���v�^�q�[�v����
		if (FAILED(CreateDescriptorHeap(device))) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}

	// TODO: ���g�p�̗̈悪����ꍇ�͂�����g���悤�ɂ���
	int DescriptorHeapCBV_SRV_UAV::RegistShaderResource(
		ID3D12Device& device, Texture& texture,
		ShaderResourceViewDesc& desc, const int& registerNo)
	{
		assert(registerNo < static_cast<int>(m_MAX_SRV_DESCRIPTOR_NUM) && registerNo >= NEXT_REGISTER);

		auto cpuHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		auto gpuHandle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();

		// GPUDescriptorHandle���ꏏ�ɃC���N�������g����K�v����H�H

		if (registerNo == NEXT_REGISTER)	// �o�^����Ă��郊�\�[�X���̎��̃��W�X�^
		{
			cpuHandle.ptr += m_handleIncrimentSize * (static_cast<SIZE_T>(m_registedSRVNum) + m_MAX_CBV_DESCRIPTOR_NUM);
			gpuHandle.ptr += m_handleIncrimentSize * (static_cast<SIZE_T>(m_registedSRVNum) + m_MAX_CBV_DESCRIPTOR_NUM);
		}
		else                                // �w�肳�ꂽ���W�X�^
		{
			cpuHandle.ptr += m_handleIncrimentSize * (static_cast<SIZE_T>(registerNo) + m_MAX_CBV_DESCRIPTOR_NUM);
			gpuHandle.ptr += m_handleIncrimentSize * (static_cast<SIZE_T>(registerNo) + m_MAX_CBV_DESCRIPTOR_NUM);
		}

		int retIndex = 0;
		if (registerNo == NEXT_REGISTER)
		{
			retIndex = m_registedSRVNum;
		}
		else
		{
			retIndex = registerNo;
		}

		device.CreateShaderResourceView(&texture.GetBuffer(), &desc.desc, cpuHandle);

		//// ���̃��W�X�^���w�肵���ꍇ�͓o�^���ꂽ���\�[�X�����C���N�������g
		//if (registerNo == NEXT_REGISTER)
		//{
		m_registedSRVNum++;
		//}

		return retIndex;
	}

	void DescriptorHeapCBV_SRV_UAV::RegistConstantBuffer(
		ID3D12Device& device, ConstantBuffer& constantBuffer, const int& registerNo)
	{
		assert(registerNo < static_cast<int>(m_MAX_CBV_DESCRIPTOR_NUM) && registerNo >= NEXT_REGISTER);

		auto cpuHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		auto gpuHandle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();

		if (registerNo == NEXT_REGISTER)	// �o�^����Ă��郊�\�[�X���̎��̃��W�X�^
		{
			cpuHandle.ptr += m_handleIncrimentSize * m_registedCBVNum;
			gpuHandle.ptr += m_handleIncrimentSize * m_registedCBVNum;
		}
		else                                // �w�肳�ꂽ���W�X�^
		{
			cpuHandle.ptr += m_handleIncrimentSize * registerNo;
			gpuHandle.ptr += m_handleIncrimentSize * registerNo;
		}

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = constantBuffer.GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = constantBuffer.GetBufferSize();

		device.CreateConstantBufferView(&cbvDesc, cpuHandle);

		m_registedCBVNum++;
	}

	void DescriptorHeapCBV_SRV_UAV::RegistUnorderedAccessResource(
		ID3D12Device& device, UnorderedAccessResource& unorderedAccessResource, const int& registerNo)
	{
		assert(registerNo < static_cast<int>(m_MAX_CBV_DESCRIPTOR_NUM) && registerNo >= NEXT_REGISTER);
		// Todo:�K�v�ɂȂ��������
	}
}
