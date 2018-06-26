#ifndef _GAMEBASE_H_
#define _GAMEBASE_H_

#include<d3d11.h>		//
#include<d3dx11.h>		//
#include<DxErr.h>		//Directx error
#include<D3Dcompiler.h> //
#include<dinput.h>		//Input Lib
#include"GameTime.h"
#include"GameInput.h"
#include "Templates.h"
#include <xnamath.h>

#pragma comment (lib, "D3D10_1.lib")
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")
#include <D3D10_1.h>
#include <DXGI.h>
#include <D2D1.h>
#include <sstream>
#include <dwrite.h>


class GameBase
{
	public:

		GameBase();
		virtual ~GameBase();

		bool Initialize(HINSTANCE hInstance, HWND hwnd);
		void Shutdown();

		//Virtual methods
		virtual bool LoadContent();
		virtual void UnloadContent();
		virtual void Update(GameTime*);
		virtual void Render();

		//Window sizes 
		unsigned int windowHeight;
		unsigned int windowWidth;

		HWND getHwnd();
		void InitD2DScreenTexture();
		void RenderText(std::wstring text, XMMATRIX &transformation);
		void DrawString();
		void InitDraw();

	private:
	
		void InitD3D11();
		bool InitD2D_D3D101_DWrite(IDXGIAdapter1 *Adapter);

	protected:
		GameInput* Input;
		//Window
		HINSTANCE pHInstance;
		HWND pHwnd;

		//Device
		D3D_DRIVER_TYPE DriverType;
		D3D_FEATURE_LEVEL FeatureLevel;
		ID3D11Device* pd3dDevice;
		ID3D11DeviceContext* pd3dContext;

		//Buffer
		IDXGISwapChain* pSwapChain;
		ID3D11RenderTargetView* pBackBufferTarget;
		ID3D11Texture2D* pDepthStencilBuffer;
		ID3D11DepthStencilView* pDepthStencilView;
		
		//Shader related pointers
		ID3D11VertexShader* pmSolidColorVS;
		ID3D11PixelShader* pmSolidColorPS;

		ID3D11VertexShader* pmTextureColorVS;
		ID3D11PixelShader* pmTextureColorPS;

		//Other
		ID3D11InputLayout* pmInputLayout;
		ID3D11Buffer* pmVertexBuffer;

		ID3D11ShaderResourceView* pmColorMap;
		ID3D11SamplerState* pmColorMapSampler;
		ID3D11BlendState* pmAlphaBlendState;

		D3D11_BUFFER_DESC vertexDesc;
		D3D11_SUBRESOURCE_DATA resourceData;
		D3D11_BLEND_DESC blendDesc;
		D3D11_SAMPLER_DESC samplerDesc;

		HRESULT d3dResult;
		

		ID3D10Device1 *d3d101Device;
		IDXGIKeyedMutex *keyedMutex11;
		IDXGIKeyedMutex *keyedMutex10;
		ID2D1RenderTarget *D2DRenderTarget;
		ID2D1SolidColorBrush *Brush;
		ID3D11Texture2D *BackBuffer11;
		ID3D11Texture2D *sharedTex11;
		ID3D11Buffer *d2dVertBuffer;
		ID3D11Buffer *d2dIndexBuffer;
		ID3D11ShaderResourceView *d2dTexture;
		IDWriteFactory *DWriteFactory;
		IDWriteTextFormat *TextFormat;

		std::wstring printText;

		ID3D11Buffer* squareIndexBuffer;
		ID3D11Buffer* cbPerObjectBuffer;
		ID3D11RasterizerState* CCWcullMode;
		ID3D11RasterizerState* CWcullMode;


		XMMATRIX WVP;


		XMMATRIX d2dWorld;

	


		ID3D10Blob* VS_Buffer;
		ID3D10Blob* PS_Buffer;

		ID3D11VertexShader* VS;
		ID3D11PixelShader* PS;


};
#endif