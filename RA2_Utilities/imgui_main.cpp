#include "imgui_main.h"
#include "process_data.h"
// Data
LPDIRECT3D9              g_pD3D = NULL;
LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS    g_d3dpp = {};

HWND hwnd = NULL;
WNDCLASSEX wc{ 0 };
// Helper functions

std::map<int, ImVec4> rgbTable;

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

void ToggleButton(const char* str_id, bool* v)
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.50f;

    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    if (ImGui::IsItemClicked()) *v = !*v;
    ImGuiContext& gg = *GImGui;
    float ANIM_SPEED = 0.085f;
    if (gg.LastActiveId == gg.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
        float t_anim = ImSaturate(gg.LastActiveIdTimer / ANIM_SPEED);
    if (ImGui::IsItemHovered())
        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_ButtonActive] : ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * 0.5f);
    else
        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_Button] : ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * 0.50f);
    draw_list->AddCircleFilled(ImVec2(p.x + radius + (*v ? 1 : 0) * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}


// Main code
extern Game game;
//extern ImPlotContext* GImPlot = NULL;
int init_img = 0;
int img_width, img_height, img_channel;
PDIRECT3DTEXTURE9 my_texture = NULL;
int img2_width, img2_height, img2_channel;
PDIRECT3DTEXTURE9 small_id = NULL;

template <typename T>
int BinarySearch(const T* arr, int l, int r, T x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;
        if (arr[mid] == x)
            return mid;
        if (arr[mid] > x)
            return BinarySearch(arr, l, mid - 1, x);
        return BinarySearch(arr, mid + 1, r, x);
    }
    return -1;
}

extern int showUIState;
extern vector<FrameInfo> all_frame;
extern vector<GlobalPlayerInfo> all_player_info;

void render_handle()
{
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Main", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), game.game_status == 1 ? u8"游戏启动!" : u8"游戏结束!");
    
    static int initFile = 0;
    if (initFile == 0)
    {
        handle_game_data();
        initFile++;
    }
    ImPlot::ShowDemoWindow();
    

    if (ImGui::BeginTable("table_columns_flags", 2, ImGuiTableFlags_None))
    {

        static bool showUnit = true;
        static bool showInfantry = true;
        static int frameIndex = 0;
        static float unit_x[500] = { 0 }, unit_y[500] = { 0 };

        {
            ImGui::TableNextColumn();
            ImGui::PushID(0);

            //ToggleButton("showUnit", &showUnit);
            //ToggleButton("showInfantry", &showInfantry);
            ImGui::SliderInt("frameIndex", &frameIndex, 0, all_frame.size());
            ImGui::SliderInt("frameIndex", &frameIndex, 0, all_frame.size());

            ImGui::PopID();
        }
        {
            ImGui::TableNextColumn();
            ImGui::PushID(1);
            if (1 == showUIState)
            {

                memset(unit_x, 0, sizeof(unit_x));
                memset(unit_y, 0, sizeof(unit_y));
                if (all_frame.size() != 0)
                {
                    if (showUnit)
                    {
                        int cnt = 0;
                        for (int each_unit = 0; each_unit < all_frame[frameIndex].vec_fui.size(); ++each_unit)
                        {
                            if (0 != all_frame[frameIndex].vec_fui[each_unit].x && 0 != all_frame[frameIndex].vec_fui[each_unit].y)
                            {
                                unit_x[cnt] = all_frame[frameIndex].vec_fui[each_unit].x;
                                unit_y[cnt] = -all_frame[frameIndex].vec_fui[each_unit].y;// +0.5f * ((float)rand() / (float)RAND_MAX);
                                ++cnt;
                            }
                        }
                    }

                    ++frameIndex;
                    if (frameIndex >= all_frame.size())
                    {
                        frameIndex = all_frame.size() - 1;
                    }
                }

                if (ImPlot::BeginPlot("Scatter Plot", ImVec2(800, 800)))
                {
                    ImPlot::SetupLegend(ImPlotLocation_South | ImPlotLocation_East, ImPlotLegendFlags_Outside);
                    static ImPlotRect lims(0, 120, -120, 0);
                    static bool linkx = true, linky = true;

                    ImPlot::SetupAxisLinks(ImAxis_X1, linkx ? &lims.X.Min : NULL, linkx ? &lims.X.Max : NULL);
                    ImPlot::SetupAxisLinks(ImAxis_Y1, linky ? &lims.Y.Min : NULL, linky ? &lims.Y.Max : NULL);

                    ImPlot::PlotScatter("Data 1", unit_x, unit_y, all_frame[frameIndex].vec_fui.size());
                    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 1.25f);
                    ImPlot::SetNextMarkerStyle(ImPlotMarker_Square, 6, ImPlot::GetColormapColor(1), IMPLOT_AUTO, ImPlot::GetColormapColor(1));
                    ImPlot::PopStyleVar();
                    ImPlot::EndPlot();
                }

            }
            else if (0 == showUIState)
            {

            }
            else if (999 == showUIState)
            {

            }
            ImGui::PopID();
        }
        
        


        ImGui::EndTable();
    }


    static float values[90] = {};
    static int values_offset = 0;
    static double refresh_time = 0.0;
    if (refresh_time == 0.0)
        refresh_time = ImGui::GetTime();
    while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
    {
        static float phase = 0.0f;
        values[values_offset] = cosf(phase);
        values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
        phase += 0.10f * values_offset;
        refresh_time += 1.0f / 60.0f;
    }
    ImGui::End();

    // Rendering
    ImGui::EndFrame();
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
    if (g_pd3dDevice->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        g_pd3dDevice->EndScene();
    }
    HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

    if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
        ResetDevice();
}

