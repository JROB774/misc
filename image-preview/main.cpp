// ensure this application isn't compiled without neccessary files
#ifndef _WIN32
#error This application requires Win32 to compile!
#endif

// c library includes
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>
// c++ library includes
#include <string>
#include <fstream>
#include <exception>
// external library includes
#include <windows.h>
#include <commctrl.h>
#include <gdiplus.h>
// local includes
#include "resource.h"

namespace App
{
	enum Hotkey
	{
		HOTKEY_OPEN,
		HOTKEY_SCALEDOWN,
		HOTKEY_SCALEUP,
		HOTKEY_SCALERESET,
		HOTKEY_FITWINDOW,
		HOTKEY_TOTAL
	};

	constexpr const char WINDOW_CAPTION[] = "Image Preview ~ Scale ";
	constexpr const int WINDOW_WIDTH = 384, WINDOW_HEIGHT = 304;

	constexpr const uint8_t BACK_R = 0xF4;
	constexpr const uint8_t BACK_G = 0xF7;
	constexpr const uint8_t BACK_B = 0xFC;

	constexpr const int MAX_FILENAME = 512;
	char filenamebuffer[MAX_FILENAME] = {};

	constexpr const char FILE_FILTER[] =
	"All Files (*.bmp, *.gif, *.jpe, *.jpeg, *.jpg, *.png)\0*.BMP;*.GIF;*.JPE;*.JPEG;*.JPG;*.PNG\0"
	"BMP Files (*.bmp)\0*.BMP\0"
	"GIF Files (*.gif)\0*.GIF\0"
	"JPEG Files (*.jpe, *.jpeg, *.jpg)\0*.JPE;*.JPEG;*.JPG\0"
	"PNG Files (*.png)\0*.PNG\0\0";

	constexpr const double MIN_SCALE = 0.1, MAX_SCALE = 4.0;
	constexpr const double INC_SCALE = 0.25;

	WNDCLASSEX windowclass = {};
	HWND window = nullptr;

	HWND toolbar = nullptr;
	TBBUTTON toolbuttons[HOTKEY_TOTAL] = {};
	TBADDBITMAP toolbitmap = {};

	HBITMAP image = nullptr;
	FILETIME filetime = {};

	double scale = 1.0;
	bool running = false;

	inline void ErrorBox (const char* _msg)
	{
		MessageBox(nullptr, _msg, "Error", MB_ICONEXCLAMATION|MB_OK);
		throw std::runtime_error("Error");
	}

	void ResizeWindow (HWND _window, int _width, int _height)
	{
		RECT clirect, winrect, toolrect;
		POINT difference;

		GetClientRect(_window, &clirect);
		GetWindowRect(_window, &winrect);
		GetWindowRect(toolbar, &toolrect);

		difference.x = (winrect.right - winrect.left) - clirect.right;
		difference.y = ((winrect.bottom - winrect.top) - clirect.bottom) + (toolrect.bottom - toolrect.top);

		MoveWindow(_window, winrect.left, winrect.top, _width + difference.x, _height + difference.y, true);
		RedrawWindow(_window, nullptr, nullptr, RDW_ERASE|RDW_INVALIDATE);
	}

	void GetPreviewImageSize (int* _width, int* _height)
	{
		BITMAP bitmap;
		GetObject(image, sizeof(bitmap), &bitmap);
		*_width = bitmap.bmWidth, *_height = bitmap.bmHeight;
	}

	void FitWindow ()
	{
		if (image) {
			int width, height;
			GetPreviewImageSize(&width, &height);
			ResizeWindow(window, width * scale, height * scale);
		}
	}

	void LoadPreviewImage (HWND _window)
	{
		DeleteObject(image);
		image = nullptr;

		wchar_t wfilename[MAX_FILENAME];
		std::swprintf(wfilename, L"%s", filenamebuffer);

		Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(wfilename);
		if (!bitmap) { ErrorBox("Loading image failed!"); }
		bitmap->GetHBITMAP(Gdiplus::Color(BACK_R, BACK_G, BACK_B), &image);
		if (!image) { ErrorBox("Converting image to bitmap failed!"); }
		delete bitmap;
		bitmap = nullptr;

		FitWindow();
	}

