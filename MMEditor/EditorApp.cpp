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
    glfwGetCursorPos(window.getGLFWwindow(), &lastMouseX, &lastMouseY);
}

void EditorApp::Tick(float deltaTime)
{
    // Get mouse displacement
    double mouseX, mouseY;
    glfwGetCursorPos(window.getGLFWwindow(), &mouseX, &mouseY);
    mouseDeltaX = mouseX - lastMouseX;
    mouseDeltaY = mouseY - lastMouseY;
    lastMouseX = mouseX;
    lastMouseY = mouseY;

    // Update editor zoom
    mapRenderer.SetVerticalSize(zoom);
    worldEditorRenderer.SetVerticalSize(zoom);

    // Update editor panning
    if (panning)
    {
        size_t screenWidth, screenHeight;
        window.GetScreenSize(screenWidth, screenHeight);
        xOffset -= (mouseDeltaX) / (screenWidth);
        yOffset += (mouseDeltaY) / (screenHeight);
    }
    float camVerticalSize = mapRenderer.GetCamVerticalSize();
    mapRenderer.SetCamera(xOffset * camVerticalSize * RENDERRATIO, yOffset * camVerticalSize, 0.0f);
    worldEditorRenderer.SetCamera(xOffset * camVerticalSize * RENDERRATIO, yOffset * camVerticalSize, 0.0f);

    if(!panning)
    {
        float worldX, worldY;
        WindowToWorld(lastMouseX, lastMouseY, worldX, worldY);
        MapFeature* foundFeature = featureManager.FindSelectedFeature(worldEditorRenderer, worldX, worldY);
        SetHoveredFeature(foundFeature);
    }else
    {
        SetHoveredFeature(nullptr);
    }

    // Move selected Feature
    if(selectedFeature != nullptr && dragging)
    {
        float worldX, worldY;
        DeltaWindowToWorld(mouseDeltaX, mouseDeltaY, worldX, worldY);
        selectedFeature->Drag(worldEditor, worldX, worldY);
    }
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
    featureManager.Draw(worldEditorRenderer);
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
    featureManager.InitializeFromWorld(editableWorld);
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
    if (selectedFeature != nullptr)
    {
        selectedFeature->RenderGui(editableWorld);
    }
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
    float zoomRate = (1 - ySroll * 0.1f);
    zoom *= zoomRate;
    xOffset /= zoomRate;
    yOffset /= zoomRate;
}

void EditorApp::MouseButtonCallBackEvent(GLFWwindow* window, bool guiWantToCapture, int button, int action, int mods)
{
    if(guiWantToCapture)
    {
        return;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            panning = true;
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
        }
        else if (action == GLFW_RELEASE)
        {
            panning = false;
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            float worldX, worldY;
            WindowToWorld(lastMouseX, lastMouseY, worldX, worldY);
            MapFeature* foundFeature = featureManager.FindSelectedFeature(worldEditorRenderer, worldX, worldY);
            if(foundFeature!=nullptr)
            {
                dragging = true;
                std::cout << "Pressed" << std::endl;
            }
            SetSelectedFeature(foundFeature);
        }
        else if (action == GLFW_RELEASE)
        {
            std::cout << "Released" << std::endl;
            dragging = false;
        }
    }
}

void EditorApp::MousePositionCallBackEvent(GLFWwindow* window, bool guiWantToCapture, double xPos, double yPos)
{
}

void EditorApp::KeyboardKeyCallBackEvent(GLFWwindow* window, bool guiWantToCapture, int key, int scancode, int action,
                                         int mods)
{
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

void EditorApp::WindowToWorld(float x, float y, float& outX, float& outY)
{
    size_t ww, wh;
    window.GetScreenSize(ww, wh);
    x *= static_cast<float>(RENDERWIDTH) / ww;
    y *= static_cast<float>(RENDERHEIGHT) / wh;
    worldEditorRenderer.ViewToWorld(x, y, outX, outY);
}

void EditorApp::DeltaWindowToWorld(float dx, float dy, float& outDx, float& outDy)
{
    size_t ww, wh;
    window.GetScreenSize(ww, wh);
    dx *= static_cast<float>(RENDERWIDTH) / ww;
    dy *= -static_cast<float>(RENDERHEIGHT) / wh;
    worldEditorRenderer.VectorViewToWorld(dx, dy, outDx, outDy);
}

void EditorApp::SetHoveredFeature(MapFeature* feature)
{
    if (hoveredFeature != nullptr)
        hoveredFeature->SetState(MapFeature::Normal);
    if(feature != nullptr && feature == selectedFeature)
        return;
    hoveredFeature = feature;
    if (hoveredFeature != nullptr)
        hoveredFeature->SetState(MapFeature::Hovered);
}

void EditorApp::SetSelectedFeature(MapFeature* feature)
{
    if (selectedFeature != nullptr)
        selectedFeature->SetState(MapFeature::Normal);
    if(feature != nullptr && feature == hoveredFeature)
        hoveredFeature = nullptr;
    selectedFeature = feature;
    if (selectedFeature != nullptr)
        selectedFeature->SetState(MapFeature::Selected);
}
