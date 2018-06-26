#include "TextureManager.h"

//Instaciate class
TextureManager::TextureManager(ID3D11Device* d3dDevice_, LPCSTR textname)
{
	LoadTexture(d3dDevice_,textname);
}
//Set Sampler desc and call CreateTextureFromFile
void TextureManager::LoadTexture(ID3D11Device* d3dDevice_, LPCSTR textname)
{
	
	ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
	colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

	CreateTextureFromFile(d3dDevice_, textname);
}
TextureManager::~TextureManager()
{
	
}
void TextureManager::Unload()
{
	SafeRelease(&colorMapSampler_);
	SafeRelease(&colorMap_);
	SafeRelease(&colorMapSampler_);
}
//Create a texture from file and return it as SamplerState
ID3D11SamplerState* TextureManager::CreateTextureFromFile(ID3D11Device* d3dDevice_, LPCSTR textname)
{

	HRESULT d3dResult = D3DX11CreateShaderResourceViewFromFile(d3dDevice_,
		textname, 0, 0, &colorMap_, 0);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Failed to load the textur!");
		return NULL;
	}

	d3dResult = d3dDevice_->CreateSamplerState(&colorMapDesc, &colorMapSampler_);

	if (FAILED(d3dResult))
	{
		//DXTRACE_MSG("Failed to create color map sampler state!");
		return NULL;
	}

	return colorMapSampler_;
}
