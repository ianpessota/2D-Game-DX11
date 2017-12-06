#ifndef _GAME_SPRITE_H_
#define _GAME_SPRITE_H_

#include<xnamath.h>


class GameSprite
{
public:
	GameSprite();
	virtual ~GameSprite();

	void SetPosition(XMFLOAT4& position);
	void SetRotation(float rotation);
	void SetScale(XMFLOAT2& scale);
	void SetSource(XMFLOAT4& vertices_);
	XMFLOAT4 GetSource();
	XMFLOAT2 GetScale();
	XMFLOAT4 GetSourceOut();
	XMFLOAT4 GetPosition();
	void Flip();
	bool GetFlip();
	void Animation(int row, int col, float* frame);
	void Update();
	

private:
	XMFLOAT4 position_;
	float rotation_;
	XMFLOAT2 scale_;
	XMFLOAT4 source_;
	XMFLOAT4 sourceOut_;
	bool flip = false;
};

#endif
