//gui
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

//raytracer
#include "MathLib/mathLibrary.h"
#include "RayGeneration/camera.h"
#include "Hittables/hittable.h"
#include "Materials/material.h"
#include "Hittables/hittable_list.h"
#include "Hittables/sphere.h"
#include "Hittables/triangle.h"
#include "Hittables/mesh.h"

#include "sceneManager.h"
#include <sstream>
#include "fstream"
#include "stdlib.h"

static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static bool                     g_DeviceLost = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
bool RenderImageOnTexture(PDIRECT3DTEXTURE9& texture, unsigned char* rgbaValues, int width, int height);
void ColorPicker(color& col);
void Vec3Input(double &x, double&y, double&z, int id);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main(int, char**)
{
    // Create application window
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);
    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Setup Render Settings
    int image_width = 1200;
    int image_height = 675;

    int samples_per_pixel = 1;
    int ray_depth = 1;

    double lookfrom_x = 0;
    double lookfrom_y = 5;
    double lookfrom_z = 5;

    double lookat_x = 0;
    double lookat_y = 0;
    double lookat_z = 0;

    double pos_x = 0;
    double pos_y = 0;
    double pos_z = 0;
    double radius = 0;
    color color_selected;

    sceneManager scene (image_width, image_height,
                        samples_per_pixel, ray_depth,
                        lookfrom_x, lookfrom_y, lookfrom_z,
                        lookat_x, lookat_y, lookat_z
                        );

    PDIRECT3DTEXTURE9 texture = NULL;

    // Main loop
    bool done = false;

    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;
        // Handle lost D3D9 device
        if (g_DeviceLost)
        {
            HRESULT hr = g_pd3dDevice->TestCooperativeLevel();
            if (hr == D3DERR_DEVICELOST)
            {
                ::Sleep(10);
                continue;
            }
            if (hr == D3DERR_DEVICENOTRESET)
                ResetDevice();
            g_DeviceLost = false;
        }
        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }
        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("Render Options");

            ImGui::Text("SPHERE");
            Vec3Input(pos_x, pos_y, pos_z, 0);
            ImGui::InputDouble("radius", &radius);

            ColorPicker(color_selected);
            if (ImGui::Button("Add Sphere"))
            {
                scene.add_sphere(vec3(pos_x,pos_y,pos_z), radius, color_selected);
            }

            ImGui::Text("CAMERA SETTINGS");
            ImGui::InputInt("samples", &samples_per_pixel);
            ImGui::InputInt("depth", &ray_depth);
            Vec3Input(lookfrom_x, lookfrom_y, lookfrom_z, 1);
            ImGui::SameLine();
            ImGui::Text("look from");
            Vec3Input(lookat_x, lookat_y, lookat_z, 2);
            ImGui::SameLine();
            ImGui::Text("look at");

            if (ImGui::Button("Render"))
            {
                auto beg = high_resolution_clock::now();
                RenderImageOnTexture(texture, scene.load_scene(), image_width, image_height);
                std::cout << "ELAPSED: " << duration_cast<microseconds>(high_resolution_clock::now() - beg).count();
            }
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        //4. image
        if (texture)
        {
            ImGui::SetNextWindowSize(ImVec2(image_width, image_height), ImGuiCond_Always);
            ImGui::Begin("Image Window", nullptr, ImGuiWindowFlags_NoScrollbar);
            ImGui::Image((void*)texture, ImVec2(image_width, image_height));
            ImGui::End();
        }

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
        if (result == D3DERR_DEVICELOST)
            g_DeviceLost = true;
    }

    if (texture)
        texture->Release();

    // Cleanup
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    return 0;
}


void Vec3Input(double &x, double&y, double&z, int id)
{
    ImGui::PushID(id);
    ImGui::PushItemWidth(75);
    ImGui::InputDouble("x", &x);
    ImGui::SameLine();
    ImGui::InputDouble("y", &y);
    ImGui::SameLine();
    ImGui::InputDouble("z", &z);
    ImGui::PopItemWidth();
    ImGui::PopID();
}

