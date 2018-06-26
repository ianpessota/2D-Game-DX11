/*Set basics directx to render images using shaders
***********************************************************************/
#ifndef _DRAW_BASE_H_
#define _DRAW_BASE_H_

//INCLUDE
#include<d3d11.h>
#include<d3dx11.h>
#include<DxErr.h>
#include<xnamath.h>
#include "Templates.h"
#include "GameBase.h"
#pragma comment (lib, "D3D10_1.lib")
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")
#include <D3D10_1.h>
#include <DXGI.h>
#include <D2D1.h>
#include <sstream>
#include <dwrite.h>

template<size_t Alignment> class AlignedAllocationPolicy

{

public:

	static void* operator new(size_t size)

	{

		return _aligned_malloc(size, Alignment);

	}


	static void operator delete(void* memory)

	{

		_aligned_free(memory);

	}

};
class DrawBase :  public GameBase
{
	public:
		DrawBase();
		virtual ~DrawBase();

		//Functions
		void Begin();
		void End();


		//Vertex type struct
		struct VertexPositionColor
		{
			XMFLOAT3 pos;
			XMFLOAT4 color;
		};

		struct VertexPositionTexture
		{
			XMFLOAT3 pos;
			XMFLOAT2 tex;
		};
		struct VertexPositionColorTexture
		{
			XMFLOAT3 pos;
			XMFLOAT4 color;
			XMFLOAT2 tex;
		};

	protected:

		//Shader related functions
		bool LoadPixelShader(char* file, ID3D11PixelShader** ps);
		bool LoadVertexShader(char* file, ID3D11VertexShader** vs);
		bool CompileD3DShader(char* filePath, char* entry, char* shaderModel, ID3DBlob** buffer);

		//Functions to adjust positions
		XMFLOAT4 AdjustPosition(XMFLOAT4& pos);
		XMFLOAT4 AdjustAsRect(XMFLOAT4&);
		
	
	
};

#endif
