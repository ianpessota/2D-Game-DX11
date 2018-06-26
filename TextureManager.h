#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include<d3d11.h>
#include<d3dx11.h>
#include<DxErr.h>
#include "Templates.h"

class TextureManager
{
public:
	TextureManager(ID3D11Device* d3dDevice_, LPCSTR textname);
	~TextureManager();
	
	//Functions
	void LoadTexture(ID3D11Device* d3dDevice_, LPCSTR textname);
	void Unload();
	ID3D11ShaderResourceView* colorMap_;
	ID3D11SamplerState* colorMapSampler_;


private:

	ID3D11SamplerState* CreateTextureFromFile(ID3D11Device* d3dDevice_, LPCSTR textname);

	D3D11_SAMPLER_DESC colorMapDesc;

};

#endif