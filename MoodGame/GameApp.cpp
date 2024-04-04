#include "GameApp.h"

#include <iostream>
#include <string>

#include "InputManager.h"

int GameApp::Run()
{
    float lastFrame = 0.0f;

	
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
    window.terminate();
    return 0;
}

#define RENDERHEIGHT 150
#define RENDERWIDTH 250

void GameApp::Init()
{
    if (window.init() == -1)
    {
        std::cerr << "Application : Window.init() Failed." << std::endl;
    }
    glEnable(GL_DEPTH_TEST);
    
    reader.Load();
    shownTexture.CreateBlankTexture(RENDERWIDTH,RENDERHEIGHT, GL_RGB);
    window.SetDisplayedTexture(shownTexture.GetTextureId());

    BindInput();
}

void GameApp::Tick(float deltaTime)
{

    lastDeltaTimesIndex = (lastDeltaTimesIndex + 1) % 200;
    lastDeltaTimes[lastDeltaTimesIndex] = deltaTime;
    const std::string title = "FPS : " + std::to_string(static_cast<int>(GetFps()));
    glfwSetWindowTitle(window.getGLFWwindow(), title.c_str());
    InputManager& im = InputManager::GetInstance();
    float playerXInput = im.IsKeyPressed(GLFW_KEY_W) - im.IsKeyPressed(GLFW_KEY_S);
    float playerYInput = im.IsKeyPressed(GLFW_KEY_D) - im.IsKeyPressed(GLFW_KEY_A);
    float playerStrafe = - im.IsKeyPressed(GLFW_KEY_Q) + im.IsKeyPressed(GLFW_KEY_E);
    float speed = 1.7f * deltaTime * playerXInput;
    float strafe = 1.7f * deltaTime * playerStrafe;
    float angleSpeed = -4.0f * deltaTime * playerYInput;
    float camX = worldRenderer.GetCamX(), camY = worldRenderer.GetCamY();
    float camAngle = worldRenderer.GetCamAngle();
    worldRenderer.SetCamX(camX + speed * sin(-camAngle) + strafe * cos(-camAngle));
    worldRenderer.SetCamY(camY + speed * cos(-camAngle) - strafe * sin(-camAngle));
    worldRenderer.SetCamAngle(camAngle + angleSpeed);
}

void GameApp::Render()
{
    window.PreRender();
    unsigned char* renderData = shownTexture.GetTextureData();
    worldRenderer.Render(shownTexture.GetWidth(), shownTexture.GetHeight(), renderData);
    shownTexture.SendDataToOpenGl();
    
    window.RenderScreen();
    window.PostRender();
}

float GameApp::GetFps()
{
    float sum = 0.0f;
    for (auto& lastDeltaTime : lastDeltaTimes)
    {
        sum += lastDeltaTime;
    }
    sum /= 200;
    return 1 / sum;
}


GameApp& GameApp::GetInstance()
{
    if(instance == nullptr)
    {
        instance = new GameApp();
    }
    return *instance;
}

void GameApp::AskForLayoutRefresh()
{
}

void GameApp::framebufferSizeEvent(int width, int height)
{    
    glViewport(0, 0, width, height);
}

void GameApp::ScrollCallBackEvent(GLFWwindow* window, bool guiWantToCapture, double xOffset, double yOffset)
{
    
}

void GameApp::MouseButtonCallBackEvent(GLFWwindow* window, bool guiWantToCapture, int button, int action, int mods)
{
}

void GameApp::MousePositionCallBackEvent(GLFWwindow* window, bool guiWantToCapture, double xPos, double yPos)
{
}

void GameApp::KeyboardKeyCallBackEvent(GLFWwindow* window, bool guiWantToCapture, int key, int scancode, int action,
    int mods)
{
    InputManager::GetInstance().KeyboardKeyCallBackEvent(key, scancode,action);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    GameApp::GetInstance().framebufferSizeEvent(width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{    
    //ImGuiIO& io = ImGui::GetIO();
    //GameApp::GetInstance().ScrollCallBackEvent(window, io.WantCaptureMouse, xoffset, yoffset);
    GameApp::GetInstance().ScrollCallBackEvent(window, false, xoffset, yoffset);
}

void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    //ImGuiIO& io = ImGui::GetIO();
    //GameApp::GetInstance().MousePositionCallBackEvent(window, io.WantCaptureMouse, xpos, ypos);
    GameApp::GetInstance().MousePositionCallBackEvent(window, false, xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //ImGuiIO& io = ImGui::GetIO();
    //GameApp::GetInstance().KeyboardKeyCallBackEvent(window, io.WantCaptureKeyboard, key, scancode, action, mods);
    GameApp::GetInstance().KeyboardKeyCallBackEvent(window, false, key, scancode, action, mods);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    //ImGuiIO& io = ImGui::GetIO();
    //GameApp::GetInstance().MouseButtonCallBackEvent(window, io.WantCaptureMouse, button, action, mods);
    GameApp::GetInstance().MouseButtonCallBackEvent(window, false, button, action, mods);
}

void GameApp::BindInput()
{
    window.BindframebufferSizeEvent(framebuffer_size_callback);
    window.BindScrollCallBackEvent(scroll_callback);
    window.BindMouseButtonCallBackEvent(mouse_button_callback);
    window.BindMousePositionCallBackEvent(mouse_position_callback);
    window.BindKeyboardKeyCallBackEvent(key_callback);
}
