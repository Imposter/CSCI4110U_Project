#pragma once

#include "Utility/Event.h"
#include <GL/glew.h>
#include <map>

enum MouseButton
{
	kMouseButton_None = -1,
	kMouseButton_Left = 0,
	kMouseButton_Right = 1,
	kMouseButton_Middle = 2
};

enum KeyState
{
	kKeyState_Up,
	kKeyState_Down
};

enum Key
{
	kKey_None = 0x0,

	kKey_Escape = 0x1B,

	kKey_PageUp = 0x21,
	kKey_PageDown = 0x22,
	kKey_End = 0x23,
	kKey_Home = 0x24,
	kKey_Left = 0x25,
	kKey_Up = 0x26,
	kKey_Right = 0x27,
	kKey_Down = 0x28,
	kKey_Insert = 0x2D,
	kKey_Delete = 0x2E,

	kKey_F1 = 0x70,
	kKey_F2 = 0x71,
	kKey_F3 = 0x72,
	kKey_F4 = 0x73,
	kKey_F5 = 0x74,
	kKey_F6 = 0x75,
	kKey_F7 = 0x76,
	kKey_F8 = 0x77,
	kKey_F9 = 0x78,
	kKey_F10 = 0x79,
	kKey_F11 = 0x7A,
	kKey_F12 = 0x7B,
	kKey_NumLock = 0x90,
	kKey_Scroll = 0x91,
	kKey_LShiftKey = 0xA0,
	kKey_RShiftKey = 0xA1,
	kKey_LControlKey = 0xA2,
	kKey_RControlKey = 0xA3,
	kKey_LMenu = 0xA4,
	kKey_RMenu = 0xA5,

	kKey_Count,

	kKey_Shift = 0x10000,
	kKey_Control = 0x20000,
	kKey_Alt = 0x40000
};

struct ResizeEventArgs : EventArgs
{
	int Width;
	int Height;

	ResizeEventArgs(int width, int height);
};

struct MoveEventArgs : EventArgs
{
	int X;
	int Y;

	MoveEventArgs(int x, int y);
};

struct MouseEventArgs : EventArgs
{
	int X;
	int Y;
	MouseButton Button;
	int Delta;

	MouseEventArgs(int x, int y);
	MouseEventArgs(int x, int y, MouseButton button);
	MouseEventArgs(int x, int y, int delta);
};

// For ASCII -- we might require caching
struct KeyPressEventArgs : EventArgs
{
	unsigned char Char;

	KeyPressEventArgs(unsigned char c);
};

struct KeyEventArgs : EventArgs
{
	int Value;
	bool Alt;
	bool Control;
	bool Shift;

	KeyEventArgs(int value, unsigned int modifiers);
	KeyEventArgs(Key special);
};

class Window
{
public:
	enum Flag
	{
		kFlag_DoubleBuffered = 1,
		kFlag_Fullscreen = 2
	};

private:
	static std::map<int, Key> m_SpecialKeys;

	GLuint m_ID;
	std::string m_Title;
	unsigned int m_X;
	unsigned int m_Y;
	unsigned int m_Width;
	unsigned int m_Height;
	bool m_DoubleBuffered;
	bool m_Fullscreen;
	bool m_Visible;
	bool m_CursorVisible;
	bool m_Open;
	std::map<int, KeyState> m_KeyStates;

	// Window callbacks
	static void onClose();
	static void onRender();
	static void onPosition(int x, int y);
	static void onResize(int width, int height);
	static void onMouseMove(int x, int y);
	static void onMouseButton(int button, int state, int x, int y);
	static void onMouseWheel(int mouse, int direction, int x, int y);
	static void onKeyDown(unsigned char key, int x, int y);
	static void onKeyUp(unsigned char key, int x, int y);
	static void onKeySpecialDown(int key, int x, int y);
	static void onKeySpecialUp(int key, int x, int y);

	void initWindow();
	void shutdownWindow();

public:
	Window(std::string title, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int flags = kFlag_DoubleBuffered);
	Window(std::string title, unsigned int width, unsigned int height, unsigned int flags = kFlag_DoubleBuffered);
	~Window();

	void Close();

	const std::string &GetTitle() const;
	void SetTitle(const std::string &title);

	bool IsFullscreen() const;
	void SetFullscreen(bool fullscreen);

	unsigned int GetX() const;
	unsigned int GetY() const;
	void SetPosition(unsigned int x, unsigned int y);

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	void SetSize(unsigned int width, unsigned int height);

	bool IsCursorVisible() const;
	void SetCursorVisible(bool visible);

	bool IsVisible() const;
	void Show();
	void Hide();

	void SwapBuffers();

	// Public events
	Event<> OnClose;
	Event<> OnRender;
	Event<ResizeEventArgs &> OnResize;
	Event<MoveEventArgs &> OnMove;
	Event<MouseEventArgs &> OnMouseDown;
	Event<MouseEventArgs &> OnMouseUp;
	Event<MouseEventArgs &> OnMouseMove;
	Event<MouseEventArgs &> OnMouseWheel;
	Event<KeyEventArgs &> OnKeyDown;
	Event<KeyEventArgs &> OnKeyUp;
	Event<KeyPressEventArgs &> OnKeyPress;
};