	void OpenPreviewImage (HWND _window)
	{
		OPENFILENAME openfilename = {};

		openfilename.lStructSize = sizeof(OPENFILENAME);
		openfilename.hwndOwner = _window;
		openfilename.lpstrFilter = FILE_FILTER;
		openfilename.nFilterIndex = 1;
		openfilename.lpstrFile = filenamebuffer;
		openfilename.lpstrFile[0] = '\0';
		openfilename.nMaxFile = MAX_FILENAME;
		openfilename.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;

		if (!GetOpenFileName(&openfilename)) { return; }

		LoadPreviewImage(_window);
	}

	void SetScale (float _scale)
	{
		scale = _scale;

		if (scale < MIN_SCALE) { scale = MIN_SCALE; }
		else if (scale > MAX_SCALE) { scale = MAX_SCALE; }

		if (image) { FitWindow(); }

		SetWindowText(window, std::string(WINDOW_CAPTION + std::to_string(scale)).c_str());
	}

	BOOL CALLBACK DialogProc (HWND _window, UINT _msg, WPARAM _wparam, LPARAM _lparam)
	{
		switch (_msg) {
			case (WM_INITDIALOG): { return true; }

			case (WM_CLOSE):
			case (WM_DESTROY): { EndDialog(_window, 0); break; }
		}

		return false;
	}

	LRESULT CALLBACK MainProc (HWND _window, UINT _msg, WPARAM _wparam, LPARAM _lparam)
	{
		switch (_msg) {
			case (WM_CLOSE):
			case (WM_DESTROY): { PostQuitMessage(0); return 0; }

			case (WM_SIZE): { SendMessage(toolbar, TB_AUTOSIZE, 0, 0); break; }

			case (WM_SETFOCUS): {
				RegisterHotKey(window, HOTKEY_OPEN, MOD_CONTROL, 0x4F);
				RegisterHotKey(window, HOTKEY_SCALEDOWN, MOD_CONTROL, VK_OEM_MINUS);
				RegisterHotKey(window, HOTKEY_SCALEUP, MOD_CONTROL, VK_OEM_PLUS);
				RegisterHotKey(window, HOTKEY_SCALERESET, MOD_CONTROL, 0x30);
				RegisterHotKey(window, HOTKEY_FITWINDOW, MOD_CONTROL, 0x46);
				return 0;
			}
			case (WM_KILLFOCUS): {
				UnregisterHotKey(window, HOTKEY_OPEN);
				UnregisterHotKey(window, HOTKEY_SCALEDOWN);
				UnregisterHotKey(window, HOTKEY_SCALEUP);
				UnregisterHotKey(window, HOTKEY_SCALERESET);
				UnregisterHotKey(window, HOTKEY_FITWINDOW);
				return 0;
			}

			case (WM_COMMAND): {
				switch (LOWORD(_wparam)) {
					case (ID_FILE_OPEN): { OpenPreviewImage(_window); break; }
					case (ID_FILE_EXIT): { PostMessage(_window, WM_CLOSE, 0, 0); break; }

					case (ID_VIEW_SCALEDOWN): { SetScale(scale - INC_SCALE); break; }
					case (ID_VIEW_SCALEUP): { SetScale(scale + INC_SCALE); break; }
					case (ID_VIEW_SCALERESET): { SetScale(1.0f); break; }
					case (ID_VIEW_FITWINDOW): { FitWindow(); break; }

					case (ID_HELP_TUTORIAL): { DialogBox(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDD_TUTORIAL), _window, DialogProc); break; }
					case (ID_HELP_ABOUT): { DialogBox(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDD_ABOUT), _window, DialogProc); break; }
				}
				return 0;
			}

