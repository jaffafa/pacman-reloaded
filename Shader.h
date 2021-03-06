#pragma once
#include "stdafx.h"

class Shader
{

	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mImmediateContext;
	ID3D11InputLayout*		m_pInputLayout;

	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pTechnique;

public:
	Shader();
	~Shader();

	HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string filename,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, unsigned int numElements);

	HRESULT Apply(unsigned int pass);

	void SetMatrix(char* variable, D3DXMATRIX& mat);
	void SetFloat(char* variable, float value);
	void SetFloat2(char* variable, D3DXVECTOR2 value);
	void SetFloat3(char* variable, D3DXVECTOR3 value);
	void SetFloat4(char* variable, D3DXVECTOR4 value);

	void SetInt(char* variable, int value);
	void SetBool(char* variable, bool value);
	void SetRawData(char* variable, void* data, size_t size);

	void SetResource(char* variable, ID3D11ShaderResourceView* value);
	void SetUnorderedAccessView(char* variable, ID3D11UnorderedAccessView* value);
};
