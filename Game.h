#ifndef _GAME_H_
#define _GAME_H_

#include"GameBase.h"
#include"GameSprite.h"
#include"DrawSprite.h"

#include"GameTime.h"
#include "DebugCon.h"


class Game : public AlignedAllocationPolicy<16>, public GameBase
{
	public:
		Game();
		virtual ~Game();
		bool LoadContent();
		void UnloadContent();
		void Update(GameTime*);
		void Render();

	private:
		DrawSprite* sb_;

		
		GameSprite *sprite, *sprite2, *batata;
};
#endif