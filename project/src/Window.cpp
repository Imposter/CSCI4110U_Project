#include "Window.h"
#include <GL/freeglut.h>
#include <utility>

std::map<int, Key> Window::m_SpecialKeys =
{
	// Special keys
	{ VK_ESCAPE, kKey_Escape },

	{ GLUT_KEY_F1, kKey_F1 },
	{ GLUT_KEY_F2, kKey_F2 },
	{ GLUT_KEY_F3, kKey_F3 },
	{ GLUT_KEY_F4, kKey_F4 },
	{ GLUT_KEY_F5, kKey_F5 },
	{ GLUT_KEY_F6, kKey_F6 },
	{ GLUT_KEY_F7, kKey_F7 },
	{ GLUT_KEY_F8, kKey_F8 },
	{ GLUT_KEY_F9, kKey_F9 },
	{ GLUT_KEY_F10, kKey_F10 },
	{ GLUT_KEY_F11, kKey_F11 },
	{ GLUT_KEY_F12, kKey_F12 },
	
	{ GLUT_KEY_LEFT, kKey_Left },
	{ GLUT_KEY_UP, kKey_Up },
	{ GLUT_KEY_RIGHT, kKey_Right },
	{ GLUT_KEY_DOWN, kKey_Down },
	
	{ GLUT_KEY_PAGE_UP, kKey_PageUp },
	{ GLUT_KEY_PAGE_DOWN, kKey_PageDown },
	{ GLUT_KEY_HOME, kKey_Home },
	{ GLUT_KEY_END, kKey_End },
	{ GLUT_KEY_INSERT, kKey_Insert },

	{ GLUT_KEY_NUM_LOCK, kKey_NumLock },
	{ GLUT_KEY_DELETE, kKey_Delete },
	{ GLUT_KEY_SHIFT_L, kKey_LShiftKey },
	{ GLUT_KEY_SHIFT_R, kKey_RShiftKey },
	{ GLUT_KEY_CTRL_L, kKey_LControlKey },
	{ GLUT_KEY_CTRL_R, kKey_RControlKey },
	{ GLUT_KEY_NUM_LOCK, kKey_NumLock },
	{ GLUT_KEY_NUM_LOCK, kKey_NumLock }
};

ResizeEventArgs::ResizeEventArgs(int width, int height)
{
	Width = width;
	Height = height;
}

MoveEventArgs::MoveEventArgs(int x, int y)
{
	X = x;
	Y = y;
}

MouseEventArgs::MouseEventArgs(int x, int y)
{
	X = x;
	Y = y;
	Delta = 0;
	Button = kMouseButton_None;
}

MouseEventArgs::MouseEventArgs(int x, int y, MouseButton button)
{
	X = x;
	Y = y;
	Button = button;
	Delta = 0;
}

MouseEventArgs::MouseEventArgs(int x, int y, int delta)
{
	X = x;
	Y = y;
	Delta = delta;
	Button = kMouseButton_None;
}

KeyPressEventArgs::KeyPressEventArgs(unsigned char c)
{
	Char = c;
}

KeyEventArgs::KeyEventArgs(int value, unsigned modifiers)
{
	Value = value;
	Alt = modifiers & kKey_Alt;
	Control = modifiers & kKey_Control;
	Shift = modifiers & kKey_Shift;
}

KeyEventArgs::KeyEventArgs(Key special)
{
	Value = special;
	Alt = special == kKey_Alt;
	Control = special == kKey_LControlKey || kKey_RControlKey;
	Shift = special == kKey_LShiftKey || kKey_RShiftKey;
}

void Window::onClose()
{
	const auto window = static_cast<Window *>(glutGetWindowData());
	
	window->m_Open = false;
	window->m_Visible = false;

	// Trigger event
	window->OnClose({});
}

void Window::onRender()
{
	const auto window = static_cast<Window *>(glutGetWindowData());

	// Trigger event
	window->OnRender({});
}

void Window::onPosition(int x, int y)
{
	const auto window = static_cast<Window *>(glutGetWindowData());

	// Update window data
	window->m_X = x;
	window->m_Y = y;

	// Trigger event
	window->OnMove({ x, y });
}

