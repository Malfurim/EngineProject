#include "Window.h"

// --- ADDITIONAL INCLUDES ---
#include <Windowsx.h>
#include "InputManager.h"
#include "Interactive.h"
#include "Settings.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Window* Window::ms_instance = nullptr;

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Window::Window()
{
    Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, FULL_SCREEN);
}

Window::~Window()
{
    if (m_fullscreen)
    {
        // Switch to windowed if full screen
        ChangeDisplaySettings(NULL, 0);
    }

    // Destroy window
    DestroyWindow(m_hwnd);
    m_hwnd = NULL;

    // Unregister window class
    UnregisterClass(m_applicationName, m_hinstance);
    m_hinstance = NULL;

    ms_instance = nullptr;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
bool Window::Initialize(int screenWidth, int screenHeight, LPCWSTR title, bool isFullscreen)
{
    WNDCLASSEX winClass;
    DEVMODE dmScreenSettings;
    DWORD windowStyle = 0;
    int posX, posY;

    m_fullscreen = isFullscreen;

    // Get the instance of this application
    m_hinstance = GetModuleHandle(NULL);

    // Give the application a name
    m_applicationName = title;

    // Setup the windows class with default settings
    winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    winClass.lpfnWndProc = WndProc;
    winClass.cbClsExtra = 0;
    winClass.cbWndExtra = 0;
    winClass.hInstance = m_hinstance;
    winClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    winClass.hIconSm = winClass.hIcon;
    winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winClass.lpszMenuName = NULL;
    winClass.lpszClassName = m_applicationName;
    winClass.cbSize = sizeof(WNDCLASSEX);

    // Register the window class
    if (!RegisterClassEx(&winClass))
    {
        MessageBox(NULL, L"RegisterClassEx() failed.", L"Error", MB_OK);
        return false;
    }

    // Setup the screen settings depending on whether it is running in full screen or in windowed mode
    if (m_fullscreen)
    {
        screenWidth = GetSystemMetrics(SM_CXSCREEN);
        screenHeight = GetSystemMetrics(SM_CYSCREEN);

        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
        dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
        posX = 0;
        posY = 0;

        windowStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_VISIBLE;
    }
    else
    {
        windowStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;

        // Create wanted size of window screen
        RECT windowRect = { 0, 0, screenWidth, screenHeight };

        // Adjust the window size to the correct window screen + window border and header
        AdjustWindowRect(&windowRect, windowStyle, FALSE);

        screenWidth = windowRect.right - windowRect.left;
        screenHeight = windowRect.bottom - windowRect.top;

        posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
    }

    //int nStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;

    m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
        windowStyle, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

    return true;
}

//void Window::Update()
//{
//}

//void Window::Render()
//{
//}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Window* Window::Instance()
{
    if (ms_instance == nullptr)
        ms_instance = new Window();
    return ms_instance;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }

    case WM_KEYDOWN:
    {
        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
        }

        INP->KeyDown((unsigned int)wParam);
        return 0;
    }

    case WM_KEYUP:
    {
        INP->KeyUp((unsigned int)wParam);
        return 0;
    }

    case WM_LBUTTONDOWN:
    {
        INP->MouseButtonDown(MouseButton::MOUSE_BUTTON_LEFT);
        return 0;
    }

    case WM_LBUTTONUP:
    {
        INP->MouseButtonUp(MouseButton::MOUSE_BUTTON_LEFT);
        return 0;
    }

    case WM_RBUTTONDOWN:
    {
        INP->MouseButtonDown(MouseButton::MOUSE_BUTTON_RIGHT);
        return 0;
    }

    case WM_RBUTTONUP:
    {
        INP->MouseButtonUp(MouseButton::MOUSE_BUTTON_RIGHT);
        return 0;
    }

    case WM_MBUTTONDOWN:
    {
        INP->MouseButtonDown(MouseButton::MOUSE_BUTTON_MIDDLE);
        return 0;
    }

    case WM_MBUTTONUP:
    {
        INP->MouseButtonUp(MouseButton::MOUSE_BUTTON_MIDDLE);
        return 0;
    }

    case WM_XBUTTONDOWN:
    {
        WORD xButton = GET_XBUTTON_WPARAM(wParam);
        if (xButton == XBUTTON1)
            INP->MouseButtonDown(MouseButton::MOUSE_BUTTON_X1);
        else if (xButton == XBUTTON2)
            INP->MouseButtonDown(MouseButton::MOUSE_BUTTON_X2);
        return true;
    }

    case WM_XBUTTONUP:
    {
        WORD xButton = GET_XBUTTON_WPARAM(wParam);
        if (xButton == XBUTTON1)
            INP->MouseButtonUp(MouseButton::MOUSE_BUTTON_X1);
        else if (xButton == XBUTTON2)
            INP->MouseButtonUp(MouseButton::MOUSE_BUTTON_X2);
        return true;
    }

    case WM_MOUSEMOVE:
    {
        INP->SetMousePosition(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    }

    case WM_CHAR:
    {
        if (Interactive::GetFocus(FocusType::FOCUS_TYPE_UI) != nullptr)
        {
            Interactive::GetFocus(FocusType::FOCUS_TYPE_UI)->OnTextInput((wchar_t)wParam);
        }
        return 0;
    }

    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }

    default:
    {
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    };

    return 0;
}
