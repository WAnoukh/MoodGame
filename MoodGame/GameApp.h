#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "../WindowMaker/GameWindow.h"
#include "../src/World/WorldRenderer.h"
#include "../src/World/World.h"
#include "../src/World/WorldReader.h"
#include "../WindowMaker/GLTexture.h"
#include "../src/World/MapRenderer.h"

class GameApp
{
public:
    
    void Init(const char* startingMapPath = "../res/DefaultMap.map");
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

    void LoadLevel(const char* path);
private:
    inline static GameApp* instance = nullptr;
    GameWindow window = GameWindow();

    float lastDeltaTimes[200] = {0.0f};
    int lastDeltaTimesIndex = 0;

    World world;
    WorldReader reader = WorldReader(world);
    WorldRenderer worldRenderer = WorldRenderer(world);
    MapRenderer mapRenderer = MapRenderer(world);
    GLTexture shownTexture;
};
