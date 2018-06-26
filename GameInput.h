#pragma once
#include<dinput.h>		//Input Lib
#include "Templates.h"

#define KEYDOWN(name, key) ( name[key] & 0x80 )
enum Keys
{
	None,
	Down,Up,Right,Left,
	A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,X,Y,W,Z,
	NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,
	NUMPAD0, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9, NUMPADENTER,
	Space, Enter, Esc, Tab, CtrlLeft, CtrlRight, Alt, Shift

};
class GameInput
{
public:
	GameInput(HINSTANCE hInstance, HWND hwnd);
	~GameInput();
	bool KeyDown(Keys k);
	//DirectInput
	LPDIRECTINPUT8 directInput_;
	LPDIRECTINPUTDEVICE8 keyboardDevice_;
	char keyboardKeys_[256];
	char prevKeyboardKeys_[256];
	Keys key;
	void Update(Keys);
	

};