void ColorPicker(color &col) {
    const int EDGE_SIZE = 200; // = int( ImGui::GetWindowWidth() * 0.75f );
    const ImVec2 SV_PICKER_SIZE = ImVec2(EDGE_SIZE, EDGE_SIZE);
    const float SPACING = ImGui::GetStyle().ItemInnerSpacing.x;
    const float HUE_PICKER_WIDTH = 20.f;
    const float CROSSHAIR_SIZE = 7.0f;

    ImColor color(float(col[0]), col[1], col[2]);

    ImDrawList *draw_list = ImGui::GetWindowDrawList();

// setup

    ImVec2 picker_pos = ImGui::GetCursorScreenPos();

    float hue, saturation, value;
    ImGui::ColorConvertRGBtoHSV(
            color.Value.x, color.Value.y, color.Value.z, hue, saturation, value);

// draw hue bar

    ImColor colors[] = {ImColor(255, 0, 0),
                        ImColor(255, 255, 0),
                        ImColor(0, 255, 0),
                        ImColor(0, 255, 255),
                        ImColor(0, 0, 255),
                        ImColor(255, 0, 255),
                        ImColor(255, 0, 0)};

    for (int i = 0; i < 6; ++i) {
        draw_list->AddRectFilledMultiColor(
                ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING, picker_pos.y + i * (SV_PICKER_SIZE.y / 6)),
                ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING + HUE_PICKER_WIDTH,
                       picker_pos.y + (i + 1) * (SV_PICKER_SIZE.y / 6)),
                colors[i],
                colors[i],
                colors[i + 1],
                colors[i + 1]);
    }

    draw_list->AddLine(
            ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING - 2, picker_pos.y + hue * SV_PICKER_SIZE.y),
            ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING + 2 + HUE_PICKER_WIDTH,
                   picker_pos.y + hue * SV_PICKER_SIZE.y),
            ImColor(255, 255, 255));

// draw color matrix

    {
        const ImU32 c_oColorBlack = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 1.f));
        const ImU32 c_oColorBlackTransparent = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 0.f));
        const ImU32 c_oColorWhite = ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f, 1.f));

        ImVec4 cHueValue(1, 1, 1, 1);
        ImGui::ColorConvertHSVtoRGB(hue, 1, 1, cHueValue.x, cHueValue.y, cHueValue.z);
        ImU32 oHueColor = ImGui::ColorConvertFloat4ToU32(cHueValue);

        draw_list->AddRectFilledMultiColor(
                ImVec2(picker_pos.x, picker_pos.y),
                ImVec2(picker_pos.x + SV_PICKER_SIZE.x, picker_pos.y + SV_PICKER_SIZE.y),
                c_oColorWhite,
                oHueColor,
                oHueColor,
                c_oColorWhite
        );

        draw_list->AddRectFilledMultiColor(
                ImVec2(picker_pos.x, picker_pos.y),
                ImVec2(picker_pos.x + SV_PICKER_SIZE.x, picker_pos.y + SV_PICKER_SIZE.y),
                c_oColorBlackTransparent,
                c_oColorBlackTransparent,
                c_oColorBlack,
                c_oColorBlack
        );
    }

// draw cross-hair

    float x = saturation * SV_PICKER_SIZE.x;
    float y = (1 - value) * SV_PICKER_SIZE.y;
    ImVec2 p(picker_pos.x + x, picker_pos.y + y);
    draw_list->AddLine(ImVec2(p.x - CROSSHAIR_SIZE, p.y), ImVec2(p.x - 2, p.y), ImColor(255, 255, 255));
    draw_list->AddLine(ImVec2(p.x + CROSSHAIR_SIZE, p.y), ImVec2(p.x + 2, p.y), ImColor(255, 255, 255));
    draw_list->AddLine(ImVec2(p.x, p.y + CROSSHAIR_SIZE), ImVec2(p.x, p.y + 2), ImColor(255, 255, 255));
    draw_list->AddLine(ImVec2(p.x, p.y - CROSSHAIR_SIZE), ImVec2(p.x, p.y - 2), ImColor(255, 255, 255));

