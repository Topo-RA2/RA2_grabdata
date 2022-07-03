#pragma once
#include "include.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "implot.h"
#include "implot_internal.h"

#include <tchar.h>

#include "find_game.h"

// Forward declarations of helper functions
void init_wnd();
void init_d3d9();
void init_imgui();
void msg_handle();
void CleanupDeviceD3D();
void ResetDevice();
void clear_setting();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void ToggleButton(const char* str_id, bool* v);

void showGuiThread();

namespace ImPlot {
	ImPlotContext* CreateContext();
	void ShowDemoWindow(bool* p_open);
}