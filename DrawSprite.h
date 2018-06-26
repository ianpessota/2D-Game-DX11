
#ifndef _SPRITE_DRAW_H_
#define _SPRITE_DRAW_H_
#include<d3d11.h>
#include<d3dx11.h>
#include<DxErr.h>
#include"DrawBase.h"
#include<xnamath.h>
#include"TextureManager.h"
#include"GameSprite.h"


class DrawSprite :  public DrawBase
{
	public:
		DrawSprite(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dContext);
		virtual ~DrawSprite();

		void Begin(XMMATRIX &transformation);

		void End();

		//Differents Draw methods
		void Draw(XMFLOAT4& pos, XMFLOAT4& color);
		void Draw(TextureManager* texture, XMFLOAT4& position, XMFLOAT4& color);
		void Draw(TextureManager* texture, XMFLOAT4& position, XMFLOAT2& scale, float rotation, XMFLOAT4& source, bool flip, XMFLOAT4& color);
		void Draw(TextureManager* texture, XMFLOAT4& position, XMFLOAT4& source, bool flip, XMFLOAT4& color);
		void Draw(TextureManager* texture, GameSprite sprite, bool flip, XMFLOAT4& color);
		
		bool Load();
		void Unload();
private:

		ID3D11Buffer* pmMvpCbuffer;

		ID3D11DepthStencilView* pmDepthStencilView;
};

#endif