			case (WM_HOTKEY): {
				switch (_wparam) {
					case (HOTKEY_OPEN): { OpenPreviewImage(_window); break; }

					case (HOTKEY_SCALEDOWN): { SetScale(scale - INC_SCALE); break; }
					case (HOTKEY_SCALEUP): { SetScale(scale + INC_SCALE); break; }
					case (HOTKEY_SCALERESET): { SetScale(1.0f); break; }
					case (HOTKEY_FITWINDOW): { FitWindow(); break; }
				}
				return 0;
			}

			case (WM_PAINT): {
				PAINTSTRUCT paint;
				HDC hdcdst, hdcsrc;
				BITMAP bitmap;
				HGDIOBJ oldbitmap;

				hdcdst = BeginPaint(_window, &paint);
				hdcsrc = CreateCompatibleDC(hdcdst);
				oldbitmap = SelectObject(hdcsrc, image);

				if ((GetDeviceCaps(hdcdst, RASTERCAPS) & RC_STRETCHBLT) == 0) { ErrorBox("Not capable of performing stretch blit!"); }

				if (scale < 1.0) { SetStretchBltMode(hdcdst, HALFTONE); }
				else { SetStretchBltMode(hdcdst, COLORONCOLOR); }
				SetBrushOrgEx(hdcdst, 0, 0, nullptr);

				RECT clirect, toolrect;
				GetClientRect(_window, &clirect);
				GetWindowRect(toolbar, &toolrect);

				GetObject(image, sizeof(bitmap), &bitmap);
				StretchBlt(
					hdcdst,
					clirect.left, toolrect.bottom - toolrect.top,
					bitmap.bmWidth * scale, bitmap.bmHeight * scale,
					hdcsrc,
					0, 0,
					bitmap.bmWidth, bitmap.bmHeight,
					SRCCOPY
				);

				SelectObject(hdcsrc, oldbitmap);
				DeleteDC(hdcsrc);
				EndPaint(_window, &paint);

				return 0;
			}
		}

		return DefWindowProc(_window, _msg, _wparam, _lparam);
	}

} // App

