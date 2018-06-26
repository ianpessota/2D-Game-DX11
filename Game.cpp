#include"Game.h"
#include<xnamath.h>
#include <random>
#include"TextureManager.h"
#include <string.h>
#include <stdio.h>
#include <new>
#include "Camera.h"

Camera *camera;
TextureManager* map, *pikachu;

float mov = -2;
float mov2 = 2;
float mov3 = 1;
bool pause=false;
float t=0, timer=0, f=0;
Game::Game() 
{
	
}
Game::~Game()
{
}

bool Game::LoadContent()
{
	camera = (Camera*)_aligned_malloc(sizeof(Camera), 16);
	camera->SetPosition(XMFLOAT2(0,0));
	camera->SetRotation(0);
	camera->SetZoom(1);

	sprite = (GameSprite*)_aligned_malloc(sizeof(GameSprite), 16);
	sprite->SetSource(XMFLOAT4(0, 0.25f, 0.25f, 0.25f));
	sprite->SetPosition(XMFLOAT4(80, 81, 10, 15));
	sprite->SetScale(XMFLOAT2(1, 1));
	sprite->SetRotation(1);

	sb_ =new DrawSprite(pd3dDevice, pd3dContext);

	map = new TextureManager(pd3dDevice, "map.jpg");
	pikachu = new TextureManager(pd3dDevice, "pikachu.png");

	return true;
}
void Game::UnloadContent()
{
	sb_->Unload();
	map->Unload();
	pikachu->Unload();

}

void Game::Update(GameTime *gameTime)
{
	
	if (Input->KeyDown(Keys::Esc))
		PostQuitMessage(0);
	if (Input->KeyDown(Keys::NUMPAD3))
	{

		gameTime->Update(30, true);
		
	}
	if (Input->KeyDown(Keys::NUMPAD6))
	{

		gameTime->Update(60, true);

	}
	if (Input->KeyDown(Keys::NUMPAD9))
	{

		gameTime->Update(120, true);

	}
	if (Input->KeyDown(Keys::NUMPAD5))
	{

		gameTime->Update(1, false);

	}
		
	
	if (Input->KeyDown(Keys::Right))
	{
		camera->Move(XMFLOAT2(0.00005f, 0));

	}
	if (Input->KeyDown(Keys::Left))
	{
		camera->Move(XMFLOAT2(-0.00005f, 0));
	}
	if (Input->KeyDown(Keys::Up))
	{
		camera->Move(XMFLOAT2(0, 0.00005f));
	}
	if (Input->KeyDown(Keys::Down))
	{
		camera->Move(XMFLOAT2(0, -0.00005f));
	}
	if (Input->KeyDown(Keys::Z))
	{
		camera->SetZoom(camera->GetZoom() + 0.000001f);
	}
	if (Input->KeyDown(Keys::X))
	{
		camera->SetZoom(camera->GetZoom() - 0.000001f);
	}
		
		if (Input->keyboardDevice_->Poll() != DI_OK)
			Input->keyboardDevice_->Acquire();

		
	
	
}
void Game::Render()
{
	float clearColor[4] = { 0.39f, 0.582f, 0.926f, 1 };

	pd3dContext->ClearRenderTargetView(pBackBufferTarget, clearColor);
	//pd3dContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	////Set our Render Target
	//pd3dContext->OMSetRenderTargets(1, &pBackBufferTarget, pDepthStencilView);

	////Set the default blend state (no blending) for opaque objects
	//pd3dContext->OMSetBlendState(0, 0, 0xffffffff);

	sb_->Begin(camera->GetTransformation());
	sb_->Draw(map, XMFLOAT4(0, 0, 100, 100), XMFLOAT4(1,1,1,1));
	sb_->Draw(pikachu, *sprite, !sprite->GetFlip(), XMFLOAT4(1, 1, 1,1));
	
	InitDraw();
	pSwapChain->Present(0, 0);
	

	
	
}