#include<d3d11.h>
#include<d3dx11.h>
#include<DxErr.h>

#include"GameSprite.h"



GameSprite::GameSprite() : rotation_(0)
{
	scale_.x = scale_.y = 1.0f;
	
}

GameSprite::~GameSprite()
{

}

void GameSprite::SetPosition(XMFLOAT4& position)
{
	position_ = position;
}
XMFLOAT4 GameSprite::GetPosition()
{
	return position_;
}

void GameSprite::SetRotation(float rotation)
{
	rotation_ = rotation;
}
void GameSprite::SetScale(XMFLOAT2& scale)
{
	scale_ = scale;
}
XMFLOAT2 GameSprite::GetScale()
{
	return scale_;
}
void GameSprite::SetSource(XMFLOAT4& vertices)
{
	source_ = vertices;
	sourceOut_ = source_;
}
XMFLOAT4 GameSprite::GetSource()
{
	return source_;
}
XMFLOAT4 GameSprite::GetSourceOut()
{
	return sourceOut_;
}
bool GameSprite::GetFlip()
{
	return flip;
}
void GameSprite::Flip()
{
	if (flip)
		flip = false;
	else
		flip = true;
}
void GameSprite::Animation(int row, int col, float* currentFrame)
{
	int r = 0;

	if (*currentFrame >= col) //if our frame exceeds the sheet width, goto next line first frame
	{
		*currentFrame = 0;
		sourceOut_.y = sourceOut_.y + source_.w;
		r++;
		if (r >= row) //if it exceeds the sheet height, return to first frame
		{
			r = 0;
			sourceOut_.y = source_.z;
		}
	}	
	sourceOut_.x = source_.x + source_.z*(*currentFrame);
}
