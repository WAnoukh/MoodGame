#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "GameWindow.h"
#include "../src/World/WorldRenderer.h"
#include "../src/World/World.h"
#include "../src/World/WorldReader.h"
#include "GLTexture.h"
#include "../src/World/MapRenderer.h"

class GameApp
{
public:
    
    void Init();
    int Run();
    
    void Tick(float deltaTime);

    void Render();
    
    float GetFps();
    
    static GameApp& GetInstance();

    void AskForLayoutRefresh();

    void framebufferSizeEvent(int width, int height);

    void ScrollCallBackEvent(GLFWwindow* window, bool guiWantToCapture, double xOffset, double yOffset);

    void MouseButtonCallBackEvent(GLFWwindow* window, bool guiWantToCapture, int button, int action, int mods);

    void MousePositionCallBackEvent(GLFWwindow* window, bool guiWantToCapture, double xPos, double yPos);

    void KeyboardKeyCallBackEvent(GLFWwindow* window, bool guiWantToCapture, int key, int scancode, int action,
                                  int mods);

    void BindInput();
private:
    inline static GameApp* instance = nullptr;
    GameWindow window = GameWindow();

    float lastDeltaTimes[200] = {0.0f};
    int lastDeltaTimesIndex = 0;

    World world;
    WorldReader reader = WorldReader(world, "../res/DefaultMap.map");
    //WorldReader reader = WorldReader(world, "../res/wall.map");
    WorldRenderer worldRenderer = WorldRenderer(world);
    MapRenderer mapRenderer = MapRenderer(world);
    GLTexture shownTexture;
};
