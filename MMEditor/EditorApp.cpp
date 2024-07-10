#include "EditorApp.h"

#include <iostream>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "../WindowMaker/WindowInput.h"
#include "../src/Texture/TextureDrawing.h"
#include "EditableWorld/WorldConverter.h"

#define PLAYERRADIUS 0.1f

#define RENDERHEIGHT 150*4
#define RENDERWIDTH 250*4
#define RENDERRATIO RENDERWIDTH/static_cast<float>(RENDERHEIGHT)

int EditorApp::Run()
{
    float lastFrame = 0.0f;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.getGLFWwindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    // render loop
    // -----------
    while (!window.shouldClose())
    {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        Tick(deltaTime);
        Render();
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    window.terminate();
    return 0;
}

void EditorApp::Init()
{
    if (window.init() == -1)
    {
        std::cerr << "Application : Window.init() Failed." << std::endl;
    }
    glEnable(GL_DEPTH_TEST);

    
    shownTexture.CreateBlankTexture(RENDERWIDTH,RENDERHEIGHT, GL_RGB);
    window.SetDisplayedTexture(shownTexture.GetTextureId());
    LoadWorld();

    BindInput();
}

void EditorApp::Tick(float deltaTime)
{
    double mouseX, mouseY;
    glfwGetCursorPos(window.getGLFWwindow(), &mouseX, &mouseY);
    float worldX, worldY;
    WindowToWorld(mouseX, mouseY, worldX, worldY);
    float viewX, viewY;
    WindowToViewport(mouseX, mouseY, viewX, viewY);
    worldInteractor.MousePositionCallBackEvent(viewX, viewY, worldX, worldY);
    
    worldInteractor.Tick(deltaTime);
    
    // Update editor zoom
    worldEditorRenderer.SetVerticalSize(worldInteractor.GetZoom());

    // Update editor offset
    float xOffset, yOffset;
    float worldOffsetX, worldOffsetY;
    worldInteractor.GetOffset(xOffset, yOffset);
    worldEditorRenderer.VectorViewToWorld(xOffset, yOffset, worldOffsetX, worldOffsetY);
    if (std::isnan(worldOffsetX) || std::isnan(worldOffsetY))
    {
        worldOffsetX = 0;
        worldOffsetY = 0;
    }
    worldEditorRenderer.SetCamera(worldOffsetX, worldOffsetY, 0.0f);
}

void EditorApp::Render()
{
    window.PreRender();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    RenderMenu();
    unsigned char* renderData = shownTexture.GetTextureData();
    shownTexture.Fill(0, 0, 0);
    worldEditorRenderer.NewFrame(shownTexture.GetWidth(), shownTexture.GetHeight(), renderData);
    worldEditorRenderer.DrawGrid();
    worldInteractor.DrawFeatures(worldEditorRenderer);
    shownTexture.SendDataToOpenGl();

    window.RenderScreen();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    window.PostRender();
}

void EditorApp::LoadWorld()
{
    reader.SetPath(openPath);
    reader.Load();
    WorldConverter::ConvertWorld(editableWorld, world);
    worldInteractor.InitializeFeatureManager();
}

void EditorApp::RenderMenu()
{

    bool openFileMenu = false;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            bool p_open = true;
            if (ImGui::MenuItem("Open"))
            {
                openFileMenu = true;
            }
            if (ImGui::MenuItem("Save", NULL, false, p_open))
            {
                WorldConverter::ConvertEditableWorld(world, editableWorld);
                reader.Save("../res/test1.map");
            }
            if (ImGui::MenuItem("Save and Run", NULL, false, p_open))
            {
                WorldConverter::ConvertEditableWorld(world, editableWorld);
                reader.Save("../res/test1.map");
                system("start ../x64/Debug/MoodGame.exe ../res/test1.map");
            }
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }

    ImGuiViewport* guiViewPort = ImGui::GetMainViewport();
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(guiViewPort,dockspace_flags | ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoWindowMenuButton);

    ImGui::Begin("Parameters", nullptr);
    worldInteractor.RenderSelectedFeatureGUI();
    ImGui::End();

    if(LayoutNeedRefresh)
    {
        ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, guiViewPort->Size);
        auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.3f, nullptr, &dockspace_id);
        ImGui::DockBuilderDockWindow("Parameters", dock_id_right);
        LayoutNeedRefresh = false;
    }	
    if (openFileMenu)
    {
        ImGui::OpenPopup("Open File");
    }

    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Open File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        
        ImGui::InputText("Path", openPath, 256);
        if (ImGui::Button("Load", ImVec2(120, 0)))
        {
            LoadWorld();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
}

