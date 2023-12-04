#pragma once
#include <directxtk/Mouse.h>
#include <directxtk/Keyboard.h>
// KeyManager가 필요한 이유
// 1. 프레임 동기화
// 동일 프레임 내에서 같은 키에 대해, 동일한 이벤트를 가져간다.

// 2. 키 입력 이벤트 처리
// tap. hold, away ex) 점프
using DirectX::Mouse;
using DirectX::Keyboard;

class InputManager
{
public:
	// 장치 초기화.
	static void Init(HWND hwnd);
	static bool IsKeyDown(Keyboard::Keys key);
	static bool IsKeyUp(Keyboard::Keys key);

	// 리셋.
	static void ResetKeyboardState() { keyboard.Reset(); }

	// 마우스 입력.
	//static Vector2 MouseDragState();
	static Mouse::State MouseState();

private:
	// 키보드, 마우스.
	static Keyboard keyboard;
	static Mouse mouse;
};

