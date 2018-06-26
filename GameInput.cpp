#include "GameInput.h"

GameInput::GameInput(HINSTANCE hInstance, HWND hwnd)
{
	//DirectInput Initialization
	HRESULT result;
	ZeroMemory(keyboardKeys_, sizeof(keyboardKeys_));
	ZeroMemory(prevKeyboardKeys_, sizeof(prevKeyboardKeys_));

	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, 0);

	if (FAILED(result))
	{
		return;
	}
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboardDevice_, 0);

	if (FAILED(result))
	{
		return ;
	}
	result = keyboardDevice_->SetDataFormat(&c_dfDIKeyboard);

	if (FAILED(result))
	{
		return ;
	}
	result = keyboardDevice_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	if (FAILED(result))
	{
		return ;
	}
	result = keyboardDevice_->Acquire();

	if (FAILED(result))
	{
		return ;
	}

}


GameInput::~GameInput()
{
	SafeRelease(&directInput_);
	if (keyboardDevice_)
	{
		keyboardDevice_->Unacquire();
		keyboardDevice_->Release();
	}
	keyboardDevice_ = NULL;
}
void GameInput::Update(Keys key_)
{
	
	keyboardDevice_->GetDeviceState(sizeof(keyboardKeys_), (LPVOID)&keyboardKeys_);
	if (KEYDOWN(keyboardKeys_, DIK_DOWN))
		key = Down;
	if (KEYDOWN(keyboardKeys_, DIK_RIGHT) )//&& !KEYDOWN(prevKeyboardKeys_, DIK_RIGHT))
		key = Right;
	if (KEYDOWN(keyboardKeys_, DIK_UP))
		key = Up;
	if (KEYDOWN(keyboardKeys_, DIK_LEFT))
		key = Left;
	if (KEYDOWN(keyboardKeys_, DIK_ESCAPE))
		key = Esc;
	
	if (KEYDOWN(keyboardKeys_, DIK_SPACE))
		key = Space;
	if (KEYDOWN(keyboardKeys_, DIK_LSHIFT))
		key = Shift;
	if (KEYDOWN(keyboardKeys_, DIK_Z))
		key = Z;
	if (KEYDOWN(keyboardKeys_, DIK_X))
		key = X;

	//NUMPAD
	if (KEYDOWN(keyboardKeys_, DIK_NUMPADENTER))
		key = NUMPADENTER;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPADCOMMA))
		key = NUMPADENTER;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPADSLASH))
		key = NUMPADENTER;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPADPLUS))
		key = NUMPADENTER;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPADMINUS))
		key = NUMPADENTER;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPADSTAR))
		key = NUMPADENTER;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPADMINUS))
		key = NUMPADENTER;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPAD0))
		key = NUMPAD0;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPAD1))
		key = NUMPAD1;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPAD2))
		key = NUMPAD2;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPAD3))
		key = NUMPAD3;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPAD4))
		key = NUMPAD4;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPAD5))
		key = NUMPAD5;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPAD6))
		key = NUMPAD6;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPAD7))
		key = NUMPAD7;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPAD8))
		key = NUMPAD8;
	if (KEYDOWN(keyboardKeys_, DIK_NUMPAD9))
		key = NUMPAD9;


	//else
	//	key = None;
	memcpy(prevKeyboardKeys_, keyboardKeys_, sizeof(keyboardKeys_));
}

bool GameInput::KeyDown(Keys key_)
{
	Update(key_);
	
	if (key == key_)
	{
		key = None;
		return true;
	}
	return false;
}