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
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int showGuiThread();

namespace ImPlot {
	ImPlotContext* CreateContext();
	void ShowDemoWindow(bool* p_open);
}