float EditorApp::GetFps()
{
    float sum = 0.0f;
    for (auto& lastDeltaTime : lastDeltaTimes)
    {
        sum += lastDeltaTime;
    }
    sum /= 200;
    return 1 / sum;
}


EditorApp& EditorApp::GetInstance()
{
    if (instance == nullptr)
    {
        EditorApp lol;
        instance = new EditorApp();
    }
    return *instance;
}

void EditorApp::AskForLayoutRefresh()
{
}

void EditorApp::framebufferSizeEvent(int width, int height)
{
    window.SetScreenSize(width, height);
}

void EditorApp::ScrollCallBackEvent(GLFWwindow* window, bool guiWantToCapture, double xScroll, double ySroll)
{
    if(guiWantToCapture)
    {
        return;
    }
    worldInteractor.ScrollCallBackEvent(xScroll, ySroll);
}

void EditorApp::MouseButtonCallBackEvent(GLFWwindow* window, bool guiWantToCapture, int button, int action, int mods)
{
    if(guiWantToCapture)
    {
        return;
    }
    worldInteractor.MouseButtonCallBackEvent(button, action);
}

void EditorApp::MousePositionCallBackEvent(GLFWwindow* window, bool guiWantToCapture, double xPos, double yPos)
{
    if(guiWantToCapture)
    {
        return;
    }
}

void EditorApp::KeyboardKeyCallBackEvent(GLFWwindow* window, bool guiWantToCapture, int key, int scancode, int action,
                                         int mods)
{
    if (guiWantToCapture)
    {
        return;
    }
    worldInteractor.KeyboardKeyCallBackEvent(key, scancode, action, mods);
    WindowInput::GetInstance().KeyboardKeyCallBackEvent(key, scancode, action);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    EditorApp::GetInstance().framebufferSizeEvent(width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //ImGuiIO& io = ImGui::GetIO();
    //EditorApp::GetInstance().ScrollCallBackEvent(window, io.WantCaptureMouse, xoffset, yoffset);
    EditorApp::GetInstance().ScrollCallBackEvent(window, false, xoffset, yoffset);
}

void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    //ImGuiIO& io = ImGui::GetIO();
    //EditorApp::GetInstance().MousePositionCallBackEvent(window, io.WantCaptureMouse, xpos, ypos);
    EditorApp::GetInstance().MousePositionCallBackEvent(window, false, xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //ImGuiIO& io = ImGui::GetIO();
    //EditorApp::GetInstance().KeyboardKeyCallBackEvent(window, io.WantCaptureKeyboard, key, scancode, action, mods);
    EditorApp::GetInstance().KeyboardKeyCallBackEvent(window, false, key, scancode, action, mods);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    EditorApp::GetInstance().MouseButtonCallBackEvent(window, io.WantCaptureMouse, button, action, mods);
}

void EditorApp::BindInput()
{
    window.BindframebufferSizeEvent(framebuffer_size_callback);
    window.BindScrollCallBackEvent(scroll_callback);
    window.BindMouseButtonCallBackEvent(mouse_button_callback);
    window.BindMousePositionCallBackEvent(mouse_position_callback);
    window.BindKeyboardKeyCallBackEvent(key_callback);
}

void EditorApp::WindowToViewport(float x, float y, float& outX, float& outY)
{
    size_t ww, wh;
    window.GetScreenSize(ww, wh);
    outX = x * static_cast<float>(RENDERWIDTH) / ww;
    outY = y * static_cast<float>(RENDERHEIGHT) / wh;
}

void EditorApp::WindowToWorld(float x, float y, float& outX, float& outY)
{
    WindowToViewport(x, y, x, y);
    worldEditorRenderer.ViewToWorld(x, y, outX, outY);
}

void EditorApp::DeltaWindowToWorld(float dx, float dy, float& outDx, float& outDy)
{
    WindowToViewport(dx, dy, dx, dy);
    worldEditorRenderer.VectorViewToWorld(dx, dy, outDx, outDy);
}


