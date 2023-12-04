#include "../GameApp/pch.h"
#include "InputManager.h"

Keyboard InputManager::keyboard;
Mouse InputManager::mouse;

DirectX::Mouse::State InputManager::MouseState()
{
	return mouse.GetState();
}

void InputManager::Init(HWND hWnd)
{
	mouse.SetWindow(hWnd);
	mouse.SetMode(Mouse::Mode::MODE_RELATIVE);
	ShowCursor(true);
}

bool InputManager::IsKeyDown(Keyboard::Keys key)
{
    return keyboard.GetState().IsKeyDown(key);
}

bool InputManager::IsKeyUp(Keyboard::Keys key)
{
    return keyboard.GetState().IsKeyUp(key);
}

//Vector2 InputManager::MouseDragState()
//{
//    Mouse::State state = mouse.GetState();
//    return Vector2((float)state.x, (float)state.y);
//}