// color matrix logic

    ImGui::InvisibleButton("saturation_value_selector", SV_PICKER_SIZE);

    if (ImGui::IsItemActive() && ImGui::GetIO().MouseDown[0]) {
        ImVec2 mouse_pos_in_canvas = ImVec2(
                ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

/**/ if (mouse_pos_in_canvas.x < 0) mouse_pos_in_canvas.x = 0;
        else if (mouse_pos_in_canvas.x >= SV_PICKER_SIZE.x - 1) mouse_pos_in_canvas.x = SV_PICKER_SIZE.x - 1;

/**/ if (mouse_pos_in_canvas.y < 0) mouse_pos_in_canvas.y = 0;
        else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1) mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

        value = 1 - (mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1));
        saturation = mouse_pos_in_canvas.x / (SV_PICKER_SIZE.x - 1);
    }

// hue bar logic

    ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + SPACING + SV_PICKER_SIZE.x, picker_pos.y));
    ImGui::InvisibleButton("hue_selector", ImVec2(HUE_PICKER_WIDTH, SV_PICKER_SIZE.y));

    if (ImGui::GetIO().MouseDown[0] && (ImGui::IsItemHovered() || ImGui::IsItemActive())) {
        ImVec2 mouse_pos_in_canvas = ImVec2(
                ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

/**/ if (mouse_pos_in_canvas.y < 0) mouse_pos_in_canvas.y = 0;
        else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1) mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

        hue = mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1);
    }

// R,G,B or H,S,V color editor

    color = ImColor::HSV(hue >= 1 ? hue - 10 * 1e-6 : hue, saturation > 0 ? saturation : 10 * 1e-6,
                         value > 0 ? value : 1e-6);
    col[0] = color.Value.x;
    col[1] = color.Value.y;
    col[2] = color.Value.z;

// try to cancel hue wrap (after ColorEdit), if any
    {
        float new_hue, new_sat, new_val;
        ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], new_hue, new_sat, new_val);
        if (new_hue <= 0 && hue > 0) {
            if (new_val <= 0 && value != new_val) {
                color = ImColor::HSV(hue, saturation, new_val <= 0 ? value * 0.5f : new_val);
                col[0] = color.Value.x;
                col[1] = color.Value.y;
                col[2] = color.Value.z;
            } else if (new_sat <= 0) {
                color = ImColor::HSV(hue, new_sat <= 0 ? saturation * 0.5f : new_sat, new_val);
                col[0] = color.Value.x;
                col[1] = color.Value.y;
                col[2] = color.Value.z;
            }
        }
    }

}

bool RenderImageOnTexture(PDIRECT3DTEXTURE9& texture, unsigned char* rgbaValues, int width, int height)
{
    HRESULT result = g_pd3dDevice->CreateTexture(
            width,
            height,
            1,                               // Mip levels (1 for no mipmaps)
            0,                               // Usage
            D3DFMT_A8R8G8B8,                 // Format
            D3DPOOL_MANAGED,                 // Memory pool
            &texture,                        // Pointer to texture
            NULL
    );

    if (FAILED(result) || !texture)
    {
        std::cerr << "Failed to create texture. HRESULT: " << result << std::endl;
        return false;
    }

    D3DLOCKED_RECT lockedRect;
    result = texture->LockRect(0, &lockedRect, NULL, 0);
    if (FAILED(result))
    {
        std::cerr << "Failed to lock texture. HRESULT: " << result << std::endl;
        return false;
    }

    // Ensure the memory is correctly allocated and copied
    unsigned char* dst = (unsigned char*)lockedRect.pBits;
    for (int y = 0; y < height; ++y)
    {
        // Check for buffer overflow
        if (y * lockedRect.Pitch + width * 4 > height * lockedRect.Pitch)
        {
            std::cerr << "Row data exceeds allocated texture memory." << std::endl;
            texture->UnlockRect(0);
            return false;
        }

        // Copy row data
        memcpy(dst + y * lockedRect.Pitch, &rgbaValues[y * width * 4], width * 4);
    }

    // Unlock the texture
    result = texture->UnlockRect(0);
    if (FAILED(result))
    {
        std::cerr << "Failed to unlock texture. HRESULT: " << result << std::endl;
        return false;
    }

    return true;
}
bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;
    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;
    return true;
}
void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}
void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
    switch (msg)
    {
        case WM_SIZE:
            if (wParam == SIZE_MINIMIZED)
                return 0;
            g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
            g_ResizeHeight = (UINT)HIWORD(lParam);
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}