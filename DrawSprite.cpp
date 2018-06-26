#include "DrawSprite.h"

DrawSprite::DrawSprite(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext) : pmMvpCbuffer(0), pmDepthStencilView(0)
{
	pd3dContext = d3dContext;
	pd3dDevice = d3dDevice;
	Load();
}

DrawSprite::~DrawSprite()
{

}

bool DrawSprite::Load()
{
	//Load shaders
	LoadVertexShader("SolidColor.fx", &pmSolidColorVS);
	LoadPixelShader("SolidColor.fx", &pmSolidColorPS);
	LoadVertexShader("TextureMap.fx",&pmTextureColorVS);
	LoadPixelShader("TextureMap.fx",&pmTextureColorPS);
	
	//Create Sampler desc
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create blend desc
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	pd3dDevice->CreateBlendState(&blendDesc, &pmAlphaBlendState);
	pd3dContext->OMSetBlendState(pmAlphaBlendState, blendFactor, 0xFFFFFFFF);

	D3D11_BUFFER_DESC constDesc;
	ZeroMemory(&constDesc, sizeof(constDesc));
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof(XMMATRIX);
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dResult = pd3dDevice->CreateBuffer(&constDesc, 0, &pmMvpCbuffer);
	
	return true;
}

void DrawSprite::Unload()
{
	SafeRelease(&pmColorMap);
	SafeRelease(&pmSolidColorVS);
	SafeRelease(&pmSolidColorPS);
	SafeRelease(&pmTextureColorVS);
	SafeRelease(&pmTextureColorPS);
	SafeRelease(&pmInputLayout);
	SafeRelease(&pmAlphaBlendState);
	SafeRelease(&pmMvpCbuffer);
	SafeRelease(&pmColorMapSampler);
	SafeRelease(&pmDepthStencilView);
	SafeRelease(&pmVertexBuffer);

}
void DrawSprite::Begin(XMMATRIX &transformation)
{
	//Refresh the Depth/Stencil view
	//pd3dContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	////Set our Render Target
	//pd3dContext->OMSetRenderTargets(1, &pBackBufferTarget, pDepthStencilView);
	//pd3dContext->Begin(null);
	XMMATRIX projection = XMMatrixOrthographicOffCenterLH(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	XMMATRIX mvp = (transformation*projection);
	mvp = XMMatrixTranspose(mvp);

	pd3dContext->UpdateSubresource(pmMvpCbuffer, 0, 0, &mvp, 0, 0);
	pd3dContext->VSSetConstantBuffers(0, 1, &pmMvpCbuffer);
}
void DrawSprite::End()
{
	pmMvpCbuffer = 0 ;
}
//position, color
void DrawSprite::Draw(XMFLOAT4& position, XMFLOAT4& color)
{
	XMFLOAT4 pos = AdjustPosition(position);
	VertexPositionColor vertices[] =
	{
		{ XMFLOAT3(pos.x, pos.y, 0.5f), color},
		{ XMFLOAT3(pos.z, pos.y, 0.5f), color },
		{ XMFLOAT3(pos.x, pos.w, 0.5f), color },
		{ XMFLOAT3(pos.z, pos.w, 0.5f), color }
	};

	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPositionColor)* 4;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;
	d3dResult = pd3dDevice->CreateBuffer(&vertexDesc, &resourceData, &pmVertexBuffer);

	if (FAILED(d3dResult))
	{
		return ;
	}

	unsigned int stride = sizeof(VertexPositionColor);
	unsigned int offset = 0;

	pd3dContext->IASetInputLayout(pmInputLayout);
	pd3dContext->IASetVertexBuffers(0, 1, &pmVertexBuffer, &stride, &offset);
	pd3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	pd3dContext->VSSetShader(pmSolidColorVS, 0, 0);
	pd3dContext->PSSetShader(pmSolidColorPS, 0, 0);

	pd3dContext->Draw(4, 0);
	pmVertexBuffer->Release();
}
//texture, Rect, color
void DrawSprite::Draw(TextureManager* texture, XMFLOAT4& position, XMFLOAT4& color)
{
	XMFLOAT4 pos = AdjustPosition(position);
	VertexPositionColorTexture vertices[] =
	{
		{ XMFLOAT3(pos.x, pos.y, 0.5f),color, XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(pos.z, pos.y, 0.5f),color, XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(pos.x, pos.w, 0.5f), color,XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(pos.z, pos.w, 0.5f), color,XMFLOAT2(1.0f, 1.0f) }
	};

	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPositionColorTexture)* 4;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;
	d3dResult = pd3dDevice->CreateBuffer(&vertexDesc, &resourceData, &pmVertexBuffer);

	if (FAILED(d3dResult))
	{
		return;
	}
	
	unsigned int stride = sizeof(VertexPositionColorTexture);
	unsigned int offset = 0;

	pd3dContext->IASetInputLayout(pmInputLayout);
	pd3dContext->IASetVertexBuffers(0, 1, &pmVertexBuffer, &stride, &offset);
	pd3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	pd3dContext->VSSetShader(pmTextureColorVS, 0, 0);
	pd3dContext->PSSetShader(pmTextureColorPS, 0, 0);
	pd3dContext->PSSetShaderResources(0, 1, &(texture->colorMap_));
	pd3dContext->PSSetSamplers(0, 1, &(texture->colorMapSampler_));


	pd3dContext->Draw(4, 0);
	pmVertexBuffer->Release();

}
//Texture, rect(position), rect(texture), flip, color
void DrawSprite::Draw(TextureManager* texture, XMFLOAT4& position, XMFLOAT4& source, bool flip,XMFLOAT4& color)
{
	XMFLOAT4 pos = AdjustPosition(position);
	source = AdjustAsRect(source);

	if (flip)
	{
		VertexPositionColorTexture vertices[] =
		{
			{ XMFLOAT3(pos.x, pos.y, 0.5f), color, XMFLOAT2(source.z, source.y) },
			{ XMFLOAT3(pos.z, pos.y, 0.5f), color, XMFLOAT2(source.x, source.y) },
			{ XMFLOAT3(pos.x, pos.w, 0.5f), color, XMFLOAT2(source.z, source.w) },
			{ XMFLOAT3(pos.z, pos.w, 0.5f), color, XMFLOAT2(source.x, source.w) }
		};
		ZeroMemory(&vertexDesc, sizeof(vertexDesc));
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(VertexPositionColorTexture)* 4;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = vertices;
		d3dResult = pd3dDevice->CreateBuffer(&vertexDesc, &resourceData, &pmVertexBuffer);

		if (FAILED(d3dResult))
		{
			return;
		}
	}
	else
	{
		VertexPositionColorTexture vertices[] =
		{
			{ XMFLOAT3(pos.x, pos.y, 0.5f), color, XMFLOAT2(source.x, source.y) },
			{ XMFLOAT3(pos.z, pos.y, 0.5f), color, XMFLOAT2(source.z, source.y) },
			{ XMFLOAT3(pos.x, pos.w, 0.5f), color, XMFLOAT2(source.x, source.w) },
			{ XMFLOAT3(pos.z, pos.w, 0.5f), color, XMFLOAT2(source.z, source.w) }
		};
		ZeroMemory(&vertexDesc, sizeof(vertexDesc));
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(VertexPositionColorTexture)* 4;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = vertices;
		d3dResult = pd3dDevice->CreateBuffer(&vertexDesc, &resourceData, &pmVertexBuffer);

		if (FAILED(d3dResult))
		{
			return;
		}
	}

	unsigned int stride = sizeof(VertexPositionColorTexture);
	unsigned int offset = 0;

	pd3dContext->IASetInputLayout(pmInputLayout);
	pd3dContext->IASetVertexBuffers(0, 1, &pmVertexBuffer, &stride, &offset);
	pd3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	pd3dContext->VSSetShader(pmTextureColorVS, 0, 0);
	pd3dContext->PSSetShader(pmTextureColorPS, 0, 0);
	pd3dContext->PSSetShaderResources(0, 1, &(texture->colorMap_));
	pd3dContext->PSSetSamplers(0, 1, &(texture->colorMapSampler_));


	pd3dContext->Draw(4, 0);
	pmVertexBuffer->Release();

}
void DrawSprite::Draw(TextureManager* texture, XMFLOAT4& position, XMFLOAT2& scale, float rotation, XMFLOAT4& source, bool flip, XMFLOAT4& color)
{

	position.w *= scale.x;
	position.z *= scale.y;
	XMFLOAT4 pos = AdjustPosition(position);
	source = AdjustAsRect(source);
	if (flip)
	{
		VertexPositionColorTexture vertices[] =
		{
			{ XMFLOAT3(pos.x, pos.y, 0.5f), color, XMFLOAT2(source.z, source.y) },
			{ XMFLOAT3(pos.z, pos.y, 0.5f), color, XMFLOAT2(source.x, source.y) },
			{ XMFLOAT3(pos.x, pos.w, 0.5f), color, XMFLOAT2(source.z, source.w) },
			{ XMFLOAT3(pos.z, pos.w, 0.5f), color, XMFLOAT2(source.x, source.w) }
		};
		ZeroMemory(&vertexDesc, sizeof(vertexDesc));
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(VertexPositionColorTexture)* 4;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = vertices;
		d3dResult = pd3dDevice->CreateBuffer(&vertexDesc, &resourceData, &pmVertexBuffer);

		if (FAILED(d3dResult))
		{
			return;
		}
	}
	else
	{
		VertexPositionColorTexture vertices[] =
		{
			{ XMFLOAT3(pos.x, pos.y, 0.5f), color, XMFLOAT2(source.x, source.y) },
			{ XMFLOAT3(pos.z, pos.y, 0.5f), color, XMFLOAT2(source.z, source.y) },
			{ XMFLOAT3(pos.x, pos.w, 0.5f), color, XMFLOAT2(source.x, source.w) },
			{ XMFLOAT3(pos.z, pos.w, 0.5f), color, XMFLOAT2(source.z, source.w) }
		};
		ZeroMemory(&vertexDesc, sizeof(vertexDesc));
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(VertexPositionColorTexture)* 4;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = vertices;
		d3dResult = pd3dDevice->CreateBuffer(&vertexDesc, &resourceData, &pmVertexBuffer);

		if (FAILED(d3dResult))
		{
			return;
		}
	}

	unsigned int stride = sizeof(VertexPositionColorTexture);
	unsigned int offset = 0;

	pd3dContext->IASetInputLayout(pmInputLayout);
	pd3dContext->IASetVertexBuffers(0, 1, &pmVertexBuffer, &stride, &offset);
	pd3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	pd3dContext->VSSetShader(pmTextureColorVS, 0, 0);
	pd3dContext->PSSetShader(pmTextureColorPS, 0, 0);
	pd3dContext->PSSetShaderResources(0, 1, &(texture->colorMap_));
	pd3dContext->PSSetSamplers(0, 1, &(texture->colorMapSampler_));


	pd3dContext->Draw(4, 0);
	pmVertexBuffer->Release();

}
void DrawSprite::Draw(TextureManager* texture, GameSprite sprite, bool flip, XMFLOAT4& color)
{
	sprite.SetPosition(XMFLOAT4(sprite.GetPosition().x * sprite.GetScale().x,
						sprite.GetPosition().y,
						sprite.GetPosition().z*sprite.GetScale().y,
						sprite.GetPosition().w));// *= sprite.GetScale().x;
	// position.z *= sprite.GetScale().y;
	XMFLOAT4 pos = AdjustPosition(sprite.GetPosition());
	XMFLOAT4 source = AdjustAsRect(sprite.GetSourceOut());
	if (flip)
	{
		VertexPositionColorTexture vertices[] =
		{
			{ XMFLOAT3(pos.x, pos.y, 0.5f), color, XMFLOAT2(source.z, source.y) },
			{ XMFLOAT3(pos.z, pos.y, 0.5f), color, XMFLOAT2(source.x, source.y) },
			{ XMFLOAT3(pos.x, pos.w, 0.5f), color, XMFLOAT2(source.z, source.w) },
			{ XMFLOAT3(pos.z, pos.w, 0.5f), color, XMFLOAT2(source.x, source.w) }
		};
		ZeroMemory(&vertexDesc, sizeof(vertexDesc));
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(VertexPositionColorTexture)* 4;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = vertices;
		d3dResult = pd3dDevice->CreateBuffer(&vertexDesc, &resourceData, &pmVertexBuffer);

		if (FAILED(d3dResult))
		{
			return;
		}
	}
	else
	{
		VertexPositionColorTexture vertices[] =
		{
			{ XMFLOAT3(pos.x, pos.y, 0.5f), color, XMFLOAT2(source.x, source.y) },
			{ XMFLOAT3(pos.z, pos.y, 0.5f), color, XMFLOAT2(source.z, source.y) },
			{ XMFLOAT3(pos.x, pos.w, 0.5f), color, XMFLOAT2(source.x, source.w) },
			{ XMFLOAT3(pos.z, pos.w, 0.5f), color, XMFLOAT2(source.z, source.w) }
		};
		ZeroMemory(&vertexDesc, sizeof(vertexDesc));
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(VertexPositionColorTexture)* 4;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = vertices;
		d3dResult = pd3dDevice->CreateBuffer(&vertexDesc, &resourceData, &pmVertexBuffer);

		if (FAILED(d3dResult))
		{
			return;
		}
	}

	unsigned int stride = sizeof(VertexPositionColorTexture);
	unsigned int offset = 0;

	pd3dContext->IASetInputLayout(pmInputLayout);
	pd3dContext->IASetVertexBuffers(0, 1, &pmVertexBuffer, &stride, &offset);
	pd3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	pd3dContext->VSSetShader(pmTextureColorVS, 0, 0);
	pd3dContext->PSSetShader(pmTextureColorPS, 0, 0);
	pd3dContext->PSSetShaderResources(0, 1, &(texture->colorMap_));
	pd3dContext->PSSetSamplers(0, 1, &(texture->colorMapSampler_));


	pd3dContext->Draw(4, 0);
	pmVertexBuffer->Release();

}