void init_wnd()
{
    // Create application window
    ImGui_ImplWin32_EnableDpiAwareness();
    wc = 
    {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        WndProc,
        0L,
        0L,
        GetModuleHandle(NULL),
        NULL,
        NULL,
        NULL,
        NULL,
        _T("RA2_Data_analysis"),
        NULL
    };
    ::RegisterClassEx(&wc);
    hwnd = CreateWindow(
        wc.lpszClassName,
        _T("RA2_Data_analysis"),
        WS_OVERLAPPEDWINDOW,
        100, 100, 1600, 900,
        NULL,
        NULL,
        wc.hInstance,
        NULL
    );
    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);
}

void init_d3d9()
{
    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    HRESULT state = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice);
}

void init_imgui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    GImPlot = ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    float scale = ImGui_ImplWin32_GetDpiScaleForHwnd(hwnd);

    ImFontConfig config;
    config.OversampleH = 2;
    config.OversampleV = 2;
    io.Fonts->Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight;
    io.Fonts->AddFontFromFileTTF("./PingFang-SC-Semibold-2.ttf", 20.0f * scale, &config, io.Fonts->GetGlyphRangesChineseFull());

    ImGui::GetStyle().ScaleAllSizes(scale);

    rgbTable[3] =   (ImVec4)ImColor(222, 227, 8);
    rgbTable[11] =  (ImVec4)ImColor(255, 24, 24);
    rgbTable[21] =  (ImVec4)ImColor(41, 117, 231);
    rgbTable[29] =  (ImVec4)ImColor(57, 211, 41);
    rgbTable[13] =  (ImVec4)ImColor(255, 162, 24);
    rgbTable[25] =  (ImVec4)ImColor(49, 215, 231);
    rgbTable[17] =  (ImVec4)ImColor(148, 40, 189);
    rgbTable[15] =  (ImVec4)ImColor(255, 154, 239);
}

void msg_handle()
{
    MSG msg{ 0 };
    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else
            render_handle();
    }
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}
void clear_setting()
{
    // Cleanup
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
}

void showGuiThread()
{
    init_wnd();
    init_d3d9();
    init_imgui();
    msg_handle();
    clear_setting();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