void Window::onResize(int width, int height)
{
	const auto window = static_cast<Window *>(glutGetWindowData());

	// Update window data
	window->m_Width = width;
	window->m_Height = height;

	// Trigger event
	window->OnResize({ width, height });
}

void Window::onMouseMove(int x, int y)
{
	const auto window = static_cast<Window *>(glutGetWindowData());
	window->OnMouseMove({ x, y });
}

void Window::onMouseButton(int button, int state, int x, int y)
{
	const auto window = static_cast<Window *>(glutGetWindowData());

	auto mouseButton = kMouseButton_None;
	if (button == GLUT_LEFT_BUTTON)
		mouseButton = kMouseButton_Left;
	else if (button == GLUT_RIGHT_BUTTON)
		mouseButton = kMouseButton_Right;
	else if (button == GLUT_MIDDLE_BUTTON)
		mouseButton = kMouseButton_Middle;

	if (state == GLUT_DOWN)
		window->OnMouseDown({ x, y, mouseButton });
	else if (state == GLUT_UP)
		window->OnMouseUp({ x, y, mouseButton });
}

void Window::onMouseWheel(int mouse, int direction, int x, int y)
{
	const auto window = static_cast<Window *>(glutGetWindowData());

	window->OnMouseWheel({ x, y, direction });
}

void Window::onKeyDown(unsigned char key, int x, int y)
{
	const auto window = static_cast<Window *>(glutGetWindowData());
	
	// Get modifiers
	unsigned int modifiers = 0;
	const auto mods = glutGetModifiers();
	if (mods & GLUT_ACTIVE_SHIFT)
		modifiers |= kKey_Shift;
	if (mods & GLUT_ACTIVE_CTRL)
		modifiers |= kKey_Control;
	if (mods & GLUT_ACTIVE_ALT)
		modifiers |= kKey_Alt;

	// If the key was previously up, we should trigger the event, otherwise it is a key press event
	std::map<int, KeyState>::iterator it;
	if ((it = window->m_KeyStates.find(key)) == window->m_KeyStates.end())
		window->m_KeyStates.emplace(key, kKeyState_Down);
	else
	{
		if (it->second == kKeyState_Up)
		{
			it->second = kKeyState_Down;
			window->OnKeyDown({ key, modifiers });
		}
	}

	window->OnKeyPress({ key });
}

void Window::onKeyUp(unsigned char key, int x, int y)
{
	const auto window = static_cast<Window *>(glutGetWindowData());

	// Get modifiers
	unsigned int modifiers = 0;
	const auto mods = glutGetModifiers();
	if (mods & GLUT_ACTIVE_SHIFT)
		modifiers |= kKey_Shift;
	if (mods & GLUT_ACTIVE_CTRL)
		modifiers |= kKey_Control;
	if (mods & GLUT_ACTIVE_ALT)
		modifiers |= kKey_Alt;

	// If the key was previously down, we should trigger the event
	std::map<int, KeyState>::iterator it;
	if ((it = window->m_KeyStates.find(key)) == window->m_KeyStates.end())
		window->m_KeyStates.emplace(key, kKeyState_Up);
	else
	{
		if (it->second == kKeyState_Down)
		{
			it->second = kKeyState_Up;
			window->OnKeyDown({ key, modifiers });
		}
	}
}

void Window::onKeySpecialDown(int key, int x, int y)
{
	const auto window = static_cast<Window *>(glutGetWindowData());

	std::map<int, Key>::iterator it;
	Key k;
	if ((it = m_SpecialKeys.find(key)) == m_SpecialKeys.end())
		k = kKey_None;
	else k = it->second;

	window->OnKeyDown({ k });
}

void Window::onKeySpecialUp(int key, int x, int y)
{
	const auto window = static_cast<Window *>(glutGetWindowData());

	std::map<int, Key>::iterator it;
	Key k;
	if ((it = m_SpecialKeys.find(key)) == m_SpecialKeys.end())
		k = kKey_None;
	else k = it->second;

	window->OnKeyUp({ k });
}

