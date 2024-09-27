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

#include <sstream>
#include "fstream"
#include "stdlib.h"

class sceneManager {
public:
    int image_width;
    double aspect_ratio;

    sceneManager (int width, double ratio): image_width(width), aspect_ratio(ratio) {};

    unsigned char * load_scene()
    {
        auto material = make_shared<lambertian>(color(0.2, 0.2, 0.2));

        world.add(make_shared<sphere>(point3(0,-1000,0), 1000, material));

        camera cam;

        cam.image_width       = image_width;
        cam.aspect_ratio      = aspect_ratio;
        cam.samples_per_pixel = 5;
        cam.max_depth         = 10;
        cam.background        = color(0.5,0.5,0.5);

        cam.vfov     = 30;
        cam.lookfrom = point3(20,2,5);
        cam.lookat   = point3(0,1.5,4.5);
        cam.vup      = vec3(0,1,0);

        cam.defocus_angle = 0;
        cam.focus_dist    = 20.0;

        return cam.render(world);
    }

    void add_sphere(point3 pos, double radius)
    {
        auto material  = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
        world.add(make_shared<sphere>(pos, radius, material));
    }

private:
    hittable_list world;


};

// Data
static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static bool                     g_DeviceLost = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
bool RenderImageOnTexture(PDIRECT3DTEXTURE9& texture, unsigned char* rgbaValues, int width, int height);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{

    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);
    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    //set up image
    int image_width = 1200;
    int image_height = 675;
    PDIRECT3DTEXTURE9 texture = NULL;
    sceneManager scene(image_width, double(image_width) / double(image_height) );

    // Main loop
    bool done = false;

    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
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
            static int renderTime = 0;
            static double x = 0;
            static double y = 0;
            static double z = 0;
            static double radius = 0;

            ImGui::Begin("Settings!");
            ImGui::Text("Render Options");
            ImGui::InputDouble("x", &x);
            ImGui::InputDouble("y", &y);
            ImGui::InputDouble("z", &z);
            ImGui::InputDouble("radius", &radius);
            if (ImGui::Button("Add Sphere"))
            {
                scene.add_sphere(vec3(x,y,z), radius);
            }

            if (ImGui::Button("Render"))
            {
                auto beg = high_resolution_clock::now();
                RenderImageOnTexture(texture, scene.load_scene(), image_width, image_height);

                renderTime = duration_cast<seconds>(high_resolution_clock::now() - beg).count();
                std::cout << "ELAPSED: " << duration_cast<microseconds>(high_resolution_clock::now() - beg).count();
            }
            std::string text = "Rendered in " + std::to_string(renderTime) + "s \n";
            ImGui::Text(text.c_str());
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
// Helper functions
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