int CALLBACK WinMain (HINSTANCE _instance, HINSTANCE _previnstance, LPSTR _cmdline, int _show)
{
	try {
		INITCOMMONCONTROLSEX commctrl = {};
		commctrl.dwSize = sizeof(INITCOMMONCONTROLSEX);
		commctrl.dwICC = ICC_WIN95_CLASSES;
		InitCommonControlsEx(&commctrl);

		ULONG_PTR gptoken;
		Gdiplus::GdiplusStartupInput gpinput;
		Gdiplus::GdiplusStartup(&gptoken, &gpinput, nullptr);

		App::windowclass.cbSize = sizeof(WNDCLASSEX);
		App::windowclass.style = CS_VREDRAW|CS_HREDRAW;
		App::windowclass.lpfnWndProc = App::MainProc;
		App::windowclass.hInstance = _instance;
		App::windowclass.hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON));
		App::windowclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		App::windowclass.hbrBackground = (HBRUSH)(COLOR_INACTIVEBORDER + 1);
		App::windowclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
		App::windowclass.lpszClassName = "WNDCLASSEX_ImgPreview";
		App::windowclass.hIconSm = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON));

		if (!RegisterClassEx(&App::windowclass)) { App::ErrorBox("Window class registration failed!"); }

		App::window = CreateWindowEx(
			WS_EX_TOPMOST,
			App::windowclass.lpszClassName,
			std::string(App::WINDOW_CAPTION + std::to_string(App::scale)).c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			App::WINDOW_WIDTH, App::WINDOW_HEIGHT,
			nullptr,
			nullptr,
			App::windowclass.hInstance,
			nullptr
		);

		if (!App::window) { App::ErrorBox("Window creation failed!"); }

		ShowWindow(App::window, _show);
		UpdateWindow(App::window);

		App::toolbar = CreateWindowEx(
			0,
			TOOLBARCLASSNAME,
			nullptr,
			WS_CHILD|WS_VISIBLE,
			0, 0,
			0, 0,
			App::window,
			(HMENU)IDC_TOOLBAR,
			_instance,
			nullptr
		);

		if (!App::toolbar) { App::ErrorBox("Toolbar creation failed!"); }

		SendMessage(App::toolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

		App::toolbitmap.hInst = HINST_COMMCTRL;
		App::toolbitmap.nID = IDB_STD_SMALL_COLOR;
		SendMessage(App::toolbar, TB_ADDBITMAP, 0, (LPARAM)&App::toolbitmap);

		App::toolbuttons[0].iBitmap = STD_FILEOPEN;
		App::toolbuttons[0].fsState = TBSTATE_ENABLED;
		App::toolbuttons[0].fsStyle = TBSTYLE_BUTTON;
		App::toolbuttons[0].idCommand = ID_FILE_OPEN;
		App::toolbuttons[1].iBitmap = STD_UNDO;
		App::toolbuttons[1].fsState = TBSTATE_ENABLED;
		App::toolbuttons[1].fsStyle = TBSTYLE_BUTTON;
		App::toolbuttons[1].idCommand = ID_VIEW_SCALEDOWN;
		App::toolbuttons[2].iBitmap = STD_REDOW;
		App::toolbuttons[2].fsState = TBSTATE_ENABLED;
		App::toolbuttons[2].fsStyle = TBSTYLE_BUTTON;
		App::toolbuttons[2].idCommand = ID_VIEW_SCALEUP;
		App::toolbuttons[3].iBitmap = STD_FIND;
		App::toolbuttons[3].fsState = TBSTATE_ENABLED;
		App::toolbuttons[3].fsStyle = TBSTYLE_BUTTON;
		App::toolbuttons[3].idCommand = ID_VIEW_SCALERESET;
		App::toolbuttons[4].iBitmap = STD_PRINTPRE;
		App::toolbuttons[4].fsState = TBSTATE_ENABLED;
		App::toolbuttons[4].fsStyle = TBSTYLE_BUTTON;
		App::toolbuttons[4].idCommand = ID_VIEW_FITWINDOW;

		SendMessage(App::toolbar, TB_ADDBUTTONS, sizeof(App::toolbuttons) / sizeof(TBBUTTON), (LPARAM)&App::toolbuttons);

		if (std::strlen(_cmdline) > 0) {
			std::string cmdline = _cmdline, filename;
			if (cmdline.at(0) == '"') { filename = cmdline.substr(1, cmdline.find('"', 1) - 1); }
			else { filename = cmdline.substr(0, cmdline.find(' ', 1)); }
			std::memcpy(App::filenamebuffer, filename.c_str(), filename.length());
			App::filenamebuffer[filename.length()] = '\0';
			App::LoadPreviewImage(App::window);
		}

		App::running = true;
		while (App::running) {
			MSG msg;
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) { App::running = false; }
				else {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			if (App::image) {
				HANDLE file = CreateFile(
					App::filenamebuffer, GENERIC_READ, FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE,
					nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr
				);
				if (file != INVALID_HANDLE_VALUE) {
					FILETIME newfiletime;
					if (!GetFileTime(file, nullptr, nullptr, &newfiletime)) { App::ErrorBox("File time retrieval failed!"); }
					if (App::filetime.dwLowDateTime != newfiletime.dwLowDateTime || App::filetime.dwHighDateTime != newfiletime.dwHighDateTime) {
						App::LoadPreviewImage(App::window);
						App::filetime = newfiletime;
					}
				}
			}
		}

		DestroyWindow(App::window);
		App::window = nullptr;
		DeleteObject(App::image);
		App::image = nullptr;

		Gdiplus::GdiplusShutdown(gptoken);
	}
	catch (...) { return EXIT_FAILURE; }
	return EXIT_SUCCESS;
}

/******************************************************************************\
 * This is free and unencumbered software released into the public domain.
 * 
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <http://unlicense.org/>
 * 
\******************************************************************************/