void Window::initWindow()
{
	// Set window display mode
	unsigned int displayMode = GLUT_RGBA;
	if (m_DoubleBuffered) displayMode |= GLUT_DOUBLE;
	glutInitDisplayMode(displayMode);

	// Set window size
	glutInitWindowSize(m_Width, m_Height);

	// Set window position
	if (m_X != 0 && m_Y != 0) glutInitWindowPosition(m_X, m_Y);

	// Create window
	m_ID = glutCreateWindow(m_Title.c_str());
	glutSetWindowData(this);

	// Set callbacks
	glutCloseFunc(&onClose);
	glutDisplayFunc(&onRender);
	glutPositionFunc(&onPosition);
	glutReshapeFunc(&onResize);
	glutMotionFunc(&onMouseMove);
	glutMouseFunc(&onMouseButton);
	glutMouseWheelFunc(&onMouseWheel);
	glutKeyboardFunc(&onKeyDown);
	glutKeyboardUpFunc(&onKeyUp);
	glutSpecialFunc(&onKeySpecialDown);
	glutSpecialUpFunc(&onKeySpecialUp);

	// Set window as open
	m_Open = true;
	m_Visible = true;

	// Init key states
	for (auto i = static_cast<int>(kKey_None); i < static_cast<int>(kKey_Count); i++)
		m_KeyStates.emplace(static_cast<Key>(i), kKeyState_Up);
}

void Window::shutdownWindow()
{
	if (m_Open)
	{
		glutDestroyWindow(m_ID);
		m_Open = false;
	}
}

Window::Window(std::string title, unsigned x, unsigned y, unsigned width, unsigned height, unsigned flags)
	: m_ID(0), m_Title(std::move(title)), m_X(x), m_Y(y), m_Width(width), m_Height(height), m_DoubleBuffered(flags & kFlag_DoubleBuffered),
	m_Fullscreen(flags & kFlag_Fullscreen), m_Visible(false), m_CursorVisible(true), m_Open(false)
{
	initWindow();
}

Window::Window(std::string title, unsigned width, unsigned height, unsigned flags)
	: m_ID(0), m_Title(std::move(title)), m_X(0), m_Y(0), m_Width(width), m_Height(height), m_DoubleBuffered(flags & kFlag_DoubleBuffered),
	m_Fullscreen(flags & kFlag_Fullscreen), m_Visible(false), m_CursorVisible(true), m_Open(false)
{
	initWindow();
}

Window::~Window()
{
	shutdownWindow();
}

const std::string &Window::GetTitle() const
{
	return m_Title;
}

void Window::SetTitle(const std::string &title)
{
	m_Title = title;
	glutSetWindow(m_ID);
	glutSetWindowTitle(title.c_str());
}

bool Window::IsFullscreen() const
{
	return m_Fullscreen;
}

void Window::SetFullscreen(bool fullscreen)
{
	m_Fullscreen = fullscreen;
	glutSetWindow(m_ID);
	if (fullscreen) glutFullScreen();
	else glutLeaveFullScreen();
}

unsigned Window::GetX() const
{
	return m_X;
}

unsigned Window::GetY() const
{
	return m_Y;
}

void Window::SetPosition(unsigned x, unsigned y)
{
	m_X = x;
	m_Y = y;
	glutSetWindow(m_ID);
	glutPositionWindow(x, y);
}

unsigned Window::GetWidth() const
{
	return m_Width;
}

unsigned Window::GetHeight() const
{
	return m_Height;
}

void Window::SetSize(unsigned width, unsigned height)
{
	m_Width = width;
	m_Height = height;
	glutSetWindow(m_ID);
	glutReshapeWindow(width, height);
}

bool Window::IsCursorVisible() const
{
	return m_CursorVisible;
}

void Window::SetCursorVisible(bool visible)
{
	m_CursorVisible = visible;
	glutSetWindow(m_ID);
	glutSetCursor(visible ? GLUT_CURSOR_LEFT_ARROW : GLUT_CURSOR_NONE);
}

bool Window::IsVisible() const
{
	return m_Visible;
}

void Window::Show()
{
	m_Visible = true;
	glutSetWindow(m_ID);
	glutShowWindow();
}

void Window::Hide()
{
	m_Visible = false;
	glutSetWindow(m_ID);
	glutHideWindow();
}

void Window::SwapBuffers()
{
	glutSetWindow(m_ID);
	glutSwapBuffers();
}