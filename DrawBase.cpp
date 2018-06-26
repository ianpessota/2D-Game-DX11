#include "DrawBase.h"
#include<D3Dcompiler.h>


//Input definitions
D3D11_INPUT_ELEMENT_DESC inputLayout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

//Constructor
DrawBase::DrawBase() 
{
}

//Destructor
DrawBase::~DrawBase()
{
}

//Check if file can be compiled
bool DrawBase::CompileD3DShader(char* filePath, char* entry, char* shaderModel, ID3DBlob** buffer)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* errorBuffer = 0;
	HRESULT result;

	result = D3DX11CompileFromFile(filePath, 0, 0, entry, shaderModel, shaderFlags, 0, 0, buffer, &errorBuffer, 0);

	if (FAILED(result))
	{
		if (errorBuffer != 0)
		{
			OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}
		return false;
	}
	if (errorBuffer != 0)
		errorBuffer->Release();

	return true;
}
//Compile and load pixel shader
bool DrawBase::LoadPixelShader(char* file, ID3D11PixelShader** ps)
{
	ID3DBlob* psBuffer = 0;

	bool compileResult = CompileD3DShader(file, "PS_Main", "ps_4_0", &psBuffer);

	HRESULT d3dResult;

	if (compileResult == false)
	{
		MessageBox(0, "Error loading pixel shader!", "Compile Error", MB_OK);
		return false;
	}

	d3dResult = pd3dDevice->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), 0, ps);

	psBuffer->Release();

	if (FAILED(d3dResult))
	{
		return false;
	}

	return true;
}

//Compile and load vertex shader
bool DrawBase::LoadVertexShader(char* file, ID3D11VertexShader** vs)
{
	ID3DBlob* vsBuffer = 0;

	bool compileResult = CompileD3DShader(file, "VS_Main", "vs_4_0", &vsBuffer);

	HRESULT d3dResult;
	
	if (compileResult == false)
	{
		MessageBox(0, "Error loading vertex shader!", "Compile Error", MB_OK);
		return false;
	}

	d3dResult = pd3dDevice->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, vs);

	if (FAILED(d3dResult))
	{
		if (vsBuffer)
			vsBuffer->Release();

		return false;
	}
	unsigned int totalLayoutElements = ARRAYSIZE(inputLayout);

	d3dResult = pd3dDevice->CreateInputLayout(inputLayout, totalLayoutElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &pmInputLayout);

	vsBuffer->Release();

	if (FAILED(d3dResult))
	{
		return false;
	}

	return true;
}

//Transport xy axis center to top-left corner
XMFLOAT4 DrawBase::AdjustPosition(XMFLOAT4& pos)
{
	pos = AdjustAsRect(pos);
	pos.x /= 50;
	pos.y /= 50;
	pos.z /= 50;
	pos.w /= 50;


	pos.x -= 1;
	pos.y *= -1;
	pos.y += 1;
	pos.z -= 1;
	pos.w *= -1;
	pos.w += 1;
	return pos;
}

//We transform XMFLOAT in a RECT like
XMFLOAT4 DrawBase::AdjustAsRect(XMFLOAT4& pos)
{
	pos.w += pos.y;
	pos.z += pos.x;
	return pos;
}
