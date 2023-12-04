#pragma once
#include <directxtk/Mouse.h>
#include <directxtk/Keyboard.h>
// KeyManager�� �ʿ��� ����
// 1. ������ ����ȭ
// ���� ������ ������ ���� Ű�� ����, ������ �̺�Ʈ�� ��������.

// 2. Ű �Է� �̺�Ʈ ó��
// tap. hold, away ex) ����
using DirectX::Mouse;
using DirectX::Keyboard;

class InputManager
{
public:
	// ��ġ �ʱ�ȭ.
	static void Init(HWND hwnd);
	static bool IsKeyDown(Keyboard::Keys key);
	static bool IsKeyUp(Keyboard::Keys key);

	// ����.
	static void ResetKeyboardState() { keyboard.Reset(); }

	// ���콺 �Է�.
	//static Vector2 MouseDragState();
	static Mouse::State MouseState();

private:
	// Ű����, ���콺.
	static Keyboard keyboard;
	static Mouse mouse;
};

